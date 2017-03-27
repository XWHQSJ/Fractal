#include <graphics.h>
#include <conio.h>
#include <time.h>

#define MAXSTARS 200	//星星的总数
 
//星星属性
struct STARS
{
	double x;		//星星横坐标
	int y;			//星星纵坐标
	double step;	//星星的位移
	int color;		//星星的颜色
};

//星星数组
STARS stars[MAXSTARS];
int color = 0;

//初始化星星
void InitStars(int i)
{
	stars[i].x = 0;
	stars[i].y = rand() % 480;
	stars[i].step = (rand() % 5000) / 1000.0 + 1;
	color = (int)(stars[i].step * 255 / 6.0 + 0.5);
	//stars[i].color = RGB(color, color, color);
	stars[i].color = RGB(rand() % color, rand() % color, rand() % color);
}

//移动星星
void MoveStars(int i)
{
	//擦除老的星星
	putpixel((int)stars[i].x, (int)stars[i].y, 0);

	//更新星星的位置
	stars[i].x += stars[i].step;

	if (stars[i].x > 640)
	{
		InitStars(i);
	}

	//画新的星星
	putpixel((int)stars[i].x, (int)stars[i].y, stars[i].color);
}

void main()
{
	//随机化种子
	srand((unsigned)time(NULL));

	initgraph(640, 480);

	for (int i = 0; i < MAXSTARS; i++)
	{
		InitStars(i);
		stars[i].x = rand() % 640;
	}

	//按任意键退出
	while (!kbhit())
	{
		for (int i = 0; i < MAXSTARS; i++)
		{
			MoveStars(i);
		}
		Sleep(20);
	}

	closegraph();

}