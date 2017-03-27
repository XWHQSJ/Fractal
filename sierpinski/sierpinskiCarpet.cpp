#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

void sierpinskiCarpet(double x0, double y0, double x1, double y1, int k)
{
	double x2, y2, x3, y3, x4, y4, x5, y5, 
		x6, y6, x7, y7, x8, y8, x9, y9, x10, 
		y10, x11, y11, x12, y12, x13, y13;

	x2 = 2.0 / 3 * x0 + 1.0 / 3 * x1;
	y2 = y0;

	x3 = 1.0 / 3 * x0 + 2.0 / 3 * x1;
	y3 = y0;

	x4 = x0;
	y4 = 2.0 / 3 * y0 + 1.0 / 3 * y1;

	x5 = 2.0 / 3 * x0 + 1.0 / 3 * x1;
	y5 = 2.0 / 3 * y0 + 1.0 / 3 * y1;

	x6 = 1.0 / 3 * x0 + 2.0 / 3 * x1;
	y6 = 2.0 / 3 * y0 + 1.0 / 3 * y1;

	x7 = x1;
	y7 = 2.0 / 3 * y0 + 1.0 / 3 * y1;

	x8 = x0;
	y8 = 1.0 / 3 * y0 + 2.0 / 3 * y1;

	x9 = 2.0 / 3 * x0 + 1.0 / 3 * x1;
	y9 = 1.0 / 3 * y0 + 2.0 / 3 * y1;

	x10 = 1.0 / 3 * x0 + 2.0 / 3 * x1;
	y10 = 1.0 / 3 * y0 + 2.0 / 3 * y1;

	x11 = x1;
	y11 = 1.0 / 3 * y0 + 2.0 / 3 * y1;

	x12 = 2.0 / 3 * x0 + 1.0 / 3 * x1;
	y12 = y1;

	x13 = 1.0 / 3 * x0 + 2.0 / 3 * x1;
	y13 = y1;

	if (k > 1)
	{
		sierpinskiCarpet(x0, y0, x5, y5, k - 1);
		sierpinskiCarpet(x2, y2, x6, y6, k - 1);
		sierpinskiCarpet(x3, y3, x7, y7, k - 1);
		sierpinskiCarpet(x4, y4, x9, y9, k - 1);
		//sierpinskiCarpet(x5, y5, x10, y10, k - 1);
		sierpinskiCarpet(x6, y6, x11, y11, k - 1);
		sierpinskiCarpet(x8, y8, x12, y12, k - 1);
		sierpinskiCarpet(x9, y9, x13, y13, k - 1);
		sierpinskiCarpet(x10, y10, x1, y1, k - 1);
	}
	else
	{
		setlinecolor(GREEN);
		setfillcolor(RED);
		fillrectangle(x0, y0, x5, y5);
		setlinecolor(GREEN);
		setfillcolor(RED);
		fillrectangle(x2, y2, x6, y6);
		setlinecolor(GREEN);
		setfillcolor(RED);
		fillrectangle(x3, y3, x7, y7);
		setlinecolor(GREEN);
		setfillcolor(RED);
		fillrectangle(x4, y4, x9, y9);
		setlinecolor(GREEN);
		setfillcolor(BLACK);
		fillrectangle(x5, y5, x10, y10);
		setlinecolor(GREEN);
		setfillcolor(RED);
		fillrectangle(x6, y6, x11, y11);
		setlinecolor(GREEN);
		setfillcolor(RED);
		fillrectangle(x8, y8, x12, y12);
		setlinecolor(GREEN);
		setfillcolor(RED);
		fillrectangle(x9, y9, x13, y13);
		setlinecolor(GREEN);
		setfillcolor(RED);
		fillrectangle(x10, y10, x1, y1);
	}
}

int main()
{
	srand((unsigned)time(NULL));

	initgraph(640, 480);

	char s[10];
	InputBox((LPTSTR)s, 10, _T("请输入迭代的次数"));

	int n;
	sscanf(s, "%d", &n);

	sierpinskiCarpet(50.0, 50.0, 450.0, 450.0, n);

	getch();
	closegraph();

	return 0;
}