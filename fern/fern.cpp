// fern.cpp -- Barnsley fern fractal via 4 weighted affine transforms
#include "../fractal_common.h"

static const int W = DEFAULT_WIDTH;
static const int H = DEFAULT_HEIGHT;
static const int ITERATIONS = 200000;

int main() {
    fractal_init();
    initgraph(W, H);
    BeginBatchDraw();

    double x = 0.0, y = 0.0;

    for (int i = 0; i < ITERATIONS; i++) {
        double nx, ny;
        int r = rand() % 100;

        if (r < 1) {
            // Stem
            nx = 0.0;
            ny = 0.16 * y;
        } else if (r < 86) {
            // Main leaf
            nx = 0.85 * x + 0.04 * y;
            ny = -0.04 * x + 0.85 * y + 1.6;
        } else if (r < 93) {
            // Left leaf
            nx = 0.20 * x - 0.26 * y;
            ny = 0.23 * x + 0.22 * y + 1.6;
        } else {
            // Right leaf
            nx = -0.15 * x + 0.28 * y;
            ny = 0.26 * x + 0.24 * y + 0.44;
        }
        x = nx;
        y = ny;

        // Map fern coords (x in [-2.2, 2.7], y in [0, 10]) to screen
        int px = (int)((x + 2.5) * W / 5.5);
        int py = (int)(H - y * H / 10.5);

        if (px >= 0 && px < W && py >= 0 && py < H) {
            // Green gradient: darker stem to lighter tips
            int g = 80 + (int)(175.0 * y / 10.0);
            if (g > 255) g = 255;
            putpixel(px, py, RGB(0, g, 0));
        }
    }

    EndBatchDraw();
    getch();
    closegraph();
    return 0;
}
