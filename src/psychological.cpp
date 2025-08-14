// SPDX-License-Identifier: MIT
#include <sr/sr_finder.hpp>
#include <sr/utils.hpp>
#include <cmath>
#include <algorithm>
#include <unordered_map>

namespace sr {

static double round_to_step(double value, double step) {
    return std::round(value / step) * step;
}

std::vector<SRLevel> psychological_levels(const OHLCVSeries& data, double step) {
    std::vector<SRLevel> out;
    if (data.size() == 0) return out;

    if (step <= 0) {
        std::vector<double> closes; closes.reserve(data.size());
        for (const auto& r : data.rows) closes.push_back(r.close);
        double ref = sr::utils::median(closes);
        double pow10 = std::pow(10.0, std::floor(std::log10(std::max(1e-9, ref))) - 1);
        step = std::max( pow10, ref * 0.001 );
    }

    std::unordered_map<long long, int> counts;
    for (const auto& r : data.rows) {
        double rp = round_to_step(r.close, step);
        long long k = (long long)std::llround(rp / step);
        counts[k]++;
    }

    out.reserve(counts.size());
    for (auto& kv : counts) {
        double price = kv.first * step;
        int touches = kv.second;
        double strength = touches;
        out.push_back(SRLevel{price, touches, strength, "psychological"});
    }
    std::sort(out.begin(), out.end(), [](const SRLevel& a, const SRLevel& b){ return a.strength > b.strength; });
    return out;
}

} // namespace sr
