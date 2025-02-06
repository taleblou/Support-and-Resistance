#include <windows.h>
#include <cmath>

// Export the function for MQL
#define EXPORT extern "C" __declspec(dllexport)

EXPORT void __stdcall GetPsychologicalLevels(
    double price,        // Current price (from MQL)
    double* supports,    // Array to store support levels
    double* resistances, // Array to store resistance levels
    int levelsCount      // Number of levels to calculate (e.g., 3)
) {
    // Calculate base level (rounded to nearest 100 pips/1.00 for JPY pairs)
    double base = std::round(price);

    // Generate support and resistance levels
    for (int i = 0; i < levelsCount; ++i) {
        supports[i] = base - (i + 1) * 1.0;    // Supports below price
        resistances[i] = base + (i + 1) * 1.0; // Resistances above price
    }
}