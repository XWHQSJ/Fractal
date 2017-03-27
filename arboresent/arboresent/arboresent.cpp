#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

void arboresent(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
{

}

int main()
{
	srand((unsigned)time(NULL));

	initgraph(640, 480);

	setlinecolor(GREEN);
	setfillcolor(RED);
	POINT pts1[] = { {300,450},{80,250},{250,50} };
	fillpolygon(pts1, 3);

	setlinecolor(GREEN);
	setfillcolor(RED);
	POINT pts2[] = { {300,450},{520,250},{350,50} };
	fillpolygon(pts2, 3);

	getch();
	closegraph();

	return 0;
}