#pragma once
#include"graphics.h"
#include"mydefine.h"

const int MAP_NUMBER=30;
const int COINS_NUMBER=70;
const int SCORE_NUMBER=5;
const int FOOD_NUMBER=5;
class role;//超前说明（可以避免重复包含头文件）
#ifndef _MAP
#define _MAP
struct Map//地图的结构体
{
	int x;
	int y;
	int id;	
	int xAmount;//横向地图块的个数
	int yAmount;//纵向地图块的个数
	double u;//摩擦因数
};
#endif
class scene
{	
private:
	double xBg;//背景坐标
	double yBg;
	int xMap;//地图坐标
	int yMap;	
	Map map[MAP_NUMBER];
	POINT coins[COINS_NUMBER];
	POINT score[SCORE_NUMBER];
	POINT food[FOOD_NUMBER];
	IMAGE img_bg;
	IMAGE img_map;
	IMAGE img_scenery;
	IMAGE img_coin;
	IMAGE img_food;
	double scenery_iframe;//帧数
	double coin_iframe;
	double score_iframe[SCORE_NUMBER];
	double food_iframe;
	int world;//第world关
public:
	scene(int world);
	~scene(void);
	Map *getMap(){return map;}
	POINT *getCoins(){return coins;}
	POINT *getFood(){return food;}
	void setScorePos(int x,int y);
	void setFood(int x,int y);
	void createMap(int world);//创造第world关的地图	
	void createCoin();
	void createFood();
	void action(role *myRole);
	void show();	
	bool isEnding(int distance);
};
