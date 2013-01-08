#ifndef DISTANCE_H
#define DISTANCE_H

#ifndef MISC_H
#include "misc.h"
#endif MISC_H
#ifndef GRAPHICSOBJ_H
#include "graphicsObj.h"
#endif GRAPHICSOBJ_H

class Motion 
{
public:
	Motion(double initialv, double initialx) : MU(0.5), v0(initialv), x0(initialx){};
	~Motion(){};

	double stoppingDistance() const;
	double velocityAtDistance(double) const;
	double getv0() const;
	void setv0(double initialv);
	double getx0() const;
	void setx0(double initialx);
	
	bool operator= (const Motion& v);
	
private:
	const double MU;
	double v0;
	double x0;
}; 

class Distance
{
public:
	Distance(const QList<BallTraj*>& lstpTraj, double vel): lstpBallTraj(lstpTraj), velocity0(vel){};
	~Distance(){};
	
	Mark stopPoint(); //non-const

private:
	NaturalPoint stoppingPoint(const BallTraj* ballTraj, double* endv /*out*/) const;

private:
	double velocity0;
	QList<BallTraj*> lstpBallTraj;
};

#endif //DISTANCE_H