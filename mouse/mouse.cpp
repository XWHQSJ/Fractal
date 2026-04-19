/*������������Ȥ������*/
#ifdef EASYX_COMPAT_USE_SDL
#include "../easyx_compat.h"
#else
#include <graphics.h>
#include <conio.h>
#endif

int main()
{
	initgraph(640, 480);

	MOUSEMSG msg;

	while (true)
	{
		//��ȡ�����Ϣ
		msg = GetMouseMsg();

		//��������ϢuMsg
		switch (msg.uMsg)
		{
		case WM_MOUSEMOVE:
			putpixel(msg.x, msg.y, BLUE);
			break;
		case WM_LBUTTONDOWN:
			if (msg.mkCtrl)
			{
				setlinecolor(GREEN);
				setlinestyle(PS_SOLID);
				rectangle(msg.x - 10, msg.y - 10, msg.x + 10, msg.y + 10);
			}
			else
			{
				setlinecolor(RED);
				setlinestyle(PS_DASH);
				rectangle(msg.x - 5, msg.y - 5, msg.x + 5, msg.y + 5);
			}
			break;
		case WM_RBUTTONDOWN:
			cleardevice();
			break;
		default:
			break;
		}
	}

	getch();
	closegraph();

	return 0;
}