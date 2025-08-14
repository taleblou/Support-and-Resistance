// SPDX-License-Identifier: MIT
#include <sr/sr_finder.hpp>
#include <algorithm>
#include <cmath>

namespace sr {

std::vector<SRLevel> fibonacci_levels(const OHLCVSeries& data, size_t lookback) {
    std::vector<SRLevel> out;
    if (data.size() < 2) return out;
    if (lookback == 0 || lookback > data.size()) lookback = std::min<size_t>(data.size(), 300);

    size_t start = data.size() - lookback;
    double high = data.rows[start].high, low = data.rows[start].low;
    for (size_t i = start; i < data.size(); ++i) {
        high = std::max(high, data.rows[i].high);
        low  = std::min(low,  data.rows[i].low);
    }
    double range = std::max(1e-12, high - low);

    const double fibs[] = {0.236, 0.382, 0.5, 0.618, 0.786};
    for (double f : fibs) {
        double level = high - f * range;
        out.push_back(SRLevel{level, 0, 0.0, "fibonacci"});
    }

    double tol_ratio = 0.0025; // 0.25%
    for (auto& lvl : out) {
        double tol = std::fabs(lvl.price) * tol_ratio;
        int touches = 0;
        for (size_t i = start; i < data.size(); ++i) {
            const auto& r = data.rows[i];
            if (std::abs(r.low - lvl.price) <= tol || std::abs(r.high - lvl.price) <= tol ||
                (r.low <= lvl.price && r.high >= lvl.price)) {
                touches++;
            }
        }
        lvl.touches = touches;
        lvl.strength = touches;
    }
    std::sort(out.begin(), out.end(), [](const SRLevel& a, const SRLevel& b){ return a.strength > b.strength; });
    return out;
}

} // namespace sr
