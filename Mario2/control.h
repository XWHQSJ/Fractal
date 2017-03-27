#pragma once
#include"graphics.h"

class control
{
private:
	int key;
private:
    int GetCommand();
	void pauseClick();
public:
	control(void);
	~control(void);
	int getKey();//获取玩家按下的键	
	void gameStart();//游戏开始界面
	void showScore(int score);
	void showLevel(int level);
	void showDied(int life);     //主角死亡时的动画
	void showGameOver();         //游戏结束时的动画
	void showPassed(int word);   //主角过关时的动画
	void showPassedAll();        //通关时的动画
};
