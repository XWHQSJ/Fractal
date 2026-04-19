// hilbert.cpp -- Hilbert space-filling curve
#include "../fractal_common.h"

static const int W = DEFAULT_WIDTH;
static const int H = DEFAULT_HEIGHT;

static int hx, hy;
static int colorIdx;
static int totalSegs;

static void hilbert_move(int dx, int dy, int step) {
    int nx = hx + dx * step;
    int ny = hy + dy * step;
    setlinecolor(palette_color(colorIdx++, totalSegs));
    line(hx, hy, nx, ny);
    hx = nx;
    hy = ny;
}

// Hilbert curve via L-system-like recursion
// A -> -BF+AFA+FB-
// B -> +AF-BFB-FA+
static void hilbert_a(int order, int step);
static void hilbert_b(int order, int step);

static int hdir = 0; // 0=right,1=down,2=left,3=up
static int hdx[] = { 1, 0, -1, 0 };
static int hdy[] = { 0, 1, 0, -1 };

static void turn_left() { hdir = (hdir + 3) % 4; }
static void turn_right() { hdir = (hdir + 1) % 4; }
static void forward(int step) { hilbert_move(hdx[hdir], hdy[hdir], step); }

void hilbert_a(int order, int step) {
    if (order <= 0) return;
    turn_left();
    hilbert_b(order - 1, step);
    forward(step);
    turn_right();
    hilbert_a(order - 1, step);
    forward(step);
    hilbert_a(order - 1, step);
    turn_right();
    forward(step);
    hilbert_b(order - 1, step);
    turn_left();
}

void hilbert_b(int order, int step) {
    if (order <= 0) return;
    turn_right();
    hilbert_a(order - 1, step);
    forward(step);
    turn_left();
    hilbert_b(order - 1, step);
    forward(step);
    hilbert_b(order - 1, step);
    turn_left();
    forward(step);
    hilbert_a(order - 1, step);
    turn_right();
}

int main() {
    fractal_init();
    initgraph(W, H);
    BeginBatchDraw();

    int order = 5;
    int cells = 1 << order; // 2^order
    int margin = 20;
    int side = (H - 2 * margin < W - 2 * margin) ? H - 2 * margin : W - 2 * margin;
    int step = side / (cells - 1);

    hx = margin;
    hy = margin;
    hdir = 0;
    colorIdx = 0;
    // Total segments = 4^order - 1
    totalSegs = 1;
    for (int i = 0; i < order; i++) totalSegs *= 4;

    hilbert_a(order, step);

    EndBatchDraw();
    getch();
    closegraph();
    return 0;
}
