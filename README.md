# Fractal

**Cross-platform fractal programs in C++**

![C++](https://img.shields.io/badge/language-C%2B%2B17-blue)
![License: MIT](https://img.shields.io/badge/license-MIT-green)
![Platform: macOS/Linux/Windows](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey)

## About

A collection of classic fractal and graphics programs implemented in C++. Originally built with [EasyX](https://easyx.cn) (Windows), now cross-platform via an SDL2 compatibility layer.

---

## Gallery

| Fractal / Demo | Source | Description |
|---|---|---|
| Sierpinski Triangle | [`sierpinski/sierpinski.cpp`](sierpinski/sierpinski.cpp) | Chaos-game Sierpinski triangle |
| Sierpinski Carpet | [`sierpinski/sierpinskiCarpet.cpp`](sierpinski/sierpinskiCarpet.cpp) | Recursive carpet fractal |
| Cantor Set | [`cantor/cantor.cpp`](cantor/cantor.cpp) | Cantor set line fractal |
| Koch Snowflake (EasyX) | [`kochsnow/kochsnowByEasyX.cpp`](kochsnow/kochsnowByEasyX.cpp) | Koch snowflake |
| Koch Snowflake (ASCII) | [`kochsnow/kochsnowByASCII.cpp`](kochsnow/kochsnowByASCII.cpp) | Koch snowflake as ASCII art |
| Arborescent Tree | [`arboresent/arboresent/arboresent.cpp`](arboresent/arboresent/arboresent.cpp) | Recursive branching tree with depth-gradient coloring |
| Triangle | [`triangle/triangle.cpp`](triangle/triangle.cpp) | Triangle drawing demo |
| Stars | [`stars/stars.cpp`](stars/stars.cpp) | Starfield visual effect |
| Rainbow | [`rainbow/rainbow.cpp`](rainbow/rainbow.cpp) | Rainbow arc rendering |
| Mouse / Cursor Effect | [`mouse/mouse.cpp`](mouse/mouse.cpp) | Mouse-driven cursor visual effect |

### New Fractals

| Fractal | Source | Description |
|---|---|---|
| Mandelbrot Set | [`mandelbrot/mandelbrot.cpp`](mandelbrot/mandelbrot.cpp) | Escape-time Mandelbrot with 256-color rainbow palette |
| Interactive Mandelbrot | [`mandelbrot/interactive.cpp`](mandelbrot/interactive.cpp) | Click-to-zoom Mandelbrot explorer (press R to reset) |
| Barnsley Fern | [`fern/fern.cpp`](fern/fern.cpp) | 4 weighted affine transforms, 200k iterations |
| Dragon Curve | [`dragon/dragon.cpp`](dragon/dragon.cpp) | Recursive paper-folding curve |
| Hilbert Curve | [`hilbert/hilbert.cpp`](hilbert/hilbert.cpp) | Order-5 space-filling curve |
| L-System Interpreter | [`lsystem/demo.cpp`](lsystem/demo.cpp) | General L-system engine with Koch, Dragon, Plant demos |

**[HTML Gallery with math formulas](docs/index.html)**

---

## Note on Mario2

The [`Mario2/`](Mario2/) directory contains a separate small Mario-style platformer game built with EasyX. It is **not a fractal program** but is included as bonus content.

---

## Cross-Platform Build

### Prerequisites

| Platform | Install SDL2 |
|----------|-------------|
| macOS | `brew install sdl2` |
| Ubuntu/Debian | `sudo apt install libsdl2-dev` |
| Fedora | `sudo dnf install SDL2-devel` |
| Windows | Use native EasyX, or install SDL2 and pass `-DEASYX_COMPAT_USE_SDL=ON` |

### Build

```bash
brew install sdl2          # macOS
cmake -B build -S .
cmake --build build
```

### Tests

```bash
ctest --test-dir build --output-on-failure
```

10 unit tests covering Koch midpoint math, Mandelbrot escape counts, Sierpinski convergence, and L-system rule expansion.

---

## Architecture

- **`easyx_compat.h`** -- Single-header SDL2 shim implementing the EasyX API subset used by all demos. On Windows with real EasyX installed, it's a no-op.
- **`fractal_common.h`** -- Shared boilerplate: RNG init, random/palette color helpers, standard window dimensions.
- **`lsystem/lsystem.h`** -- Reusable L-system interpreter with turtle graphics renderer.
- **`CMakeLists.txt`** -- Top-level build with per-demo targets and GoogleTest integration.

## References

- [EasyX Graphics Library (Official Site)](https://easyx.cn)
- [SDL2 Wiki](https://wiki.libsdl.org/)

## License

[MIT](LICENSE) -- Copyright (c) 2019 XWHQSJ
