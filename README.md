# Fractal

**EasyX for Fractal Programs**

![C++](https://img.shields.io/badge/language-C%2B%2B-blue)
![License: MIT](https://img.shields.io/badge/license-MIT-green)
![Platform: Windows](https://img.shields.io/badge/platform-Windows-lightgrey)

## About

A collection of classic fractal and graphics programs implemented in C++ using the [EasyX Graphics Library](https://easyx.cn). Each subdirectory contains a self-contained demo that renders a fractal or visual effect.

---

## Gallery

| Fractal / Demo | Source | Description |
|---|---|---|
| Sierpinski Triangle | [`sierpinski/sierpinski.cpp`](sierpinski/sierpinski.cpp) | Sierpinski triangle via recursive subdivision |
| Sierpinski Carpet | [`sierpinski/sierpinskiCarpet.cpp`](sierpinski/sierpinskiCarpet.cpp) | Sierpinski carpet fractal |
| Cantor Set | [`cantor/cantor.cpp`](cantor/cantor.cpp) | Cantor set line fractal |
| Koch Snowflake (EasyX) | [`kochsnow/kochsnowByEasyX.cpp`](kochsnow/kochsnowByEasyX.cpp) | Koch snowflake rendered with EasyX |
| Koch Snowflake (ASCII) | [`kochsnow/kochsnowByASCII.cpp`](kochsnow/kochsnowByASCII.cpp) | Koch snowflake rendered as ASCII art |
| Arborescent Tree | [`arboresent/arboresent/arboresent.cpp`](arboresent/arboresent/arboresent.cpp) | Barnsley-style recursive tree |
| Triangle | [`triangle/triangle.cpp`](triangle/triangle.cpp) | Triangle drawing demo |
| Stars | [`stars/stars.cpp`](stars/stars.cpp) | Starfield visual effect |
| Rainbow | [`rainbow/rainbow.cpp`](rainbow/rainbow.cpp) | Rainbow arc rendering |
| Mouse / Cursor Effect | [`mouse/mouse.cpp`](mouse/mouse.cpp) | Mouse-driven cursor visual effect |

## Note on Mario2

The [`Mario2/`](Mario2/) directory contains a separate small Mario-style platformer game built with EasyX. It is **not a fractal program** but is included as bonus content.

**Mario2 source files:** `main.cpp`, `control.cpp`, `role.cpp`, `rule.cpp`, `scene.cpp` and associated headers.

---

## Requirements

- **OS:** Windows
- **Compiler:** Visual Studio 2017+ or Visual C++ 6.0
- **Graphics Library:** [EasyX Graphics Library](https://easyx.cn) (install before compiling)

## Build and Run

1. Install [EasyX](https://easyx.cn) for your version of Visual Studio.
2. Open the `.cpp` file for any demo in Visual Studio.
3. Compile and run. Each program opens a graphics window rendering the fractal or effect.

> Some older demos may target Visual C++ 6.0. Adjust project settings if using a newer compiler.

## References

- [EasyX Graphics Library (Official Site)](https://easyx.cn)
- [EasyX Documentation](https://docs.easyx.cn)

## License

[MIT](LICENSE) -- Copyright (c) 2019 XWHQSJ
