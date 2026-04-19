// lsystem.h -- L-system interpreter with turtle graphics
#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <string>
#include <map>
#include <vector>
#include <stack>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct LSystem {
    std::string axiom;
    std::map<char, std::string> rules;
    double angle;     // turn angle in degrees
    double stepLen;   // forward step length in pixels
    int iterations;

    std::string expand() const {
        std::string current = axiom;
        for (int i = 0; i < iterations; i++) {
            std::string next;
            next.reserve(current.size() * 4);
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
};

struct TurtleState {
    double x, y, angle;
};

// Render an expanded L-system string using line() and setlinecolor()
// Recognized symbols: F/G = forward+draw, f = forward no draw,
// + = turn right, - = turn left, [ = push, ] = pop
template <typename DrawLineFn, typename SetColorFn>
void lsystem_render(
    const std::string& instructions,
    double startX, double startY, double startAngle,
    double stepLen, double turnAngle,
    DrawLineFn drawLine, SetColorFn setColor,
    int totalLen)
{
    double x = startX, y = startY, a = startAngle;
    std::stack<TurtleState> stk;
    int idx = 0;

    for (char c : instructions) {
        switch (c) {
        case 'F':
        case 'G': {
            double nx = x + stepLen * cos(a * M_PI / 180.0);
            double ny = y - stepLen * sin(a * M_PI / 180.0);
            setColor(idx, totalLen);
            drawLine(x, y, nx, ny);
            x = nx; y = ny;
            idx++;
            break;
        }
        case 'f': {
            x += stepLen * cos(a * M_PI / 180.0);
            y -= stepLen * sin(a * M_PI / 180.0);
            break;
        }
        case '+': a -= turnAngle; break;
        case '-': a += turnAngle; break;
        case '[': stk.push({x, y, a}); break;
        case ']':
            if (!stk.empty()) {
                auto s = stk.top(); stk.pop();
                x = s.x; y = s.y; a = s.angle;
            }
            break;
        default: break;
        }
    }
}

// Count drawable segments in expanded string
static inline int lsystem_count_segments(const std::string& s) {
    int n = 0;
    for (char c : s)
        if (c == 'F' || c == 'G') n++;
    return n;
}

#endif // LSYSTEM_H
