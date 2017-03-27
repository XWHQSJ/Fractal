#pragma once
#include"graphics.h"
#include"mydefine.h"
#include"scene.h"

class rule
{
public:
	rule(void);
	~rule(void);
	static double move(double &v,double t,double a);
};
