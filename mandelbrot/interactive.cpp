// interactive.cpp -- Interactive Mandelbrot zoom: click to zoom in, 'r' to reset
#include "../fractal_common.h"

static const int MAX_ITER = 256;
static const int W = DEFAULT_WIDTH;
static const int H = DEFAULT_HEIGHT;

static int mandelbrot_escape(double cx, double cy) {
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

static void draw_mandelbrot(double xmin, double xmax, double ymin, double ymax) {
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

    double xmin = -2.0, xmax = 1.0, ymin = -1.2, ymax = 1.2;
    draw_mandelbrot(xmin, xmax, ymin, ymax);

    SDL_Event e;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_r) {
                    // Reset view
                    xmin = -2.0; xmax = 1.0; ymin = -1.2; ymax = 1.2;
                    draw_mandelbrot(xmin, xmax, ymin, ymax);
                } else if (e.key.keysym.sym == SDLK_q ||
                           e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN &&
                       e.button.button == SDL_BUTTON_LEFT) {
                // Zoom in 3x centered on click position
                int mx = e.button.x, my = e.button.y;
                double cx = xmin + (xmax - xmin) * mx / W;
                double cy = ymin + (ymax - ymin) * my / H;
                double ww = (xmax - xmin) / 3.0;
                double hh = (ymax - ymin) / 3.0;
                xmin = cx - ww / 2.0;
                xmax = cx + ww / 2.0;
                ymin = cy - hh / 2.0;
                ymax = cy + hh / 2.0;
                draw_mandelbrot(xmin, xmax, ymin, ymax);
            }
        }
        SDL_Delay(16);
    }

    closegraph();
    return 0;
}
