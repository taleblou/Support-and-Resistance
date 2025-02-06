#include <windows.h>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT void CalculateFibLevels(
    double high, 
    double low, 
    bool isUptrend, 
    double levels[], 
    int& numLevels
) {
    const double ratios[] = {0.236, 0.382, 0.5, 0.618, 0.786};
    const int count = sizeof(ratios) / sizeof(ratios[0]);
    numLevels = count;

    double diff = high - low;

    for (int i = 0; i < count; ++i) {
        levels[i] = isUptrend ? 
            high - diff * ratios[i] :  // Support levels for uptrend
            low + diff * ratios[i];     // Resistance levels for downtrend
    }
}