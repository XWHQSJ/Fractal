// math_test.cpp -- Unit tests for fractal math functions
#include <gtest/gtest.h>
#include <cmath>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---- Koch midpoint calculation (from kochsnow) ----
struct KochPt { double x, y; };

static KochPt koch_midpoint(double x0, double y0, double x1, double y1) {
    // The equilateral triangle peak point
    double mx = 0.5 * (x0 + x1) + sqrt(3.0) / 6.0 * (y1 - y0);
    double my = 0.5 * (y0 + y1) - sqrt(3.0) / 6.0 * (x1 - x0);
    return {mx, my};
}

TEST(KochTest, MidpointHorizontalLine) {
    // For a horizontal line from (0,0) to (3,0):
    // Third points at (1,0) and (2,0), peak should be at (1.5, -sqrt(3)/2)
    KochPt p = koch_midpoint(0.0, 0.0, 3.0, 0.0);
    EXPECT_NEAR(p.x, 1.5, 1e-9);
    EXPECT_NEAR(p.y, -sqrt(3.0) / 2.0, 1e-9);
}

TEST(KochTest, MidpointVerticalLine) {
    KochPt p = koch_midpoint(0.0, 0.0, 0.0, 6.0);
    // Peak should be at (sqrt(3), 3)
    EXPECT_NEAR(p.x, sqrt(3.0), 1e-9);
    EXPECT_NEAR(p.y, 3.0, 1e-9);
}

// ---- Mandelbrot escape count ----
static int mandelbrot_escape(double cx, double cy, int max_iter) {
    double zx = 0.0, zy = 0.0;
    int i = 0;
    while (zx * zx + zy * zy <= 4.0 && i < max_iter) {
        double tmp = zx * zx - zy * zy + cx;
        zy = 2.0 * zx * zy + cy;
        zx = tmp;
        i++;
    }
    return i;
}

TEST(MandelbrotTest, OriginNeverEscapes) {
    // (0,0) is in the set -- should reach max iterations
    int n = mandelbrot_escape(0.0, 0.0, 256);
    EXPECT_EQ(n, 256);
}

TEST(MandelbrotTest, TwoZeroEscapesImmediately) {
    // (2,0) escapes on first iteration: z1 = 4+2i? No, z1 = 4+0i, |z1|=4>2
    // Actually: z0=0, z1=0^2+2=2, |z1|=2, z2=4+2=6, |z2|>2 -> escapes at i=2
    int n = mandelbrot_escape(2.0, 0.0, 256);
    EXPECT_EQ(n, 2);
}

TEST(MandelbrotTest, MinusOneInSet) {
    // c = -1 -> orbit: 0, -1, 0, -1, ... (period 2, never escapes)
    int n = mandelbrot_escape(-1.0, 0.0, 256);
    EXPECT_EQ(n, 256);
}

// ---- Sierpinski chaos game convergence ----
TEST(SierpinskiTest, ChaosGameConverges) {
    // After many iterations the chaos game should produce points
    // within the bounding triangle
    double Px[] = {320, 120, 520};
    double Py[] = {50, 400, 400};
    double x = 100.0, y = 200.0;

    srand(42);
    for (int i = 0; i < 10000; i++) {
        int n = rand() % 3;
        x = (x + Px[n]) / 2.0;
        y = (y + Py[n]) / 2.0;
    }

    // After convergence, point must be within triangle bounding box
    EXPECT_GE(x, 120.0);
    EXPECT_LE(x, 520.0);
    EXPECT_GE(y, 50.0);
    EXPECT_LE(y, 400.0);
}

// ---- L-system rule expansion ----
static std::string lsystem_expand(const std::string& axiom,
                                   const std::map<char, std::string>& rules,
                                   int iterations) {
    std::string current = axiom;
    for (int i = 0; i < iterations; i++) {
        std::string next;
        for (char c : current) {
            auto it = rules.find(c);
            if (it != rules.end())
                next += it->second;
            else
                next += c;
        }
        current = next;
    }
    return current;
}

TEST(LSystemTest, KochOneIteration) {
    std::map<char, std::string> rules;
    rules['F'] = "F+F--F+F";
    std::string result = lsystem_expand("F", rules, 1);
    EXPECT_EQ(result, "F+F--F+F");
}

TEST(LSystemTest, KochTwoIterations) {
    std::map<char, std::string> rules;
    rules['F'] = "F+F--F+F";
    std::string result = lsystem_expand("F", rules, 2);
    EXPECT_EQ(result, "F+F--F+F+F+F--F+F--F+F--F+F+F+F--F+F");
}

TEST(LSystemTest, DragonOneIteration) {
    std::map<char, std::string> rules;
    rules['X'] = "X+YF+";
    rules['Y'] = "-FX-Y";
    std::string result = lsystem_expand("FX", rules, 1);
    EXPECT_EQ(result, "FX+YF+");
}

TEST(LSystemTest, AxiomOnly) {
    std::map<char, std::string> rules;
    rules['A'] = "AB";
    std::string result = lsystem_expand("B", rules, 3);
    // B has no rule, stays B
    EXPECT_EQ(result, "B");
}
