# Support & Resistance (C++)

A small, production-grade skeleton to compute **Support/Resistance** levels with multiple methods and a unified CLI/API.

## Features
- Methods: **horizontal (clustered highs/lows)**, **fibonacci**, **psychological (round levels)**, **trendline**.
- **Single CLI** (`srlines`) with JSON output.
- Clean CMake build, unit tests, and CI workflow (GitHub Actions).
- Header-only vendor snippets (tiny JSON writer + minimal doctest) to avoid heavy dependencies.

> License: MIT — © 2025 Morteza Taleblou (https://taleblou.ir/)

## Build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

## Run (example)
```bash
./build/srlines --method horizontal --csv examples/data/sample_1h.csv --min-touches 3 --tolerance 0.01
./build/srlines --method fibonacci   --csv examples/data/sample_1h.csv --lookback 200
./build/srlines --method psychological --csv examples/data/sample_1h.csv --step 1
./build/srlines --method trendline   --csv examples/data/sample_1h.csv --window 3 --tolerance 0.01
```

Output is JSON like:
```json
{
  "method": "horizontal",
  "levels": [
    {"price": 101.2, "touches": 4, "strength": 3.8, "kind":"horizontal"}
  ]
}
```

## API
Public headers in `include/sr`:
```cpp
struct SRLevel { double price; int touches; double strength; std::string kind; };
std::vector<SRLevel> horizontal_levels(const OHLCVSeries&, int min_touches=3, double tolerance_ratio=0.005);
std::vector<SRLevel> fibonacci_levels(const OHLCVSeries&, size_t lookback=300);
std::vector<SRLevel> psychological_levels(const OHLCVSeries&, double step=0.0);
std::vector<SRLevel> trendline_levels(const OHLCVSeries&, int window=3, double tolerance_ratio=0.005);
```

## Tests
```bash
ctest --test-dir build --output-on-failure
```

## Notes
- Algorithms here are **robust baselines** and intentionally simple. You can iterate:
  - Horizontal: replace histogram binning with DBSCAN/KMeans over pivot points.
  - Fibonacci: smarter swing detection (ZigZag) + ATR-based tolerance.
  - Psychological: ATR-adaptive step (e.g., nearest {10, 50, 100} * pow10).
  - Trendline: RANSAC / Hough-like voting + multi-line detection, not only latest two pivots.
- For Python bindings later, add `pybind11` and expose the functions above.

