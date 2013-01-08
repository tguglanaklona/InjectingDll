#include "distance.h"

double Motion::stoppingDistance() const
{
	return 0.5*v0*v0/MU;
}

double Motion::velocityAtDistance(double dist) const
{
	if (dist>=stoppingDistance())
		return 0.0;

	return sqrt(v0*v0-2.0*dist*MU);
}

double Motion::getv0() const
{
	return v0;
}

void Motion::setv0(double initialv)
{
	v0 = initialv;
}

double Motion::getx0() const
{
	return x0;
}

void Motion::setx0(double initialx)
{
	x0 = initialx;
}

bool Motion::operator= (const Motion& v)
{
	return (v0 = v.getv0())&&(x0 = v.getx0());
}

/*
NaturalPoint Distance::stoppingPoint() const
{
	double stop = velocity.stoppingDistance();
	if (ballTraj.length()>=stop)
	{
		NaturalPoint endp; int k = 0;
		QListIterator<NaturalPoint> natit(ballTraj.trList);
		while(natit.hasNext())
		{
			endp = natit.next(); 
			if (stop<=endp.naturalLength)
				break;
			k++;
		}
		if (stop<=endp.naturalLength)
		{
			natit.previous();
			NaturalPoint begp = natit.previous();
			double frac = (stop-begp.naturalLength)/(endp.naturalLength-begp.naturalLength);
			NaturalPoint stopp(0,0,stop);
			stopp.setX(begp.x() + (endp.x()-begp.x())*frac);
			stopp.setY(begp.y() + (endp.y()-begp.y())*frac);
			return stopp;
		}
	}

	return NaturalPoint(0,0,0);
}
*/

Mark Distance::stopPoint()
{
	if (lstpBallTraj.empty())
		return Mark();
	if (velocity0<EPS_EX)
		return Mark(NaturalPoint(lstpBallTraj[0]->center, 0), 0);

	double vend = velocity0;
	NaturalPoint stp;
	QListIterator<BallTraj*> lstit(lstpBallTraj);
	int k = 0; BallTraj* pBallTraj = NULL;
	while (lstit.hasNext())
	{
		if (k==0)
			velocity0 = vend;
		else
			velocity0 = vend*lstpBallTraj[k-1]->shotAngleCos.last();
		pBallTraj = lstit.next();
		stp = stoppingPoint(pBallTraj, &vend);
		if (!stp.isNull())
			return Mark(stp, k); 
		k++;
	}

	return Mark();
}

NaturalPoint Distance::stoppingPoint(const BallTraj* pBallTraj, double* endv /*out*/) const
{
	*endv = velocity0;
	Motion uniformMotion(velocity0, 0.0);
	double vend = velocity0; double v0 = vend;
	double stop = uniformMotion.stoppingDistance();
	if (pBallTraj->length()>=stop)
	{
		NaturalPoint endp; long double endcos = -1;
		QListIterator<NaturalPoint> natit(pBallTraj->trList);
		QListIterator<long double> anglit(pBallTraj->shotAngleCos);
		while(natit.hasNext())
		{
			endp = natit.next(); 
			if (stop<=endp.naturalLength)
				break;
			endcos = anglit.next();
			if (endcos!=-1){
				vend = uniformMotion.velocityAtDistance(endp.naturalLength - uniformMotion.getx0());
				v0 = vend * sqrt(1.0 - endcos*endcos); 
				uniformMotion.setv0(v0);
				uniformMotion.setx0(endp.naturalLength);
				stop = endp.naturalLength + uniformMotion.stoppingDistance();
			}
		}
		if (stop<=endp.naturalLength)
		{
			*endv = uniformMotion.velocityAtDistance(stop - uniformMotion.getx0());
			natit.previous();
			NaturalPoint begp = natit.previous();
			double frac = (stop-begp.naturalLength)/(endp.naturalLength-begp.naturalLength);
			NaturalPoint stopp(0,0,stop);
			stopp.setX(begp.x() + (endp.x()-begp.x())*frac);
			stopp.setY(begp.y() + (endp.y()-begp.y())*frac);
			return stopp;
		}
		else
		{
			*endv = uniformMotion.velocityAtDistance(endp.naturalLength - uniformMotion.getx0());
		}
	}
	else
	{
		*endv = uniformMotion.velocityAtDistance(pBallTraj->length());
	}

	return NaturalPoint(0,0,0);
}