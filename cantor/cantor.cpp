#ifdef EASYX_COMPAT_USE_SDL
#include "../easyx_compat.h"
#else
#include <graphics.h>
#include <conio.h>
#endif
#include <stdio.h>
#include <time.h>

void cantor(double x0, double y0, double x1, double y1, int k)
{
	//�����µĵ�
	double x2, y2, x3, y3;	
	x0 = x0;									//���˵�����ƽ��һ�ξ�����ڹ۲�
	y0 = y0 + 10;
	x1 = x1;
	y1 = y1 + 10;
	x2 = 2.0 / 3 * x0 + 1.0 / 3 * x1;			//�µĵ������ȡ���ڶ˵�����
	y2 = y0;
	x3 = 1.0 / 3 * x0 + 2.0 / 3 * x1;
	y3 = y0;

	//���������������0�ͻ��Ƶ�ǰ����
	if (k > 0)
	{
		setlinecolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		line(x0, y0, x2, y2);
		line(x3, y3, x1, y1);

		//��������������1ʱ���еݹ����
		cantor(x0, y0, x2, y2, k - 1);
		cantor(x3, y3, x1, y1, k - 1);
	}
}

int main()
{
	srand((unsigned)time(NULL));

	initgraph(640, 480);
	
	//����Ҫ�����Ĵ���
	char s[10];
	InputBox((LPTSTR)s, 10, _T("����������Ĵ���(<=9)"));

	int n;
	sscanf(s, "%d", &n);

	//����ԭʼ����
	line(10.0, 200.0, 600.0, 200.0);

	//�������ּ�����
	cantor(10.0,200.0,600.0,200.0,n);

	getch();
	closegraph();

	return 0;
}