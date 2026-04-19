// demo.cpp -- L-system demo: Koch, Dragon, Plant
#include "../fractal_common.h"
#include "lsystem.h"

static const int W = DEFAULT_WIDTH;
static const int H = DEFAULT_HEIGHT;

static void run_lsystem(const LSystem& ls, double sx, double sy, double sa,
                        const char* title) {
    (void)title;
    cleardevice();
    BeginBatchDraw();

    std::string expanded = ls.expand();
    int total = lsystem_count_segments(expanded);

    lsystem_render(
        expanded, sx, sy, sa, ls.stepLen, ls.angle,
        [](double x1, double y1, double x2, double y2) {
            line(x1, y1, x2, y2);
        },
        [&](int idx, int tot) {
            setlinecolor(palette_color(idx, tot > 0 ? tot : 1));
        },
        total
    );

    EndBatchDraw();
}

int main() {
    fractal_init();
    initgraph(W, H);

    // --- Koch snowflake ---
    LSystem koch;
    koch.axiom = "F--F--F";
    koch.rules['F'] = "F+F--F+F";
    koch.angle = 60.0;
    koch.stepLen = 2.0;
    koch.iterations = 4;
    run_lsystem(koch, 80, 350, 60.0, "Koch Snowflake");
    getch();

    // --- Dragon curve ---
    LSystem dragon;
    dragon.axiom = "FX";
    dragon.rules['X'] = "X+YF+";
    dragon.rules['Y'] = "-FX-Y";
    dragon.angle = 90.0;
    dragon.stepLen = 3.0;
    dragon.iterations = 12;
    run_lsystem(dragon, W / 3, H / 2, 0.0, "Dragon Curve");
    getch();

    // --- Plant ---
    LSystem plant;
    plant.axiom = "X";
    plant.rules['X'] = "F+[[X]-X]-F[-FX]+X";
    plant.rules['F'] = "FF";
    plant.angle = 25.0;
    plant.stepLen = 2.5;
    plant.iterations = 5;
    run_lsystem(plant, W / 2, H - 20, 90.0, "Plant");
    getch();

    closegraph();
    return 0;
}
