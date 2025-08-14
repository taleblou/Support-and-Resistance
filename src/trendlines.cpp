// SPDX-License-Identifier: MIT
#include <sr/sr_finder.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

namespace sr {

static bool is_local_min(const OHLCVSeries& d, int i, int w){
    for(int k = i-w; k <= i+w; ++k){
        if(k<0 || k>= (int)d.size() || k==i) continue;
        if(d.rows[k].low < d.rows[i].low) return false;
    }
    return true;
}
static bool is_local_max(const OHLCVSeries& d, int i, int w){
    for(int k = i-w; k <= i+w; ++k){
        if(k<0 || k>= (int)d.size() || k==i) continue;
        if(d.rows[k].high > d.rows[i].high) return false;
    }
    return true;
}

std::vector<SRLevel> trendline_levels(const OHLCVSeries& data, int window, double tolerance_ratio) {
    std::vector<SRLevel> out;
    if (data.size() < (size_t)std::max(5, 2*window+1)) return out;
    if (window < 1) window = 3;
    if (tolerance_ratio <= 0.0) tolerance_ratio = 0.005; // 0.5%

    std::vector<int> mins, maxs;
    for(int i = window; i < (int)data.size()-window; ++i){
        if(is_local_min(data,i,window)) mins.push_back(i);
        if(is_local_max(data,i,window)) maxs.push_back(i);
    }
    if (mins.size() >= 2) {
        int i1 = mins[mins.size()-2], i2 = mins[mins.size()-1];
        double m = (data.rows[i2].low - data.rows[i1].low) / (double)(i2 - i1);
        double b = data.rows[i1].low - m * i1;
        int touches = 0;
        for (int i = 0; i < (int)data.size(); ++i) {
            double y = m*i + b;
            double tol = std::max(1e-9, tolerance_ratio * std::fabs(y));
            if (std::abs(data.rows[i].low - y) <= tol || data.rows[i].low <= y) touches++;
        }
        double y_last = m*((int)data.size()-1) + b;
        out.push_back(SRLevel{y_last, touches, (double)touches, "trendline_support"});
    }
    if (maxs.size() >= 2) {
        int i1 = maxs[maxs.size()-2], i2 = maxs[maxs.size()-1];
        double m = (data.rows[i2].high - data.rows[i1].high) / (double)(i2 - i1);
        double b = data.rows[i1].high - m * i1;
        int touches = 0;
        for (int i = 0; i < (int)data.size(); ++i) {
            double y = m*i + b;
            double tol = std::max(1e-9, tolerance_ratio * std::fabs(y));
            if (std::abs(data.rows[i].high - y) <= tol || data.rows[i].high >= y) touches++;
        }
        double y_last = m*((int)data.size()-1) + b;
        out.push_back(SRLevel{y_last, touches, (double)touches, "trendline_resistance"});
    }
    std::sort(out.begin(), out.end(), [](const SRLevel& a, const SRLevel& b){ return a.strength > b.strength; });
    return out;
}

} // namespace sr
