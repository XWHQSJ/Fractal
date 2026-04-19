#ifdef EASYX_COMPAT_USE_SDL
#include "../easyx_compat.h"
#else
#include <graphics.h>
#include <conio.h>
#endif

int main()
{
	initgraph(640, 480);

	//�����ɫ���õ���ɫ����
	float H = 190;	//ɫ��
	float S = 1;	//���Ͷ�
	float L = 0.7f;	//����
	
	//������յ���ɫ
	for (int i=0;i<480;i++)
	{
		//������յ�����
		L += 0.0005f;
		setlinecolor(HSLtoRGB(H, S, L));
		line(0, i, 639, i);
	}

	//���òʺ�ĸ���ɫ����
	H = 0;
	S = 1;
	L = 0.5f;

	//���Ʋʺ�
	setlinestyle(PS_SOLID, 2);
	for (int r = 400; r > 344; r --)
	{
		//���Ӳʺ����ɫǿ��--��Ȼ���������
		H += 5.5;
		setlinecolor(HSLtoRGB(H, S, L));
		circle(500, 480, r);
	}

	//��������˳�
	getch();
	closegraph();

}



