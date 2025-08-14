// SPDX-License-Identifier: MIT
#pragma once
#include "ohlcv.hpp"
#include <vector>
#include <string>

namespace sr {

struct SRLevel {
    double price = 0.0;
    int touches = 0;
    double strength = 0.0;
    std::string kind; // "horizontal","fibonacci","psychological","trendline_support","trendline_resistance"
};

std::vector<SRLevel> horizontal_levels(const OHLCVSeries& data, int min_touches, double tolerance_ratio);
std::vector<SRLevel> fibonacci_levels(const OHLCVSeries& data, size_t lookback);
std::vector<SRLevel> psychological_levels(const OHLCVSeries& data, double step);
std::vector<SRLevel> trendline_levels(const OHLCVSeries& data, int window, double tolerance_ratio);

} // namespace sr
