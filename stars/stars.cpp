// stars.cpp -- Starfield visual effect
#ifdef EASYX_COMPAT_USE_SDL
#include "../easyx_compat.h"
#else
#include <graphics.h>
#include <conio.h>
#endif
#include <time.h>

#define MAXSTARS 200

struct STARS
{
	double x;
	int y;
	double step;
	int color;
};

STARS stars[MAXSTARS];
int color = 0;

void InitStars(int i)
{
	stars[i].x = 0;
	stars[i].y = rand() % 480;
	stars[i].step = (rand() % 5000) / 1000.0 + 1;
	color = (int)(stars[i].step * 255 / 6.0 + 0.5);
	stars[i].color = RGB(rand() % color, rand() % color, rand() % color);
}

void MoveStars(int i)
{
	putpixel((int)stars[i].x, (int)stars[i].y, 0);
	stars[i].x += stars[i].step;
	if (stars[i].x > 640)
	{
		InitStars(i);
	}
	putpixel((int)stars[i].x, (int)stars[i].y, stars[i].color);
}

#ifdef __EMSCRIPTEN__
static void stars_frame() {
	for (int i = 0; i < MAXSTARS; i++) {
		MoveStars(i);
	}
}
#endif

int main()
{
	srand((unsigned)time(NULL));
	initgraph(640, 480);

	for (int i = 0; i < MAXSTARS; i++)
	{
		InitStars(i);
		stars[i].x = rand() % 640;
	}

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(stars_frame, 50, 1);
#else
	while (!kbhit())
	{
		for (int i = 0; i < MAXSTARS; i++)
		{
			MoveStars(i);
		}
		Sleep(20);
	}
#endif

	closegraph();
	return 0;
}
