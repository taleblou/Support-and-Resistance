// SupportResistanceDLL.cpp
#include <vector>
#include <set>
#include <algorithm>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT void CalculateSupportResistance(const double* highs, const double* lows, int count, 
                                      int lookback, double* supports, double* resistances, 
                                      int& numSupports, int& numResistances) {
    std::vector<double> swingHighs, swingLows;

    // Identify swing highs and lows
    for(int i = lookback; i < count - lookback; ++i) {
        bool isHigh = true, isLow = true;
        
        for(int j = 1; j <= lookback; ++j) {
            if(highs[i] <= highs[i+j] || highs[i] <= highs[i-j]) {
                isHigh = false;
            }
            if(lows[i] >= lows[i+j] || lows[i] >= lows[i-j]) {
                isLow = false;
            }
        }
        
        if(isHigh) swingHighs.push_back(highs[i]);
        if(isLow) swingLows.push_back(lows[i]);
    }

    // Get unique levels (sorted)
    std::set<double, std::greater<double>> uniqueResistances(swingHighs.begin(), swingHighs.end());
    std::set<double> uniqueSupports(swingLows.begin(), swingLows.end());

    // Extract top 3 levels
    numResistances = 0;
    auto rit = uniqueResistances.begin();
    while(rit != uniqueResistances.end() && numResistances < 3) {
        resistances[numResistances++] = *rit++;
    }

    numSupports = 0;
    auto sit = uniqueSupports.begin();
    while(sit != uniqueSupports.end() && numSupports < 3) {
        supports[numSupports++] = *sit++;
    }
}