cmake_minimum_required(VERSION 3.16)
project(Fractal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- SDL2 backend (default on non-Windows) ----
option(EASYX_COMPAT_USE_SDL "Use SDL2 backend instead of native EasyX" ON)

if(EASYX_COMPAT_USE_SDL)
    # Try cmake config first, then pkg-config fallback
    find_package(SDL2 QUIET)
    if(NOT SDL2_FOUND)
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(SDL2 IMPORTED_TARGET sdl2)
        endif()
    endif()

    if(NOT SDL2_FOUND AND NOT TARGET PkgConfig::SDL2)
        message(FATAL_ERROR
            "SDL2 not found. Install it:\n"
            "  macOS:  brew install sdl2\n"
            "  Ubuntu: sudo apt install libsdl2-dev\n"
            "  Fedora: sudo dnf install SDL2-devel\n"
        )
    endif()
endif()

# Helper: add a fractal demo target
function(add_fractal_demo name source)
    add_executable(${name} ${source})
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})
    target_compile_features(${name} PRIVATE cxx_std_17)
    if(EASYX_COMPAT_USE_SDL)
        target_compile_definitions(${name} PRIVATE EASYX_COMPAT_USE_SDL)
        if(TARGET SDL2::SDL2)
            target_link_libraries(${name} PRIVATE SDL2::SDL2 SDL2::SDL2main)
        elseif(TARGET PkgConfig::SDL2)
            target_link_libraries(${name} PRIVATE PkgConfig::SDL2)
        else()
            target_include_directories(${name} PRIVATE ${SDL2_INCLUDE_DIRS})
            target_link_libraries(${name} PRIVATE ${SDL2_LIBRARIES})
        endif()
    endif()
endfunction()

# ---- Existing demos ----
add_fractal_demo(arboresent   arboresent/arboresent/arboresent.cpp)
add_fractal_demo(sierpinski   sierpinski/sierpinski.cpp)
add_fractal_demo(sierpinski_carpet sierpinski/sierpinskiCarpet.cpp)
add_fractal_demo(cantor       cantor/cantor.cpp)
add_fractal_demo(kochsnow     kochsnow/kochsnowByEasyX.cpp)
add_fractal_demo(rainbow      rainbow/rainbow.cpp)
add_fractal_demo(stars        stars/stars.cpp)
add_fractal_demo(triangle     triangle/triangle.cpp)
add_fractal_demo(mouse_demo   mouse/mouse.cpp)

# ---- New demos (Tier 2) ----
add_fractal_demo(mandelbrot   mandelbrot/mandelbrot.cpp)
add_fractal_demo(fern         fern/fern.cpp)
add_fractal_demo(dragon       dragon/dragon.cpp)
add_fractal_demo(hilbert      hilbert/hilbert.cpp)

# ---- Tier 3 demos ----
add_fractal_demo(lsystem_demo lsystem/demo.cpp)
add_fractal_demo(mandelbrot_interactive mandelbrot/interactive.cpp)

# ---- Tests ----
enable_testing()
add_subdirectory(tests)
