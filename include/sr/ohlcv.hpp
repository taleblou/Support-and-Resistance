// SPDX-License-Identifier: MIT
#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>

namespace sr {

struct OHLCV {
    std::int64_t ts; // unix seconds
    double open, high, low, close, volume;
};

struct OHLCVSeries {
    std::vector<OHLCV> rows;
    bool empty() const { return rows.empty(); }
    size_t size() const { return rows.size(); }
    const OHLCV& operator[](size_t i) const { return rows[i]; }
    OHLCV& operator[](size_t i) { return rows[i]; }
};

// CSV IO (header: ts,open,high,low,close,volume)
OHLCVSeries read_csv(const std::string& path);
void write_csv(const std::string& path, const OHLCVSeries& s);

// Indicators/utilities
double rolling_mean(const std::vector<double>& v, int end_idx, int window);
double atr(const OHLCVSeries& s, int end_idx, int window);

// Pivot detection (simple ZigZag with percent threshold)
std::vector<int> zigzag_pivots(const OHLCVSeries& s, double pct, bool use_highs=true);

} // namespace sr
