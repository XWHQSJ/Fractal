/*
*ȡ��Milo Yip��֪��ר���������C���Ի���Щͼ��
*��Ҫ˼����ͨ�û���ͼ��
*��������easyX�Ļ�������
*��Ȼ��Ҫ�Ľ�������˼��
*/
#include <stdio.h>
#include <stdlib.h>
#ifdef EASYX_COMPAT_USE_SDL
#include "../easyx_compat.h"
#else
#include <graphics.h>
#include <conio.h>
#endif
#include <time.h>

//�����εĵױ߿��͸�
const int w = 12*2 - 1;
const int h = 12;

//����������
int triangle(int x, int y)
{
	return abs(x) <= y;
}

//��ͼ����Ҫ���Ƶ�ÿ��������ж��Ƿ���Ҫ����
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

	//�����л���
	int x, y;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			//��ӡ*��
			printf(f(x, y) ? "* " : "  ");

			if (f(x, y) != 0)
			{
				//���Ƶ�
				putpixel(x, y, RGB(rand() % 255, rand() % 255, rand() % 255));
			}
		}
		//C�����еĻ���
		puts("");
	}

	getch();
	closegraph();

	system("pause");
	return 0;
}