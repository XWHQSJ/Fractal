#ifdef EASYX_COMPAT_USE_SDL
#include "../easyx_compat.h"
#else
#include <graphics.h>
#include <conio.h>
#endif
#include <time.h>

#define MAXSTARS 200	//���ǵ�����
 
//��������
struct STARS
{
	double x;		//���Ǻ�����
	int y;			//����������
	double step;	//���ǵ�λ��
	int color;		//���ǵ���ɫ
};

//��������
STARS stars[MAXSTARS];
int color = 0;

//��ʼ������
void InitStars(int i)
{
	stars[i].x = 0;
	stars[i].y = rand() % 480;
	stars[i].step = (rand() % 5000) / 1000.0 + 1;
	color = (int)(stars[i].step * 255 / 6.0 + 0.5);
	//stars[i].color = RGB(color, color, color);
	stars[i].color = RGB(rand() % color, rand() % color, rand() % color);
}

//�ƶ�����
void MoveStars(int i)
{
	//�����ϵ�����
	putpixel((int)stars[i].x, (int)stars[i].y, 0);

	//�������ǵ�λ��
	stars[i].x += stars[i].step;

	if (stars[i].x > 640)
	{
		InitStars(i);
	}

	//���µ�����
	putpixel((int)stars[i].x, (int)stars[i].y, stars[i].color);
}

int main()
{
	//���������
	srand((unsigned)time(NULL));

	initgraph(640, 480);

	for (int i = 0; i < MAXSTARS; i++)
	{
		InitStars(i);
		stars[i].x = rand() % 640;
	}

	//��������˳�
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