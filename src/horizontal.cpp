// SPDX-License-Identifier: MIT
#include <sr/sr_finder.hpp>
#include <sr/utils.hpp>
#include <map>
#include <cmath>
#include <algorithm>

namespace sr {

static double infer_bin(double ref_price, double tol) {
    return std::max(1e-9, std::abs(ref_price) * tol);
}

std::vector<SRLevel> horizontal_levels(const OHLCVSeries& data, int min_touches, double tolerance_ratio) {
    std::vector<SRLevel> out;
    if (data.size() == 0) return out;
    if (tolerance_ratio <= 0.0) tolerance_ratio = 0.005; // 0.5%

    std::vector<double> points; points.reserve(data.size() * 2);
    for (const auto& r : data.rows) { points.push_back(r.high); points.push_back(r.low); }

    std::vector<double> closes; closes.reserve(data.size());
    for (const auto& r : data.rows) closes.push_back(r.close);
    double ref = sr::utils::median(closes);
    double bin = infer_bin(ref, tolerance_ratio);

    std::map<long long, std::vector<double>> buckets;
    for (double p : points) {
        long long k = static_cast<long long>(std::floor(p / bin));
        buckets[k].push_back(p);
    }

    for (auto& kv : buckets) {
        auto& vec = kv.second;
        if ((int)vec.size() < min_touches) continue;
        double sum = 0.0, minp = vec[0], maxp = vec[0];
        for (double v : vec) { sum += v; minp = std::min(minp, v); maxp = std::max(maxp, v); }
        double center = sum / vec.size();
        double spread = std::max(1e-12, maxp - minp);
        double strength = vec.size() / (1.0 + spread / bin);
        out.push_back(SRLevel{center, (int)vec.size(), strength, "horizontal"});
    }
    std::sort(out.begin(), out.end(), [](const SRLevel& a, const SRLevel& b){ return a.strength > b.strength; });
    return out;
}

} // namespace sr
