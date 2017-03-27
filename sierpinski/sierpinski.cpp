//////////////////////////////////////////////////////////////////////////
//谢宾斯基三角形，也叫垫片绘制，算法比较简单
//0.随机生成 3 个点 P[0]、P[1]、P[2]；
//1.随机生成 1 个点 P；
//2.绘制点 P；
//3.随机生成[0, 2] 内的整数 n；
//4.令 P = P 与 P[n] 的中点；
//5.重复执行步骤(3)～(5) 三万次。
//////////////////////////////////////////////////////////////////////////

#include <graphics.h>
#include <conio.h>
#include <time.h>

int main()
{
	srand((unsigned)time(NULL));		//随机化种子

	//最初的三个点（可以随机生成也可自定义）
	POINT P[3] = { { 320, 50 },{ 120, 400 },{ 520, 400 } };
	//POINT P[3] = { { rand() % 640,rand() % 480 },{ rand() % 480,rand() % 480 },{ rand() % 480,rand() % 480 } };
	
	//随机的点
	POINT p = { rand()%640, rand()%480};

	initgraph(640, 480);

	int n;
	for (int i = 0; i < 30000; i++)
	{
		//取[0,3)中的整数表示数组中的某个元素
		n = rand() % 3;
		p.x = (p.x + P[n].x)/2;
		p.y = (p.y + P[n].y)/2;

		//绘制像素点或任意的小的形状
		//setlinecolor(RED);
		//line(p.x - 2, p.y, p.x + 2, p.y);
		//line(p.x, p.y - 2, p.x, p.y + 2);
		//line(p.x - 2, p.y - 2, p.x + 2, p.y + 2);
		//line(p.x - 2, p.y + 2, p.x + 2, p.y - 2);
		//putpixel(p.x, p.y, RGB(rand() % 255, rand() % 255, rand() % 255));
		putpixel(p.x, p.y, BLUE);
	}

	getch();
	closegraph();

	return 0;
}