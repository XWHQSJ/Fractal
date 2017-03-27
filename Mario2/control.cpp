#include "control.h"
#include"graphics.h"
#include"conio.h"
#include"mydefine.h"

control::control(void)
{
	key=0;
}
control::~control(void)
{
}
int control::getKey()
{
	if(_kbhit())
		key = GetCommand();
	if(key & CMD_ESC)//如果按了暂停键
		pauseClick();
	return key;
}
int control::GetCommand()
{
	int c = 0;

	if (GetAsyncKeyState('A') & 0x8000)
		c |= CMD_LEFT;
	if (GetAsyncKeyState('D') & 0x8000)
    	c |= CMD_RIGHT;
	if ((GetAsyncKeyState('W') & 0x8000)||(GetAsyncKeyState('K') & 0x8000))
    	c |= CMD_UP;
	if (GetAsyncKeyState('S') & 0x8000)
    	c |= CMD_DOWN;
	if (GetAsyncKeyState('J') & 0x8000)
    	c |= CMD_SHOOT;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    	c |= CMD_ESC;
	return c;
}
void control::pauseClick()
{
	BeginBatchDraw();
	POINT points[8]={{XSIZE/2-45,YSIZE/3}, {XSIZE/2+45,YSIZE/3}, {XSIZE/2+45,YSIZE/3+90}, {XSIZE/2-45,YSIZE/3+90}};
	setfillcolor(GREEN);
	fillpolygon(points, 4);
	setbkmode(TRANSPARENT);
	setfont(20,0,"黑体");
	RECT r2={XSIZE/2-45,YSIZE/3,XSIZE/2+45,YSIZE/3+30};rectangle(XSIZE/2-45,YSIZE/3,XSIZE/2+45,YSIZE/3+30);
	drawtext("回到游戏", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r3={XSIZE/2-45,YSIZE/3+30,XSIZE/2+45,YSIZE/3+60};rectangle(XSIZE/2-45,YSIZE/3+30,XSIZE/2+45,YSIZE/3+60);
    drawtext("重新开始", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    RECT r4={XSIZE/2-45,YSIZE/3+60,XSIZE/2+45,YSIZE/3+90};rectangle(XSIZE/2-45,YSIZE/3+60,XSIZE/2+45,YSIZE/3+90);
	drawtext(" 主 菜 单 ", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	FlushBatchDraw();

	FlushMouseMsgBuffer();//清空鼠标消息队列，否则会出错
	while(true)
	{
		BeginBatchDraw();
		MOUSEMSG m=GetMouseMsg();
		switch(m.uMsg)
        {
		case WM_LBUTTONDOWN:
			EndBatchDraw();
			if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3&&m.y<YSIZE/3+30)//如果选择“回到游戏”，则返回VIR_RETURN
			{
	     	    key=VIR_RETURN;
				return;
			}
			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+30&&m.y<YSIZE/3+60)//如果选择“重新开始”，则返回VIR_RESTART
			{
				key=VIR_RESTART;
				return;
 			}
			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+60&&m.y<YSIZE/3+90)//如果选择“主菜单”，则返回VIR_HOME
			{
				key=VIR_HOME;
				return;
			}
		case WM_MOUSEMOVE:
			RECT r;
			int i;
			for(i=0;i<3;i++)
			{
			    if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+i*30&&m.y<YSIZE/3+30+i*30)
			    {
				    r.left=XSIZE/2-45;
				    r.top=YSIZE/3+i*30;
				    r.right=XSIZE/2+45;
				    r.bottom=YSIZE/3+30+i*30;
					POINT points[8]={{r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom}};
				    setfillcolor(RED);
					fillpolygon(points, 4);
					setbkmode(TRANSPARENT);
					switch(i)
					{
					case 0:
					    drawtext("回到游戏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					    break;
					case 1:
					    drawtext("重新开始", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						break;
					case 2:
					    drawtext(" 主 菜 单 ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						break;
					}
				}
			    else
			    {
					if(getpixel(XSIZE/2-45+1,YSIZE/3+i*30+1)==RED)
					{
			       	    r.left=XSIZE/2-45;
			            r.top=YSIZE/3+i*30;
			       	    r.right=XSIZE/2+45;
			       	    r.bottom=YSIZE/3+30+i*30;
						POINT points[8]={{r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom}};
		   		        setfillcolor(GREEN);
		       			fillpolygon(points, 4);
		       			setbkmode(TRANSPARENT);
		        		switch(i)
					    {
			    		case 0:
			    		    drawtext("回到游戏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			    			break;
			    		case 1:
			    			drawtext("重新开始", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			    			break;
			    		case 2:
			    			drawtext(" 主 菜 单 ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			    			break;
						}						
					}
					FlushBatchDraw();
				}
			}
		}
	}
}
void control::gameStart()
{
	cleardevice();
	setfont(40,0,"方正舒体");	
	RECT r1 = {0, 0, XSIZE, YSIZE/3};
	drawtext("超级蘑菇", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setfont(20,0,"宋体");
	RECT r2={XSIZE/2-45,YSIZE/3,XSIZE/2+45,YSIZE/3+30};rectangle(XSIZE/2-45,YSIZE/3,XSIZE/2+45,YSIZE/3+30);
	drawtext("开始游戏", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r3={XSIZE/2-45,YSIZE/3+30,XSIZE/2+45,YSIZE/3+60};rectangle(XSIZE/2-45,YSIZE/3+30,XSIZE/2+45,YSIZE/3+60);
    drawtext("游戏介绍", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r4={XSIZE/2-45,YSIZE/3+60,XSIZE/2+45,YSIZE/3+90};rectangle(XSIZE/2-45,YSIZE/3+60,XSIZE/2+45,YSIZE/3+90);
	drawtext("操作说明", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r5={XSIZE/2-45,YSIZE/3+90,XSIZE/2+45,YSIZE/3+120};rectangle(XSIZE/2-45,YSIZE/3+90,XSIZE/2+45,YSIZE/3+120);
	drawtext("退出游戏", &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	bool _HOME=true,_INTRODUCTION=false,_OPERATION=false;//true表示处于当前页面
	MOUSEMSG m;
	while(_HOME)
	{
		BeginBatchDraw();
		m=GetMouseMsg();
		switch(m.uMsg)
		{
		case WM_LBUTTONDOWN:
			EndBatchDraw();
		    if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3&&m.y<YSIZE/3+30&&_HOME==1&&_INTRODUCTION==0&&_OPERATION==0)//如果选择“开始游戏”
		    {
				_HOME=false;
				break;
			}
			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+30&&m.y<YSIZE/3+60&&_HOME==1&&_OPERATION==0)//如果选择“游戏介绍”
			{
			    _INTRODUCTION=true;
				cleardevice();
				rectangle(50,50,213,220);
				outtextxy(52,52,"游戏介绍：");
				outtextxy(52,82,"超级玛丽变");
				outtextxy(52,102,"身超级蘑菇。");
				outtextxy(52,132,"开发者：");
				outtextxy(52,152,"木子念念");
				RECT R1={XSIZE-46,YSIZE-26,XSIZE-2,YSIZE-2};rectangle(XSIZE-46,YSIZE-26,XSIZE-2,YSIZE-2);
	            drawtext("返回", &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+60&&m.y<YSIZE/3+90&&_HOME==1&&_INTRODUCTION==0)//如果选择“操作说明”
			{
				_OPERATION=true;
				cleardevice();
				rectangle(50,50,213,220);
				outtextxy(52,52,"操作说明：");
				outtextxy(52,72,"左移：A键");
				outtextxy(52,92,"右移：D键");
				outtextxy(52,112,"发射：J键");
				outtextxy(52,132,"跳跃：W键/K键");
				outtextxy(52,152,"暂停：Esc键");
				RECT R2={XSIZE-46,YSIZE-26,XSIZE-2,YSIZE-2};rectangle(XSIZE-46,YSIZE-26,XSIZE-2,YSIZE-2);
	            drawtext("返回", &R2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+90&&m.y<YSIZE/3+120&&_HOME==1&&_INTRODUCTION==0&&_OPERATION==0)//如果选择“退出游戏”
			    exit(0);
			else if(m.x>XSIZE-46&&m.x<XSIZE-3&&m.y>YSIZE-26&&m.y<YSIZE-3&&(_INTRODUCTION==1||_OPERATION==1))//如果选择“返回”
			{
			    cleardevice();
				_HOME=false,_INTRODUCTION=false,_OPERATION=false;
				gameStart();
			}
			else
			    break;
		case WM_MOUSEMOVE:
			RECT r;
			if(_INTRODUCTION==1||_OPERATION==1)
			{
			    if(m.x>XSIZE-46&&m.x<XSIZE-3&&m.y>YSIZE-26&&m.y<YSIZE-3)
				{
				    r.left=XSIZE-46;
				    r.top=YSIZE-26;
			        r.right=XSIZE-2;
					r.bottom=YSIZE-2;
					POINT points[8]={{r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom}};
				    setfillcolor(RED);
					fillpolygon(points, 4);
					setbkmode(TRANSPARENT);
					drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					if(getpixel(XSIZE-46+1,YSIZE-26+1)==RED)
					{
					    r.left=XSIZE-46;
			         	r.top=YSIZE-26;
			            r.right=XSIZE-2;
			        	r.bottom=YSIZE-2;
						POINT points[8]={{r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom}};
						setfillcolor(BLACK);
					    fillpolygon(points, 4);
					    setbkmode(TRANSPARENT);
					    drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
			}
			else
			{
			    for(int i=0;i<4;i++)
				{
				    if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+i*30&&m.y<YSIZE/3+30+i*30)
				    {
					    r.left=XSIZE/2-45;
					    r.top=YSIZE/3+i*30;
					    r.right=XSIZE/2+45;
					    r.bottom=YSIZE/3+30+i*30;
						POINT points[8]={{r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom}};
					    setfillcolor(RED);
						fillpolygon(points, 4);
						setbkmode(TRANSPARENT);
						switch(i)
						{
						case 0:
						    drawtext("开始游戏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						    break;
						case 1:
						    drawtext("游戏介绍", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
						    drawtext("操作说明", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
					   	case 3:
							drawtext("退出游戏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
				    else
				    {
						if(getpixel(XSIZE/2-45+1,YSIZE/3+i*30+1)==RED)
						{
				       	    r.left=XSIZE/2-45;
				            r.top=YSIZE/3+i*30;
				       	    r.right=XSIZE/2+45;
				       	    r.bottom=YSIZE/3+30+i*30;
							POINT points[4]={{r.left,r.top}, {r.right,r.top}, {r.right,r.bottom}, {r.left,r.bottom}};
			   		        setfillcolor(BLACK);
			       			fillpolygon(points, 4);
			       			setbkmode(TRANSPARENT);
			        		switch(i)
						    {
				    		case 0:
				    		    drawtext("开始游戏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
				    		case 1:
				    			drawtext("游戏介绍", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
				    		case 2:
				    			drawtext("操作说明", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
				    	   	case 3:
				    			drawtext("退出游戏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
							}
						}
					}
				}
			}
			FlushBatchDraw();
			break;
			default:
			    break;
		 }
	}
}
void control::showScore(int score)
{
	setfont(0,0,"宋体");
	char s1[20]="SCORE:  ";
	char s2[5];
	itoa(score,s2,10);
	strcat(s1,s2);
	setbkmode(TRANSPARENT);
	outtextxy(10,10,s1);
	setbkmode(OPAQUE);
}
void control::showLevel(int level)
{
	setfont(0,0,"宋体");
	char s1[20]="LEVEL:  ";
	char s2[2];
	itoa(level,s2,10);
	strcat(s1,s2);
	setbkmode(TRANSPARENT);
	outtextxy(XSIZE-90,10,s1);
	setbkmode(OPAQUE);
}
void control::showDied(int life)
{
	cleardevice();
	char s1[20]="生命还剩下:  ";
	char s2[2];
	itoa(life,s2,10);
	outtextxy(XSIZE/2-43,YSIZE/3,s1);
	outtextxy(XSIZE/2,YSIZE/2-20,s2);
	Sleep(2000);
}
void control::showGameOver()
{
	IMAGE img;
	loadimage(&img,"res\\home.bmp",XSIZE,5*YSIZE);
	putimage(0,-YSIZE,&img);
	Sleep(6500);
}
void control::showPassed(int world)
{
	cleardevice();
	char s1[20]="LEVEL:  ";
	char s2[2];
	itoa(world,s2,10);
	outtextxy(XSIZE/2-20,YSIZE/3,s1);
	outtextxy(XSIZE/2,YSIZE/2-20,s2);
	Sleep(2000);
}
void control::showPassedAll()
{
	IMAGE img;
	loadimage(&img,"res\\home.bmp",XSIZE,5*YSIZE);
	putimage(0,-3*YSIZE,&img);
	Sleep(7800);
}