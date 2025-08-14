// SPDX-License-Identifier: MIT
#include <sr/sr_finder.hpp>
#include <sr/ohlcv.hpp>
#include <cassert>
#include <iostream>

using namespace sr;

static OHLCV make(std::int64_t t, double o,double h,double l,double c,double v){
    return OHLCV{t,o,h,l,c,v};
}

int main(){
    // Build small synthetic data
    OHLCVSeries s;
    for (int i=0;i<500;i++) s.rows.push_back(make(i, i, i+1, i-1, i, 1000));

    // Horizontal
    auto h = horizontal_levels(s, 3, 0.02);
    assert(h.size() >= 1);

    // Fibonacci
    auto f = fibonacci_levels(s, 300);
    assert(f.size() >= 5);

    // Psychological
    auto p = psychological_levels(s, 0.0);
    assert(p.size() > 0);

    // Trendline
    auto t = trendline_levels(s, 3, 0.01);
    assert(t.size() > 0);

    std::cout << "All tests passed.\n";
    return 0;
}
