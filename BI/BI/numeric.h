#ifndef NUMERIC_H
#define NUMERIC_H

#include <qfile>
#ifndef GRAPHICSOBJ_H
#include "graphicsObj.h"
#endif

class Numeric2bAction
{
public:
	virtual bool isGap() = 0;
	virtual double impactAngle() = 0;
};

class Numeric2b
{
public:
	Numeric2b(Ball* pWhite, Ball* pVictim, Ball* pStep, Ball* pSndStep, PocketFrame* pPocket, QList<Ball>* plstObstructionBall, int chosenPocket, bool chosenGoTo, Numeric2bAction* act);
	~Numeric2b(){};

public:
	bool hit_2balls(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/);
	bool hit_3balls(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/, QList<BallTraj>* stepBallTraj /*out*/);
	bool hit_4balls(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/, QList<BallTraj>* stepBallTraj /*out*/);
	
	bool hit_2balls_anglesRotation(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/);
	bool hit_3balls_anglesRotation(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/, QList<BallTraj>* stepBallTraj /*out*/);
	bool hit_4balls_anglesRotation(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/, QList<BallTraj>* stepBallTraj /*out*/);

private:
	bool generateTraj2ballsShot(BallTraj& whiteBallTraj, TwinList<BallTraj>* wvList /*out*/, int lengthAhead = 0, int hitsAhead = 0);
	bool generateTraj3ballsShot(BallTraj& wbtj, QList<BallTraj>* wList /*out*/, QList<BallTraj>* vList /*out*/, QList<BallTraj>* sList /*out*/);
	bool generateTraj4ballsShot(BallTraj& wbtj, QList<BallTraj>* wList /*out*/, QList<BallTraj>* vList /*out*/, QList<BallTraj>* sList /*out*/, QList<BallTraj>* ssList /*out*/);
	bool generateTraj2ballsKickShot(BallTraj& whiteBallTraj, TwinList<BallTraj>* wvList /*out*/, int lengthAhead = 0, int hitsAhead = 0);
	bool generateTraj3ballsKickShot(BallTraj& whiteBallTraj, QList<BallTraj>* wList /*out*/, QList<BallTraj>* vList /*out*/, QList<BallTraj>* sList /*out*/);
	bool generateTraj4ballsKickShot(BallTraj& whiteBallTraj, QList<BallTraj>* wList /*out*/, QList<BallTraj>* vList /*out*/, QList<BallTraj>* sList /*out*/, QList<BallTraj>* ssList /*out*/);
	bool generateTraj1ball(BallTraj* ballTraj, int lengthAhead, int hitsAhead);
	qreal trajObstructedAtLength(BallTraj& traj);
	void sorts2trajs(TwinList<BallTraj>& wvTempList, QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/);
	void makeVelocityMarks(QList<BallTraj>* whiteBallTraj, QList<BallTraj>* victimBallTraj, QList<BallTraj>* stepBallTraj, QList<BallTraj>* sndStepBallTraj);

	void debugWrite(const QList<BallTraj>*);

	void digitalSortByPocketNumber(const TwinList<BallTraj>* inList, TwinList<BallTraj>* outList, QList<int>* nLst);
	void digitalSortByPocketNumber_and_excludeBadAngles(const TwinList<BallTraj>* inList, TwinList<BallTraj>* outList, QList<int>* nLst);
	void digitalSortByPocketNumber_and_excludeBadAngles_and_excludeBySideHits(const TwinList<BallTraj>* inList, TwinList<BallTraj>* outList);
	void excludeByTargetLines(TwinList<BallTraj>* inList, TwinList<BallTraj>* outList);
	void excludeByDistanceToCenter(const TwinList<BallTraj>* inList, TwinList<BallTraj>* outList);
	void removeUndue(int due, TwinList<BallTraj>* wvInList, QList<BallTraj>* outw, QList<BallTraj>* outv);
	void removeUndueBySideAndLength(const TwinList<BallTraj>* wvInList, QList<BallTraj>* outw, QList<BallTraj>* outv);

private:
	Ball* mp_whiteBall;
	Ball* mp_victimBall;
	Ball* mp_stepBall;
	Ball* mp_sndStepBall;
	QList<Ball>* mplst_obstructionBall;
	PocketFrame* mp_pocketFrame;
	Frame* m_frames;
	int m_chosenPocket;
	bool m_isGoTo;

	Numeric2bAction* m_pAction;

	QFile debugFile;
};


#endif //NUMERIC_H