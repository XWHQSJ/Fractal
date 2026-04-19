// fractal_common.h -- shared boilerplate for fractal demos
#ifndef FRACTAL_COMMON_H
#define FRACTAL_COMMON_H

#ifdef EASYX_COMPAT_USE_SDL
#include "easyx_compat.h"
#else
#include <graphics.h>
#include <conio.h>
#endif

#include <cstdlib>
#include <ctime>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const int DEFAULT_WIDTH = 640;
static const int DEFAULT_HEIGHT = 480;

static inline void fractal_init() {
    srand((unsigned)time(NULL));
}

static inline COLORREF random_color() {
    return RGB(rand() % 256, rand() % 256, rand() % 256);
}

static inline COLORREF palette_color(int index, int max_index) {
    // Maps index in [0, max_index] to a smooth rainbow palette
    float hue = 360.0f * (float)index / (float)(max_index > 0 ? max_index : 1);
    return HSLtoRGB(hue, 1.0f, 0.5f);
}

#endif // FRACTAL_COMMON_H
