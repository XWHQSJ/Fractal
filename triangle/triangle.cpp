/*
*取自Milo Yip的知乎专栏“如何用C语言画这些图”
*主要思想是通用绘制图形
*本人添加easyX的绘制内容
*仍然需要改进与深入思考
*/
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>

//三角形的底边宽和高
const int w = 12*2 - 1;
const int h = 12;

//定义三角形
int triangle(int x, int y)
{
	return abs(x) <= y;
}

//对图形需要绘制的每个点进行判断是否需要绘制
int f(int x, int y)
{
	int m, n,h;
	m = (x + y / 3 * 3 + 3) % 6 - 2;
	n = y % 3;
	h = x - 11;

	return (triangle(m,n) && triangle(h,y));
}

int main()
{
	initgraph(640, 480);

	srand((unsigned)time(NULL));

	//逐点进行绘制
	int x, y;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			//打印*号
			printf(f(x, y) ? "* " : "  ");

			if (f(x, y) != 0)
			{
				//绘制点
				putpixel(x, y, RGB(rand() % 255, rand() % 255, rand() % 255));
			}
		}
		//C语言中的换行
		puts("");
	}

	getch();
	closegraph();

	system("pause");
	return 0;
}