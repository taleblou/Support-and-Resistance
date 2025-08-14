// SPDX-License-Identifier: MIT
#include <sr/ohlcv.hpp>
#include <fstream>
#include <sstream>
#include <cmath>

namespace sr {

OHLCVSeries read_csv(const std::string& path){
    std::ifstream f(path);
    if(!f) throw std::runtime_error("Cannot open CSV: " + path);
    std::string line;
    std::getline(f, line); // header
    OHLCVSeries s;
    while(std::getline(f, line)){
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::string tok; OHLCV r{};
        std::getline(ss, tok, ','); r.ts = std::stoll(tok);
        std::getline(ss, tok, ','); r.open = std::stod(tok);
        std::getline(ss, tok, ','); r.high = std::stod(tok);
        std::getline(ss, tok, ','); r.low = std::stod(tok);
        std::getline(ss, tok, ','); r.close = std::stod(tok);
        std::getline(ss, tok, ','); r.volume = std::stod(tok);
        s.rows.push_back(r);
    }
    return s;
}

void write_csv(const std::string& path, const OHLCVSeries& s){
    std::ofstream f(path);
    f << "ts,open,high,low,close,volume\n";
    for(const auto& r : s.rows){
        f << r.ts << "," << r.open << "," << r.high << "," << r.low << "," << r.close << "," << r.volume << "\n";
    }
}

double rolling_mean(const std::vector<double>& v, int end_idx, int window){
    if(window <= 0 || end_idx < 0 || end_idx >= (int)v.size()) return 0.0;
    int start = std::max(0, end_idx - window + 1);
    double sum = 0.0;
    for(int i=start;i<=end_idx;i++) sum += v[i];
    return sum / (end_idx - start + 1);
}

static double tr(const OHLCV& cur, const OHLCV* prev){
    if(!prev) return cur.high - cur.low;
    double a = cur.high - cur.low;
    double b = std::fabs(cur.high - prev->close);
    double c = std::fabs(cur.low - prev->close);
    return std::max(a, std::max(b,c));
}

double atr(const OHLCVSeries& s, int end_idx, int window){
    if(s.size() == 0 || window <= 0 || end_idx <= 0) return 0.0;
    end_idx = std::min(end_idx, (int)s.size()-1);
    double sum = 0.0; int cnt = 0;
    for(int i=end_idx; i>0 && cnt<window; --i, ++cnt){
        sum += tr(s.rows[i], &s.rows[i-1]);
    }
    if(cnt == 0) return 0.0;
    return sum / cnt;
}

std::vector<int> zigzag_pivots(const OHLCVSeries& s, double pct, bool use_highs){
    std::vector<int> piv;
    if(s.size() < 3) return piv;
    double threshold = pct/100.0;
    int last = 0;
    double last_price = use_highs ? s.rows[0].high : s.rows[0].low;
    for(size_t i=1;i<s.size();++i){
        double price = use_highs ? s.rows[i].high : s.rows[i].low;
        if(std::fabs(price - last_price)/std::max(1e-9, last_price) >= threshold){
            piv.push_back((int)i);
            last_price = price;
            last = (int)i;
        }
    }
    return piv;
}

} // namespace sr
