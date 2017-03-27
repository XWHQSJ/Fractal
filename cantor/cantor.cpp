#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

void cantor(double x0, double y0, double x1, double y1, int k)
{
	//生成新的点
	double x2, y2, x3, y3;	
	x0 = x0;									//将端点向下平移一段距离便于观察
	y0 = y0 + 10;
	x1 = x1;
	y1 = y1 + 10;
	x2 = 2.0 / 3 * x0 + 1.0 / 3 * x1;			//新的点的坐标取决于端点坐标
	y2 = y0;
	x3 = 1.0 / 3 * x0 + 2.0 / 3 * x1;
	y3 = y0;

	//如果迭代次数不是0就绘制当前曲线
	if (k > 0)
	{
		setlinecolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		line(x0, y0, x2, y2);
		line(x3, y3, x1, y1);

		//当迭代次数大于1时进行递归迭代
		cantor(x0, y0, x2, y2, k - 1);
		cantor(x3, y3, x1, y1, k - 1);
	}
}

int main()
{
	srand((unsigned)time(NULL));

	initgraph(640, 480);
	
	//输入要迭代的次数
	char s[10];
	InputBox((LPTSTR)s, 10, _T("请输入迭代的次数(<=9)"));

	int n;
	sscanf(s, "%d", &n);

	//绘制原始曲线
	line(10.0, 200.0, 600.0, 200.0);

	//绘制三分集曲线
	cantor(10.0,200.0,600.0,200.0,n);

	getch();
	closegraph();

	return 0;
}