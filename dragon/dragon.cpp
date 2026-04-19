// dragon.cpp -- Dragon curve fractal via recursive fold
#include "../fractal_common.h"
#include <vector>

static const int W = DEFAULT_WIDTH;
static const int H = DEFAULT_HEIGHT;

// Build direction sequence: at each step insert alternating R/L between existing
static void build_turns(std::vector<int>& turns, int order) {
    turns.clear();
    turns.push_back(1); // R
    for (int i = 1; i < order; i++) {
        std::vector<int> next;
        next.reserve(turns.size() * 2 + 1);
        for (size_t j = 0; j < turns.size(); j++)
            next.push_back(turns[j]);
        next.push_back(1); // R
        for (int j = (int)turns.size() - 1; j >= 0; j--)
            next.push_back(-turns[j]);
        turns = next;
    }
}

int main() {
    fractal_init();
    initgraph(W, H);
    BeginBatchDraw();

    int order = 14;
    std::vector<int> turns;
    build_turns(turns, order);

    // Direction: 0=right, 1=up, 2=left, 3=down
    int dx[] = { 1, 0, -1, 0 };
    int dy[] = { 0, -1, 0, 1 };

    // Scale: total segments = 2^order - 1 + 1
    int step = 2;
    int cx = W / 4;
    int cy = H / 2;
    int dir = 0;

    int x = cx, y = cy;
    for (size_t i = 0; i < turns.size(); i++) {
        int nx = x + dx[dir] * step;
        int ny = y + dy[dir] * step;
        setlinecolor(palette_color((int)i, (int)turns.size()));
        line(x, y, nx, ny);
        x = nx;
        y = ny;
        dir = (dir + turns[i] + 4) % 4;
    }
    // Final segment
    int nx = x + dx[dir] * step;
    int ny = y + dy[dir] * step;
    setlinecolor(palette_color((int)turns.size(), (int)turns.size()));
    line(x, y, nx, ny);

    EndBatchDraw();
    getch();
    closegraph();
    return 0;
}
