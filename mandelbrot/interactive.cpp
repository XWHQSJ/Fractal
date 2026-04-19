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

static double xmin_g = -2.0, xmax_g = 1.0, ymin_g = -1.2, ymax_g = 1.2;
static bool running_g = true;

static void draw_mandelbrot() {
    BeginBatchDraw();
    for (int py = 0; py < H; py++) {
        for (int px = 0; px < W; px++) {
            double cx = xmin_g + (xmax_g - xmin_g) * px / W;
            double cy = ymin_g + (ymax_g - ymin_g) * py / H;
            int n = mandelbrot_escape(cx, cy);
            COLORREF c = (n == MAX_ITER) ? BLACK : palette_color(n, MAX_ITER);
            putpixel(px, py, c);
        }
    }
    EndBatchDraw();
}

static void main_loop_iter() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running_g = false;
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_r) {
                xmin_g = -2.0; xmax_g = 1.0; ymin_g = -1.2; ymax_g = 1.2;
                draw_mandelbrot();
            } else if (e.key.keysym.sym == SDLK_q ||
                       e.key.keysym.sym == SDLK_ESCAPE) {
                running_g = false;
#ifdef __EMSCRIPTEN__
                emscripten_cancel_main_loop();
#endif
            }
        } else if (e.type == SDL_MOUSEBUTTONDOWN &&
                   e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x, my = e.button.y;
            double cx = xmin_g + (xmax_g - xmin_g) * mx / W;
            double cy = ymin_g + (ymax_g - ymin_g) * my / H;
            double ww = (xmax_g - xmin_g) / 3.0;
            double hh = (ymax_g - ymin_g) / 3.0;
            xmin_g = cx - ww / 2.0;
            xmax_g = cx + ww / 2.0;
            ymin_g = cy - hh / 2.0;
            ymax_g = cy + hh / 2.0;
            draw_mandelbrot();
        }
    }
}

int main() {
    fractal_init();
    initgraph(W, H);

    draw_mandelbrot();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop_iter, 0, 1);
#else
    while (running_g) {
        main_loop_iter();
        SDL_Delay(16);
    }
#endif

    closegraph();
    return 0;
}
