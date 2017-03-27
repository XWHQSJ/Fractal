#include "rule.h"
#include"math.h"

rule::rule(void)
{
}
rule::~rule(void)
{
}
double rule::move(double &v,double t,double a)
{
	double H=0;
	H=v*t+1.0/2.0*a*pow(t,2);
	v=v+a*t;	
	return H;
}