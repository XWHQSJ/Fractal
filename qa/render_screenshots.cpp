// render_screenshots.cpp — Standalone fractal screenshot generator
// Produces PNG files for README gallery using pure math + stb_image_write.
// No SDL or windowing dependencies required.

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Pixel buffer helper
// ---------------------------------------------------------------------------
struct Image {
    int w, h;
    std::vector<uint8_t> data; // RGBA

    Image(int width, int height)
        : w(width), h(height), data(width * height * 4, 0) {}

    void set(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        if (x < 0 || x >= w || y < 0 || y >= h) return;
        auto idx = (y * w + x) * 4;
        data[idx + 0] = r;
        data[idx + 1] = g;
        data[idx + 2] = b;
        data[idx + 3] = a;
    }

    void fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        for (int i = 0; i < w * h; ++i) {
            data[i * 4 + 0] = r;
            data[i * 4 + 1] = g;
            data[i * 4 + 2] = b;
            data[i * 4 + 3] = a;
        }
    }

    // Bresenham line
    void line(int x0, int y0, int x1, int y1,
              uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;
        for (;;) {
            set(x0, y0, r, g, b, a);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }

    // Thick line (draw multiple offsets)
    void thick_line(int x0, int y0, int x1, int y1,
                    int thickness,
                    uint8_t r, uint8_t g, uint8_t b) {
        int half = thickness / 2;
        for (int dx = -half; dx <= half; ++dx) {
            for (int dy = -half; dy <= half; ++dy) {
                line(x0 + dx, y0 + dy, x1 + dx, y1 + dy, r, g, b);
            }
        }
    }

    void fill_rect(int x0, int y0, int x1, int y1,
                   uint8_t r, uint8_t g, uint8_t b) {
        for (int y = std::max(0, y0); y <= std::min(h - 1, y1); ++y)
            for (int x = std::max(0, x0); x <= std::min(w - 1, x1); ++x)
                set(x, y, r, g, b);
    }

    bool save_png(const std::string& path) const {
        return stbi_write_png(path.c_str(), w, h, 4, data.data(), w * 4) != 0;
    }
};

// ---------------------------------------------------------------------------
// Color utilities
// ---------------------------------------------------------------------------
struct RGB { uint8_t r, g, b; };

// HSV to RGB (h in [0,360), s,v in [0,1])
static RGB hsv_to_rgb(double h, double s, double v) {
    double c = v * s;
    double x = c * (1.0 - std::abs(std::fmod(h / 60.0, 2.0) - 1.0));
    double m = v - c;
    double r1, g1, b1;
    if (h < 60)       { r1 = c; g1 = x; b1 = 0; }
    else if (h < 120) { r1 = x; g1 = c; b1 = 0; }
    else if (h < 180) { r1 = 0; g1 = c; b1 = x; }
    else if (h < 240) { r1 = 0; g1 = x; b1 = c; }
    else if (h < 300) { r1 = x; g1 = 0; b1 = c; }
    else               { r1 = c; g1 = 0; b1 = x; }
    return {
        static_cast<uint8_t>((r1 + m) * 255),
        static_cast<uint8_t>((g1 + m) * 255),
        static_cast<uint8_t>((b1 + m) * 255)
    };
}

// ---------------------------------------------------------------------------
// 1. Mandelbrot Set (800x600)
// ---------------------------------------------------------------------------
static void render_mandelbrot(const std::string& path) {
    constexpr int W = 800, H = 600, MAX_ITER = 256;
    Image img(W, H);
    img.fill(0, 0, 0);

    for (int py = 0; py < H; ++py) {
        for (int px = 0; px < W; ++px) {
            double x0 = (px - W * 0.68) / (W * 0.35);
            double y0 = (py - H * 0.5) / (H * 0.45);
            double x = 0, y = 0;
            int iter = 0;
            while (x * x + y * y <= 4.0 && iter < MAX_ITER) {
                double xt = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xt;
                ++iter;
            }
            if (iter < MAX_ITER) {
                // Smooth coloring
                double mu = iter + 1.0 - std::log2(std::log2(x * x + y * y));
                double hue = std::fmod(mu * 6.0, 360.0);
                double sat = 0.85;
                double val = iter < MAX_ITER ? 1.0 : 0.0;
                // Add brightness variation for depth
                val = 0.5 + 0.5 * std::sin(mu * 0.15);
                auto c = hsv_to_rgb(hue, sat, val);
                img.set(px, py, c.r, c.g, c.b);
            }
            // Interior stays black
        }
    }
    img.save_png(path);
    std::cout << "  Mandelbrot: " << path << "\n";
}

// ---------------------------------------------------------------------------
// 2. Barnsley Fern (600x800)
// ---------------------------------------------------------------------------
static void render_barnsley(const std::string& path) {
    constexpr int W = 600, H = 800;
    constexpr int POINTS = 200000;
    Image img(W, H);
    img.fill(5, 10, 5); // very dark green background

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double x = 0.0, y = 0.0;
    for (int i = 0; i < POINTS; ++i) {
        double r = dist(rng);
        double nx, ny;
        if (r < 0.01) {
            nx = 0.0;
            ny = 0.16 * y;
        } else if (r < 0.86) {
            nx = 0.85 * x + 0.04 * y;
            ny = -0.04 * x + 0.85 * y + 1.6;
        } else if (r < 0.93) {
            nx = 0.20 * x - 0.26 * y;
            ny = 0.23 * x + 0.22 * y + 1.6;
        } else {
            nx = -0.15 * x + 0.28 * y;
            ny = 0.26 * x + 0.24 * y + 0.44;
        }
        x = nx;
        y = ny;

        // Map to pixel coords
        int px = static_cast<int>((x + 2.5) / 6.0 * W);
        int py = static_cast<int>(H - (y / 10.5) * H);

        // Green gradient based on height
        double t = y / 10.0;
        uint8_t g = static_cast<uint8_t>(100 + 155 * t);
        uint8_t r2 = static_cast<uint8_t>(20 + 40 * (1.0 - t));
        img.set(px, py, r2, g, 30);
        // Sub-pixel neighbors for density
        img.set(px + 1, py, r2, g, 30);
        img.set(px, py + 1, r2, g, 30);
    }
    img.save_png(path);
    std::cout << "  Barnsley Fern: " << path << "\n";
}

// ---------------------------------------------------------------------------
// 3. Sierpinski Triangle (600x600) — chaos game
// ---------------------------------------------------------------------------
static void render_sierpinski(const std::string& path) {
    constexpr int W = 600, H = 600;
    constexpr int POINTS = 80000;
    Image img(W, H);
    img.fill(15, 15, 25); // dark navy

    // Triangle vertices
    double vx[3] = {300.0, 30.0, 570.0};
    double vy[3] = {30.0, 560.0, 560.0};

    std::mt19937 rng(123);
    std::uniform_int_distribution<int> vert(0, 2);

    double x = 300.0, y = 300.0;
    for (int i = 0; i < POINTS; ++i) {
        int v = vert(rng);
        x = (x + vx[v]) / 2.0;
        y = (y + vy[v]) / 2.0;

        // Color by which vertex was chosen
        RGB c;
        if (v == 0)      c = {255, 90, 90};   // red-ish
        else if (v == 1)  c = {90, 200, 255};  // blue-ish
        else              c = {90, 255, 140};  // green-ish
        int px = static_cast<int>(x);
        int py = static_cast<int>(y);
        img.set(px, py, c.r, c.g, c.b);
        img.set(px + 1, py, c.r, c.g, c.b);
    }
    img.save_png(path);
    std::cout << "  Sierpinski: " << path << "\n";
}

// ---------------------------------------------------------------------------
// 4. Sierpinski Carpet (600x600) — recursive subdivision
// ---------------------------------------------------------------------------
static void carpet_rec(Image& img, int x, int y, int size, int depth) {
    if (depth <= 0 || size < 3) return;
    int s3 = size / 3;
    // Punch out center
    double t = depth / 6.0;
    uint8_t r = static_cast<uint8_t>(20 + 60 * t);
    uint8_t g = static_cast<uint8_t>(10 + 30 * t);
    uint8_t b = static_cast<uint8_t>(40 + 80 * t);
    img.fill_rect(x + s3, y + s3, x + 2 * s3 - 1, y + 2 * s3 - 1, r, g, b);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (i != 1 || j != 1)
                carpet_rec(img, x + i * s3, y + j * s3, s3, depth - 1);
}

static void render_carpet(const std::string& path) {
    constexpr int W = 600, H = 600;
    Image img(W, H);
    // Fill with warm light color
    img.fill(220, 195, 170);
    carpet_rec(img, 0, 0, 600, 5);
    img.save_png(path);
    std::cout << "  Sierpinski Carpet: " << path << "\n";
}

// ---------------------------------------------------------------------------
// 5. Koch Snowflake (600x600) — recursive midpoint replacement
// ---------------------------------------------------------------------------
struct Point { double x, y; };

static void koch_curve(Image& img, Point a, Point b, int depth,
                       uint8_t r, uint8_t g, uint8_t bl) {
    if (depth == 0) {
        img.line(static_cast<int>(a.x), static_cast<int>(a.y),
                 static_cast<int>(b.x), static_cast<int>(b.y), r, g, bl);
        return;
    }
    Point p1 = {a.x + (b.x - a.x) / 3.0, a.y + (b.y - a.y) / 3.0};
    Point p2 = {a.x + 2.0 * (b.x - a.x) / 3.0, a.y + 2.0 * (b.y - a.y) / 3.0};
    double dx = b.x - a.x, dy = b.y - a.y;
    Point peak = {
        a.x + dx / 2.0 - dy * std::sqrt(3.0) / 6.0,
        a.y + dy / 2.0 + dx * std::sqrt(3.0) / 6.0
    };
    koch_curve(img, a, p1, depth - 1, r, g, bl);
    koch_curve(img, p1, peak, depth - 1, r, g, bl);
    koch_curve(img, peak, p2, depth - 1, r, g, bl);
    koch_curve(img, p2, b, depth - 1, r, g, bl);
}

static void render_koch(const std::string& path) {
    constexpr int W = 600, H = 600;
    Image img(W, H);
    img.fill(10, 15, 35); // dark blue

    double cx = 300, cy = 310;
    double radius = 250;
    Point p0 = {cx - radius, cy + radius * std::sqrt(3.0) / 3.0};
    Point p1 = {cx + radius, cy + radius * std::sqrt(3.0) / 3.0};
    Point p2 = {cx, cy - radius * 2.0 * std::sqrt(3.0) / 3.0};

    // 3 sides, each gets a distinct color tint
    koch_curve(img, p0, p1, 5, 100, 200, 255);
    koch_curve(img, p1, p2, 5, 100, 255, 200);
    koch_curve(img, p2, p0, 5, 200, 180, 255);
    img.save_png(path);
    std::cout << "  Koch Snowflake: " << path << "\n";
}

// ---------------------------------------------------------------------------
// 6. Dragon Curve (800x600) — iterative unfolding
// ---------------------------------------------------------------------------
static void render_dragon(const std::string& path) {
    constexpr int W = 800, H = 600;
    constexpr int DEPTH = 16;
    Image img(W, H);
    img.fill(15, 12, 25); // dark purple

    // Build turn sequence
    std::vector<int> turns; // 1 = right, 0 = left
    turns.reserve(1 << DEPTH);
    for (int i = 0; i < DEPTH; ++i) {
        auto old = turns;
        turns.push_back(1); // right turn
        for (int j = static_cast<int>(old.size()) - 1; j >= 0; --j)
            turns.push_back(1 - old[j]);
    }

    // Walk the curve
    int dx = 1, dy = 0;
    double x = 280, y = 380;
    double step = 2.5;
    int total = static_cast<int>(turns.size());

    for (int i = 0; i <= total; ++i) {
        double nx = x + dx * step;
        double ny = y + dy * step;

        double t = static_cast<double>(i) / total;
        auto c = hsv_to_rgb(t * 300.0 + 180.0, 0.8, 0.9);
        img.line(static_cast<int>(x), static_cast<int>(y),
                 static_cast<int>(nx), static_cast<int>(ny), c.r, c.g, c.b);

        x = nx;
        y = ny;
        if (i < total) {
            if (turns[i]) { // right turn
                int tmp = dx;
                dx = -dy;
                dy = tmp;
            } else { // left turn
                int tmp = dx;
                dx = dy;
                dy = -tmp;
            }
        }
    }
    img.save_png(path);
    std::cout << "  Dragon Curve: " << path << "\n";
}

// ---------------------------------------------------------------------------
// 7. Hilbert Curve (600x600) — order 6
// ---------------------------------------------------------------------------
static void hilbert_d2xy(int n, int d, int& x, int& y) {
    x = y = 0;
    for (int s = 1; s < n; s *= 2) {
        int rx = 1 & (d / 2);
        int ry = 1 & (d ^ rx);
        // rotate
        if (ry == 0) {
            if (rx == 1) { x = s - 1 - x; y = s - 1 - y; }
            std::swap(x, y);
        }
        x += s * rx;
        y += s * ry;
        d /= 4;
    }
}

static void render_hilbert(const std::string& path) {
    constexpr int W = 600, H = 600;
    constexpr int ORDER = 6;
    constexpr int N = 1 << ORDER; // 64
    constexpr int TOTAL = N * N;
    Image img(W, H);
    img.fill(240, 238, 235); // warm off-white

    double margin = 30.0;
    double cell = (W - 2 * margin) / (N - 1);

    int px = -1, py = -1;
    for (int d = 0; d < TOTAL; ++d) {
        int hx, hy;
        hilbert_d2xy(N, d, hx, hy);
        int sx = static_cast<int>(margin + hx * cell);
        int sy = static_cast<int>(margin + hy * cell);

        if (px >= 0) {
            double t = static_cast<double>(d) / TOTAL;
            auto c = hsv_to_rgb(t * 330.0, 0.75, 0.65);
            img.line(px, py, sx, sy, c.r, c.g, c.b);
        }
        px = sx;
        py = sy;
    }
    img.save_png(path);
    std::cout << "  Hilbert Curve: " << path << "\n";
}

// ---------------------------------------------------------------------------
// 8. Cantor Set (800x400) — recursive line removal
// ---------------------------------------------------------------------------
static void cantor_rec(Image& img, int x, int y, int len, int depth,
                       int max_depth, int bar_h) {
    if (depth > max_depth || len < 1) return;
    double t = static_cast<double>(depth) / max_depth;
    auto c = hsv_to_rgb(30.0 + t * 200.0, 0.7, 0.8 - t * 0.3);
    img.fill_rect(x, y, x + len - 1, y + bar_h - 1, c.r, c.g, c.b);
    int third = len / 3;
    int gap = 18;
    cantor_rec(img, x, y + bar_h + gap, third, depth + 1, max_depth, bar_h);
    cantor_rec(img, x + 2 * third, y + bar_h + gap, third, depth + 1,
               max_depth, bar_h);
}

static void render_cantor(const std::string& path) {
    constexpr int W = 800, H = 400;
    Image img(W, H);
    img.fill(250, 248, 245);
    cantor_rec(img, 20, 20, W - 40, 0, 7, 12);
    img.save_png(path);
    std::cout << "  Cantor Set: " << path << "\n";
}

// ---------------------------------------------------------------------------
// 9. Arborescent / Recursive Tree (600x800)
// ---------------------------------------------------------------------------
static void tree_branch(Image& img, double x, double y, double angle,
                        double len, int depth, int max_depth) {
    if (depth > max_depth || len < 2.0) return;

    double rad = angle * M_PI / 180.0;
    double nx = x + len * std::cos(rad);
    double ny = y + len * std::sin(rad);

    double t = static_cast<double>(depth) / max_depth;
    int thickness = std::max(1, static_cast<int>((1.0 - t) * 4));

    // Brown trunk -> green leaves
    uint8_t r, g, b;
    if (t < 0.4) {
        r = static_cast<uint8_t>(100 - t * 80);
        g = static_cast<uint8_t>(60 + t * 40);
        b = static_cast<uint8_t>(30);
    } else {
        r = static_cast<uint8_t>(30 + (1.0 - t) * 60);
        g = static_cast<uint8_t>(140 + t * 115);
        b = static_cast<uint8_t>(50 + (1.0 - t) * 30);
    }

    img.thick_line(static_cast<int>(x), static_cast<int>(y),
                   static_cast<int>(nx), static_cast<int>(ny),
                   thickness, r, g, b);

    double shrink = 0.72;
    double spread = 28.0;
    tree_branch(img, nx, ny, angle - spread, len * shrink, depth + 1, max_depth);
    tree_branch(img, nx, ny, angle + spread, len * shrink, depth + 1, max_depth);
    // Occasional third branch for density
    if (depth % 2 == 0 && depth < max_depth - 1) {
        tree_branch(img, nx, ny, angle - spread * 0.3, len * shrink * 0.8,
                    depth + 2, max_depth);
    }
}

static void render_tree(const std::string& path) {
    constexpr int W = 600, H = 800;
    Image img(W, H);
    // Sky gradient
    for (int y = 0; y < H; ++y) {
        double t = static_cast<double>(y) / H;
        uint8_t r = static_cast<uint8_t>(200 + 40 * t);
        uint8_t g = static_cast<uint8_t>(215 + 30 * t);
        uint8_t b = static_cast<uint8_t>(235 + 15 * t);
        for (int x = 0; x < W; ++x)
            img.set(x, y, r, g, b);
    }
    tree_branch(img, 300, 750, -90.0, 160.0, 0, 12);
    img.save_png(path);
    std::cout << "  Arborescent Tree: " << path << "\n";
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    std::string outdir = "docs/images";
    if (argc > 1) outdir = argv[1];

    std::cout << "Rendering fractal screenshots to " << outdir << "/\n";

    render_mandelbrot(outdir + "/mandelbrot.png");
    render_barnsley(outdir + "/barnsley.png");
    render_sierpinski(outdir + "/sierpinski.png");
    render_carpet(outdir + "/sierpinski-carpet.png");
    render_koch(outdir + "/koch.png");
    render_dragon(outdir + "/dragon.png");
    render_hilbert(outdir + "/hilbert.png");
    render_cantor(outdir + "/cantor.png");
    render_tree(outdir + "/arboresent.png");

    std::cout << "Done — 9 fractal PNGs generated.\n";
    return 0;
}
