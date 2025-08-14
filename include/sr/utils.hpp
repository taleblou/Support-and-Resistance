// SPDX-License-Identifier: MIT
#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace sr::utils {

inline double median(std::vector<double> v){
    if(v.empty()) return 0.0;
    auto m = v.begin() + v.size()/2;
    std::nth_element(v.begin(), m, v.end());
    return *m;
}
inline double clamp(double x, double lo, double hi){
    return std::max(lo, std::min(hi, x));
}

} // namespace sr::utils
