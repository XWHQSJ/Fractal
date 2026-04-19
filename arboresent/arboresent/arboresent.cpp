// Recursive branching tree fractal
#ifdef EASYX_COMPAT_USE_SDL
#include "../../easyx_compat.h"
#else
#include <graphics.h>
#include <conio.h>
#endif

#include <cmath>
#include <cstdlib>
#include <ctime>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const int MAX_DEPTH = 8;
static const double TRUNK_LEN = 100.0;
static const double ANGLE_SPREAD = 30.0;
static const double LEN_RATIO = 0.72;

// Brown-to-green gradient based on depth
static COLORREF branchColor(int depth, int maxDepth) {
    double t = (double)depth / maxDepth;
    // brown (139,69,19) -> green (34,139,34)
    int r = (int)(139 + t * (34 - 139));
    int g = (int)(69 + t * (139 - 69));
    int b = (int)(19 + t * (34 - 19));
    return RGB(r, g, b);
}

void arboresent(double x, double y, double angle, double length, int depth) {
    if (depth <= 0 || length < 2.0) return;

    double x2 = x + length * cos(angle * M_PI / 180.0);
    double y2 = y - length * sin(angle * M_PI / 180.0);

    setlinecolor(branchColor(MAX_DEPTH - depth, MAX_DEPTH));
    line(x, y, x2, y2);

    double newLen = length * LEN_RATIO;
    // Small random variation for natural look
    double jitter = (rand() % 11 - 5) * 0.5;

    arboresent(x2, y2, angle + ANGLE_SPREAD + jitter, newLen, depth - 1);
    arboresent(x2, y2, angle - ANGLE_SPREAD + jitter, newLen, depth - 1);
    // Smaller center branch for richer canopy
    if (depth > 2) {
        arboresent(x2, y2, angle + jitter * 0.5, newLen * 0.85, depth - 2);
    }
}

int main() {
    srand((unsigned)time(NULL));

    initgraph(640, 480);

    // Draw trunk starting from bottom center, growing upward (90 degrees)
    arboresent(320, 450, 90.0, TRUNK_LEN, MAX_DEPTH);

    getch();
    closegraph();

    return 0;
}
