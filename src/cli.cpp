// SPDX-License-Identifier: MIT
#include <sr/sr_finder.hpp>
#include <sr/ohlcv.hpp>
#include <sr/version.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace sr;

static void usage(const char* prog){
    std::cerr << "srlevels " << SRLEVELS_VERSION << "\n"
              << "Usage:\n  " << prog << " --method {horizontal|fibonacci|psychological|trendline} --csv PATH"
              << " [--min-touches N] [--tolerance 0.005] [--lookback 300] [--step 50] [--window 3]\n";
}

int main(int argc, char** argv){
    std::string method, csv;
    int minTouches = 3;
    double tol = 0.005;
    size_t lookback = 300;
    double step = 0.0;
    int window = 3;

    for(int i=1;i<argc;i++){
        std::string a = argv[i];
        auto need = [&](int i){ if(i+1>=argc) throw std::runtime_error("Missing value for " + a); return std::string(argv[i+1]); };
        if(a=="--method"){ method = need(i); ++i; }
        else if(a=="--csv"){ csv = need(i); ++i; }
        else if(a=="--min-touches"){ minTouches = std::atoi(need(i).c_str()); ++i; }
        else if(a=="--tolerance"){ tol = std::atof(need(i).c_str()); ++i; }
        else if(a=="--lookback"){ lookback = (size_t)std::atoll(need(i).c_str()); ++i; }
        else if(a=="--step"){ step = std::atof(need(i).c_str()); ++i; }
        else if(a=="--window"){ window = std::atoi(need(i).c_str()); ++i; }
        else if(a=="-h" || a=="--help"){ usage(argv[0]); return 0; }
        else if(a=="--version"){ std::cout << SRLEVELS_VERSION << "\n"; return 0; }
    }
    if(method.empty() || csv.empty()){ usage(argv[0]); return 1; }

    OHLCVSeries s = read_csv(csv);
    std::vector<SRLevel> lvls;
    if(method=="horizontal") lvls = horizontal_levels(s, minTouches, tol);
    else if(method=="fibonacci") lvls = fibonacci_levels(s, lookback);
    else if(method=="psychological") lvls = psychological_levels(s, step);
    else if(method=="trendline") lvls = trendline_levels(s, window, tol);
    else { std::cerr<<"Unknown method\n"; return 2; }

    // Minimal JSON writer (no external deps)
    auto esc = [](const std::string& x){
        std::string o="\""; for(char c: x){ if(c=='"') o+="'"; else o+=c; } o+="\""; return o;
    };
    auto num = [](double v){ std::ostringstream o; o.setf(std::ios::fixed); o.precision(6); o<<v; return o.str(); };

    std::cout << "{\n  \"method\": " << esc(method) << ",\n  \"levels\": [\n";
    for(size_t i=0;i<lvls.size();++i){
        const auto& L = lvls[i];
        std::cout << "    {\"price\": " << num(L.price)
                  << ", \"touches\": " << L.touches
                  << ", \"strength\": " << num(L.strength)
                  << ", \"kind\": " << esc(L.kind) << "}";
        if(i+1<lvls.size()) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "  ]\n}\n";
    return 0;
}
