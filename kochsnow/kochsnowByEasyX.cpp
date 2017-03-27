//////////////////////////////////////////////////////////////////////////
/*使用EasyX绘制科赫雪花
*刻画曲线算法是核心
*/
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <time.h>

//科赫曲线算法
void koch(double x0, double y0, double x1, double y1, int k)
{
	//科赫曲线的5个顶点
	double x2, y2, x3, y3, x4, y4;
	x2 = 2.0 / 3 * x0 + 1.0 / 3 * x1;
	y2 = 2.0 / 3 * y0 + 1.0 / 3 * y1;
	x3 = 1.0 / 3 * x0 + 2.0 / 3 * x1;
	y3 = 1.0 / 3 * y0 + 2.0 / 3 * y1;
	x4 = 1.0 / 2 * (x0 + x1) + sqrt(3.0) / 6 * (y1 - y0);	//数学公式推导
	y4 = 1.0 / 2 * (y0 + y1) - sqrt(3.0) / 6 * (x1 - x0);	//数学公式推导

	//如果迭代次数超过1，进行递归操作
	if (k > 1)
	{
		koch(x0, y0, x2, y2, k - 1);
		koch(x2, y2, x4, y4, k - 1);
		koch(x4, y4, x3, y3, k - 1);
		koch(x3, y3, x1, y1, k - 1);
	}
	//如果迭代次数为一，直接绘制科赫曲线
	else
	{
		//将绘制的每条曲线的颜色设置为随机颜色
		setlinecolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		line(x0, y0, x2, y2);
		line(x2, y2, x4, y4);
		line(x4, y4, x3, y3);
		line(x3, y3, x1, y1);
	}
}

//将科赫曲线封装为科赫雪花
void kochsnow(int n)
{
	koch(50, 120, 450, 120, n);
	koch(450, 120, 250, 120 + 200 * sqrt(3.0), n);
	koch(250, 120 + 200 * sqrt(3.0), 50, 120, n);
}

int main()
{
	srand((unsigned)time(NULL));	//随机化种子

	initgraph(640, 480);			//窗口初始化

	//设置迭代的次数
	int n;
	char s[10];
	InputBox((LPTSTR)s, 10, _T("请输入迭代的次数(<=9)"));
	sscanf(s, "%d", &n);

	//调用科赫雪花绘制
	kochsnow(n);

	getch();
	closegraph();

	return 0;
}