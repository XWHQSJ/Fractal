/////////////////////////////////////////////////////////
// 程序名称：超级蘑菇2
// 编译环境：Visual C++ 2008 / 2012，EasyX 2013霜降版
// 作    者：木子念念 <www.baidu.com/p/taozi24>
// 最后修改：2013-5-30
//
#include"graphics.h"
#include"conio.h"
#include"MyTimer.h"
#include"mydefine.h"
#include"control.h"
#include"role.h"
#include"scene.h"
#pragma comment(lib,"Winmm.lib")   //给游戏添加音乐要用到它

//全局变量
int life=LIFE;   //主角有life条生命
int world=1;     //现在是第world关

void main()
{
	initgraph(XSIZE,YSIZE);

	control myCtrl;        //定义一个游戏控制
	role myRole(world);    //定义角色
	scene myScene(world);  //定义游戏场景

	myCtrl.gameStart();//游戏的开始界面

	//添加游戏音乐
	mciSendString("open res\\背景音乐.mp3 alias music_bg", NULL, 0, NULL);
	mciSendString("open res\\胜利.mp3 alias music_win", NULL, 0, NULL);
	mciSendString("open res\\通关.mp3 alias music_passedAll", NULL, 0, NULL);
	mciSendString("open res\\游戏结束.mp3 alias music_end",NULL,0,NULL);
	mciSendString("play music_bg repeat", NULL, 0, NULL);
	
	MyTimer mytimer;//精确延迟
	while(true)
	{
		//
		//控制层
		//
		int key=myCtrl.getKey(); //获取玩家按下的键                              
		if(key==VIR_RESTART)     //如果游戏过程中选择了“重新开始”
		{
			myScene=scene(world);mciSendString("play music_bg from 0", NULL, 0, NULL);
			myRole=role(world);			
		}
		else if(key==VIR_HOME)   //如果游戏过程中选择了“主菜单”
		{
			mciSendString("stop music_bg", NULL, 0, NULL);
			life=LIFE;
			world=1;
			myRole=role(world);
			myScene=scene(world);
			myCtrl.gameStart();
			mciSendString("play music_bg from 0", NULL, 0, NULL);
		}
		//
		//角色层
		//
		myRole.action(key,&myScene);  
		//
		//背景层
		//
		myScene.action(&myRole);  

		if(myRole.isDied())     //如果主角死亡
		{
			mciSendString("stop music_bg", NULL, 0, NULL);
			BeginBatchDraw();
			myScene.show();
			myRole.show();
			myCtrl.showScore(myRole.getScore());
			myCtrl.showLevel(world);
			EndBatchDraw();

			mytimer.Sleep(3500);
			life--;
			if(life==0)
			{
				mciSendString("play music_end from 0",NULL,0,NULL);
				myCtrl.showGameOver();
				life=LIFE;
				world=1;
				myCtrl.gameStart();
				myRole=role(world);
				myScene=scene(world);				
				mciSendString("play music_bg from 0", NULL, 0, NULL);
			}
			else
			{
				myCtrl.showDied(life);
				myRole=role(world);
				myScene=scene(world);		
				mciSendString("play music_bg from 0", NULL, 0, NULL);
			}
		}
		if(myRole.isPassed())   //如果主角过关
		{
			mciSendString("stop music_bg", NULL, 0, NULL);
			if(world==3)//如果主角通关
			{
				mciSendString("play music_win from 0", NULL, 0, NULL);
				mytimer.Sleep(6500);
			    mciSendString("play music_passedAll from 0",NULL,0,NULL);
				myCtrl.showPassedAll();
				life=LIFE;
				world=1;
				myRole=role(world);
				myScene=scene(world);
				myCtrl.gameStart();
				mciSendString("play music_bg from 0", NULL, 0, NULL);
			}
			else
			{
				mciSendString("play music_win from 0", NULL, 0, NULL);
				mytimer.Sleep(6500);
				world++;
				myCtrl.showPassed(world);
				myRole=role(world);
				myScene=scene(world);	
				mciSendString("play music_bg from 0", NULL, 0, NULL);
			}
		}
		

		//显示画面
		BeginBatchDraw();
		myScene.show();
		myRole.show();
		myCtrl.showScore(myRole.getScore());
		myCtrl.showLevel(world);
		EndBatchDraw();

		//延迟
		mytimer.Sleep((int)(TIME*1000));
	}
	mciSendString("clase all",NULL,0,NULL);//关闭所有多媒体音乐文件
	closegraph();
}