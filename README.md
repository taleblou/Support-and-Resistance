# srlevels (Support & Resistance) — C++

A professional, installable C++ library and CLI for detecting **support/resistance** levels.

- Methods: **horizontal**, **fibonacci**, **psychological**, **trendline**
- Unified **API** (`include/sr/*.hpp`) and **CLI** (`srlines`)
- Clean **CMake** build with **install** targets and export config
- Unit tests, CI, .clang-format, pkg-config file
- License: MIT — © 2025 **Morteza Taleblou** (<https://taleblou.ir/>)

## Build & Install
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
cmake --install build --prefix /usr/local
```

## CLI
```bash
srlines --method horizontal --csv examples/data/sample_1h.csv --min-touches 3 --tolerance 0.01
```

## Library
```cpp
#include <sr/sr_finder.hpp>
// see include/sr for full API
```

## Version
0.1.0
