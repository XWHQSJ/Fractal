// easyx_compat.h -- SDL2-based cross-platform shim for EasyX graphics API
// Define EASYX_COMPAT_USE_SDL to force SDL2 backend (default on non-Windows).
// On Windows with real EasyX, this header is a no-op unless the define is set.
#ifndef EASYX_COMPAT_H
#define EASYX_COMPAT_H

#if defined(_WIN32) && !defined(EASYX_COMPAT_USE_SDL)
// Native EasyX -- just include the real header
#include <graphics.h>
#include <conio.h>
#else

// ---------- SDL2 backend ----------
#include <SDL.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>

// ---- Color ----
typedef uint32_t COLORREF;

#ifndef RGB
#define RGB(r, g, b) \
    ((COLORREF)(((uint8_t)(r)) | ((uint16_t)((uint8_t)(g)) << 8) | ((uint32_t)((uint8_t)(b)) << 16)))
#endif

static inline uint8_t GetRValue(COLORREF c) { return (uint8_t)(c); }
static inline uint8_t GetGValue(COLORREF c) { return (uint8_t)(c >> 8); }
static inline uint8_t GetBValue(COLORREF c) { return (uint8_t)(c >> 16); }

// Named colors
#define BLACK   RGB(0, 0, 0)
#define WHITE   RGB(255, 255, 255)
#define RED     RGB(255, 0, 0)
#define GREEN   RGB(0, 255, 0)
#define BLUE    RGB(0, 0, 255)
#define YELLOW  RGB(255, 255, 0)
#define CYAN    RGB(0, 255, 255)
#define MAGENTA RGB(255, 0, 255)
#define BROWN   RGB(139, 69, 19)

// ---- HSLtoRGB ----
static inline COLORREF HSLtoRGB(float H, float S, float L) {
    float C = (1.0f - fabsf(2.0f * L - 1.0f)) * S;
    float Hp = H / 60.0f;
    float X = C * (1.0f - fabsf(fmodf(Hp, 2.0f) - 1.0f));
    float r1 = 0, g1 = 0, b1 = 0;
    if (Hp < 1)      { r1 = C; g1 = X; }
    else if (Hp < 2) { r1 = X; g1 = C; }
    else if (Hp < 3) { g1 = C; b1 = X; }
    else if (Hp < 4) { g1 = X; b1 = C; }
    else if (Hp < 5) { r1 = X; b1 = C; }
    else              { r1 = C; b1 = X; }
    float m = L - C / 2.0f;
    return RGB((uint8_t)((r1 + m) * 255), (uint8_t)((g1 + m) * 255), (uint8_t)((b1 + m) * 255));
}

// ---- POINT / MOUSEMSG ----
#ifndef _WINDEF_
struct POINT {
    long x;
    long y;
};
#endif

#define WM_MOUSEMOVE    0x0200
#define WM_LBUTTONDOWN  0x0201
#define WM_RBUTTONDOWN  0x0204

struct MOUSEMSG {
    unsigned int uMsg;
    int x;
    int y;
    bool mkCtrl;
    bool mkShift;
};

// ---- Line styles (stubs) ----
#define PS_SOLID 0
#define PS_DASH  1

// ---- Internal singleton state ----
namespace easyx_internal {
    struct State {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        int width = 640;
        int height = 480;
        COLORREF lineColor = WHITE;
        COLORREF fillColor = WHITE;
        COLORREF bkColor = BLACK;
        bool batchMode = false;
        bool initialized = false;
    };
    inline State& state() { static State s; return s; }

    inline void applyColor(COLORREF c) {
        SDL_SetRenderDrawColor(state().renderer, GetRValue(c), GetGValue(c), GetBValue(c), 255);
    }
}

// ---- Graph lifecycle ----
static inline void initgraph(int w, int h) {
    auto& s = easyx_internal::state();
    if (!s.initialized) {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        s.initialized = true;
    }
    s.width = w;
    s.height = h;
    s.window = SDL_CreateWindow("Fractal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    s.renderer = SDL_CreateRenderer(s.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(s.renderer, 0, 0, 0, 255);
    SDL_RenderClear(s.renderer);
    SDL_RenderPresent(s.renderer);
}

static inline void closegraph() {
    auto& s = easyx_internal::state();
    if (s.renderer) { SDL_DestroyRenderer(s.renderer); s.renderer = nullptr; }
    if (s.window) { SDL_DestroyWindow(s.window); s.window = nullptr; }
    SDL_Quit();
    s.initialized = false;
}

// ---- Color setters ----
static inline void setlinecolor(COLORREF c) { easyx_internal::state().lineColor = c; }
static inline void setfillcolor(COLORREF c) { easyx_internal::state().fillColor = c; }
static inline void setcolor(COLORREF c) { setlinecolor(c); }
static inline void setbkcolor(COLORREF c) {
    easyx_internal::state().bkColor = c;
}

static inline void cleardevice() {
    auto& s = easyx_internal::state();
    easyx_internal::applyColor(s.bkColor);
    SDL_RenderClear(s.renderer);
    if (!s.batchMode) SDL_RenderPresent(s.renderer);
}

// ---- Line style (stub) ----
static inline void setlinestyle(int /*style*/, int /*width*/ = 1) {}

// ---- Drawing primitives ----
static inline void line(int x1, int y1, int x2, int y2) {
    auto& s = easyx_internal::state();
    easyx_internal::applyColor(s.lineColor);
    SDL_RenderDrawLine(s.renderer, x1, y1, x2, y2);
    if (!s.batchMode) SDL_RenderPresent(s.renderer);
}
// overload for double args used in existing code
static inline void line(double x1, double y1, double x2, double y2) {
    line((int)x1, (int)y1, (int)x2, (int)y2);
}

static inline void putpixel(int x, int y, COLORREF c) {
    easyx_internal::applyColor(c);
    SDL_RenderDrawPoint(easyx_internal::state().renderer, x, y);
    if (!easyx_internal::state().batchMode) SDL_RenderPresent(easyx_internal::state().renderer);
}

static inline COLORREF getpixel(int /*x*/, int /*y*/) {
    // SDL2 read-back is expensive; return black as stub
    return BLACK;
}

// Bresenham circle outline
static inline void circle(int cx, int cy, int r) {
    auto& s = easyx_internal::state();
    easyx_internal::applyColor(s.lineColor);
    int x = 0, y = r, d = 3 - 2 * r;
    while (x <= y) {
        SDL_RenderDrawPoint(s.renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(s.renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(s.renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(s.renderer, cx - x, cy - y);
        SDL_RenderDrawPoint(s.renderer, cx + y, cy + x);
        SDL_RenderDrawPoint(s.renderer, cx - y, cy + x);
        SDL_RenderDrawPoint(s.renderer, cx + y, cy - x);
        SDL_RenderDrawPoint(s.renderer, cx - y, cy - x);
        if (d < 0) d += 4 * x + 6;
        else { d += 4 * (x - y) + 10; y--; }
        x++;
    }
    if (!s.batchMode) SDL_RenderPresent(s.renderer);
}

static inline void fillcircle(int cx, int cy, int r) {
    auto& s = easyx_internal::state();
    easyx_internal::applyColor(s.fillColor);
    for (int dy = -r; dy <= r; dy++) {
        int dx = (int)sqrt((double)(r * r - dy * dy));
        SDL_RenderDrawLine(s.renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
    if (!s.batchMode) SDL_RenderPresent(s.renderer);
}

static inline void rectangle(int x1, int y1, int x2, int y2) {
    auto& s = easyx_internal::state();
    easyx_internal::applyColor(s.lineColor);
    SDL_Rect r = { std::min(x1, x2), std::min(y1, y2), abs(x2 - x1), abs(y2 - y1) };
    SDL_RenderDrawRect(s.renderer, &r);
    if (!s.batchMode) SDL_RenderPresent(s.renderer);
}
static inline void rectangle(double x1, double y1, double x2, double y2) {
    rectangle((int)x1, (int)y1, (int)x2, (int)y2);
}

static inline void fillrectangle(int x1, int y1, int x2, int y2) {
    auto& s = easyx_internal::state();
    // Draw filled area
    easyx_internal::applyColor(s.fillColor);
    SDL_Rect r = { std::min(x1, x2), std::min(y1, y2), abs(x2 - x1), abs(y2 - y1) };
    SDL_RenderFillRect(s.renderer, &r);
    // Draw outline
    easyx_internal::applyColor(s.lineColor);
    SDL_RenderDrawRect(s.renderer, &r);
    if (!s.batchMode) SDL_RenderPresent(s.renderer);
}
static inline void fillrectangle(double x1, double y1, double x2, double y2) {
    fillrectangle((int)x1, (int)y1, (int)x2, (int)y2);
}

static inline void fillpolygon(const POINT* pts, int n) {
    auto& s = easyx_internal::state();
    if (n < 3) return;
    // Scanline fill: find bounding box then fill horizontal spans
    int minY = pts[0].y, maxY = pts[0].y;
    int minX = pts[0].x, maxX = pts[0].x;
    for (int i = 1; i < n; i++) {
        if (pts[i].y < minY) minY = pts[i].y;
        if (pts[i].y > maxY) maxY = pts[i].y;
        if (pts[i].x < minX) minX = pts[i].x;
        if (pts[i].x > maxX) maxX = pts[i].x;
    }
    easyx_internal::applyColor(s.fillColor);
    for (int y = minY; y <= maxY; y++) {
        int nodeX[64];
        int nodes = 0;
        int j = n - 1;
        for (int i = 0; i < n; i++) {
            if ((pts[i].y <= y && pts[j].y > y) || (pts[j].y <= y && pts[i].y > y)) {
                nodeX[nodes++] = (int)(pts[i].x + (double)(y - pts[i].y) /
                    (pts[j].y - pts[i].y) * (pts[j].x - pts[i].x));
            }
            j = i;
        }
        // Sort
        for (int i = 0; i < nodes - 1; i++)
            for (int k = i + 1; k < nodes; k++)
                if (nodeX[i] > nodeX[k]) { int t = nodeX[i]; nodeX[i] = nodeX[k]; nodeX[k] = t; }
        for (int i = 0; i < nodes; i += 2) {
            if (i + 1 < nodes)
                SDL_RenderDrawLine(s.renderer, nodeX[i], y, nodeX[i + 1], y);
        }
    }
    // Outline
    easyx_internal::applyColor(s.lineColor);
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        SDL_RenderDrawLine(s.renderer, pts[i].x, pts[i].y, pts[j].x, pts[j].y);
    }
    if (!s.batchMode) SDL_RenderPresent(s.renderer);
}

// ---- Text output (minimal) ----
static inline void outtextxy(int x, int y, const char* text) {
    // SDL2 without SDL_ttf: skip text rendering silently.
    (void)x; (void)y; (void)text;
}

// ---- InputBox fallback (stdin) ----
#ifdef _T
#undef _T
#endif
#define _T(x) x
typedef char* LPTSTR;

static inline bool InputBox(char* buf, int maxLen, const char* prompt) {
    printf("%s: ", prompt);
    if (fgets(buf, maxLen, stdin)) {
        char* nl = strchr(buf, '\n');
        if (nl) *nl = '\0';
        return true;
    }
    return false;
}

// ---- Keyboard ----
static inline int getch() {
    SDL_Event e;
    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) return e.key.keysym.sym;
            if (e.type == SDL_QUIT) { closegraph(); exit(0); }
        }
        SDL_Delay(10);
    }
}

static inline int kbhit() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
            SDL_PushEvent(&e);
            return 1;
        }
        if (e.type == SDL_QUIT) { closegraph(); exit(0); }
    }
    return 0;
}

// ---- Mouse ----
static inline MOUSEMSG GetMouseMsg() {
    SDL_Event e;
    MOUSEMSG msg = {};
    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEMOTION) {
                msg.uMsg = WM_MOUSEMOVE;
                msg.x = e.motion.x; msg.y = e.motion.y;
                msg.mkCtrl = (SDL_GetModState() & KMOD_CTRL) != 0;
                return msg;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                msg.x = e.button.x; msg.y = e.button.y;
                msg.mkCtrl = (SDL_GetModState() & KMOD_CTRL) != 0;
                msg.uMsg = (e.button.button == SDL_BUTTON_LEFT) ? WM_LBUTTONDOWN : WM_RBUTTONDOWN;
                return msg;
            }
            if (e.type == SDL_QUIT) { closegraph(); exit(0); }
        }
        SDL_Delay(5);
    }
}

// ---- Timing ----
#ifndef _WIN32
static inline void Sleep(unsigned int ms) { SDL_Delay(ms); }
#endif

// ---- Batch drawing ----
static inline void BeginBatchDraw() { easyx_internal::state().batchMode = true; }
static inline void EndBatchDraw() {
    auto& s = easyx_internal::state();
    s.batchMode = false;
    SDL_RenderPresent(s.renderer);
}
static inline void FlushBatchDraw() {
    SDL_RenderPresent(easyx_internal::state().renderer);
}

#endif // SDL2 backend
#endif // EASYX_COMPAT_H
