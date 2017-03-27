#include <graphics.h>
#include <conio.h>

void main()
{
	initgraph(640, 480);

	//天空颜色设置的颜色分量
	float H = 190;	//色相
	float S = 1;	//饱和度
	float L = 0.7f;	//亮度
	
	//绘制天空的颜色
	for (int i=0;i<480;i++)
	{
		//增加天空的亮度
		L += 0.0005f;
		setlinecolor(HSLtoRGB(H, S, L));
		line(0, i, 639, i);
	}

	//设置彩虹的各颜色分量
	H = 0;
	S = 1;
	L = 0.5f;

	//绘制彩虹
	setlinestyle(PS_SOLID, 2);
	for (int r = 400; r > 344; r --)
	{
		//增加彩虹的颜色强度--红橙黄绿蓝靛紫
		H += 5.5;
		setlinecolor(HSLtoRGB(H, S, L));
		circle(500, 480, r);
	}

	//按任意键退出
	getch();
	closegraph();

}



