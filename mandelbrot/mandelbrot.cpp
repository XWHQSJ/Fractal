// mandelbrot.cpp -- Mandelbrot set fractal with escape-time palette
#include "../fractal_common.h"

static const int MAX_ITER = 256;
static const int W = DEFAULT_WIDTH;
static const int H = DEFAULT_HEIGHT;

int mandelbrot_escape(double cx, double cy) {
    double zx = 0.0, zy = 0.0;
    int i = 0;
    while (zx * zx + zy * zy <= 4.0 && i < MAX_ITER) {
        double tmp = zx * zx - zy * zy + cx;
        zy = 2.0 * zx * zy + cy;
        zx = tmp;
        i++;
    }
    return i;
}

void draw_mandelbrot(double xmin, double xmax, double ymin, double ymax) {
    BeginBatchDraw();
    for (int py = 0; py < H; py++) {
        for (int px = 0; px < W; px++) {
            double cx = xmin + (xmax - xmin) * px / W;
            double cy = ymin + (ymax - ymin) * py / H;
            int n = mandelbrot_escape(cx, cy);
            COLORREF c = (n == MAX_ITER) ? BLACK : palette_color(n, MAX_ITER);
            putpixel(px, py, c);
        }
    }
    EndBatchDraw();
}

int main() {
    fractal_init();
    initgraph(W, H);

    draw_mandelbrot(-2.0, 1.0, -1.2, 1.2);

    getch();
    closegraph();
    return 0;
}
