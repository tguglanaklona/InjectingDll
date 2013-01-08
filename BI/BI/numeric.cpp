#include "numeric.h"

#ifndef TIPSWND_H
#include "TipsWnd.h"
#endif TIPSWND_H

Numeric2b::Numeric2b(Ball* pwhite, Ball* pvictim, Ball* pStep, Ball* pSndStep, PocketFrame* ppocket, QList<Ball>* plstObstructionBall, int chosenPocket, bool chosenGoTo, Numeric2bAction* act): mp_whiteBall(pwhite), 
	mp_victimBall(pvictim), mp_pocketFrame(ppocket), debugFile("debug.log"), m_frames(NULL), mplst_obstructionBall(plstObstructionBall), mp_stepBall(pStep),
	mp_sndStepBall(pSndStep), m_chosenPocket(chosenPocket), m_isGoTo(chosenGoTo), m_pAction(act)
{
	if (!(m_frames = ppocket->getpFrame()))
		return; //error
}

qreal Numeric2b::trajObstructedAtLength(BallTraj& traj)
{
	if (traj.nObstructed!=-1)
		return -1; //error

	qreal min;
	if (mplst_obstructionBall){
		if (!mplst_obstructionBall->empty()){
			min = 100*mp_pocketFrame->fstFrameDiag();
			for (int i=0;i<mplst_obstructionBall->count();i++){
				if (Number<int>(i)!=traj.obs) {
					qreal length = traj.obstructedAtLength(mplst_obstructionBall->at(i), m_pAction->isGap());
					if (length!=-1){
						if (length<min){
							min = length;
							traj.nObstructed = i;
						}
					}
				}
			}
		}
	}

	if (traj.nObstructed == -1)
		return -1;
	else
		return min;
}

bool Numeric2b::generateTraj4ballsKickShot(BallTraj& wbtj, QList<BallTraj>* wList /*out*/, QList<BallTraj>* vList /*out*/, QList<BallTraj>* sList /*out*/, QList<BallTraj>* ssList /*out*/)
{
	qreal lh = trajObstructedAtLength(wbtj);

	if (!wbtj.framesCrossing()) //(1 соударение white со стенкой)
	{
		if (lh!=-1) { 
			if (wbtj.length() >= lh){
				wbtj.obs.append(wbtj.nObstructed);
				return false;
			}
			else{ 
				wbtj.nObstructed = -1;
			}
		}

		return generateTraj4ballsShot(wbtj,wList,vList,sList,ssList);
	}

	return false;
}

bool Numeric2b::generateTraj3ballsKickShot(BallTraj& wbtj, QList<BallTraj>* wList /*out*/, QList<BallTraj>* vList /*out*/, QList<BallTraj>* sList /*out*/)
{
	qreal lh = trajObstructedAtLength(wbtj);

	if (!wbtj.framesCrossing()) //(1 соударение white со стенкой)
	{
		if (lh!=-1) { 
			if (wbtj.length() >= lh){
				wbtj.obs.append(wbtj.nObstructed);
				return false;
			}
			else{ 
				wbtj.nObstructed = -1;
			}
		}

		return generateTraj3ballsShot(wbtj,wList,vList,sList);
	}

	return false;
}

bool Numeric2b::generateTraj2ballsKickShot(BallTraj& wbtj, TwinList<BallTraj>* wvList /*out*/, int lengthAheadWbtj, int hitsAheadWbtj)
{	
	qreal lh = trajObstructedAtLength(wbtj);

	if (!wbtj.framesCrossing()) //1st kick
	{
		if (lh!=-1) { 
			if (wbtj.length() >= lh){
				wbtj.obs.append(wbtj.nObstructed);
				return false;
			}
			else{ 
				wbtj.nObstructed = -1;
			}
		}

		bool notnull = generateTraj2ballsShot(wbtj,wvList,lengthAheadWbtj,hitsAheadWbtj);
		
		if ((!notnull)&&(wbtj.nObstructed==-1)) 
		{
			qreal lh = trajObstructedAtLength(wbtj);

			if (!wbtj.framesCrossing()) //2nd kick
			{
				if (lh!=-1) { 
					if (wbtj.length() >= lh){
						wbtj.obs.append(wbtj.nObstructed);
						return false;
					}
					else{ 
						wbtj.nObstructed = -1;
					}
				}

				return generateTraj2ballsShot(wbtj,wvList,lengthAheadWbtj,hitsAheadWbtj);
			}
		}
		return notnull;
	}

	return false;
}
		
/*bool Numeric2b::generateTraj2ballsKickShot(BallTraj& wbtj, TwinList<BallTraj>* wvList)
{	
	qreal lh = trajObstructedAtLength(wbtj);

	if (!wbtj.framesCrossing()) //(1 соударение white со стенкой)
	{
		if (lh!=-1) { 
			if (wbtj.length() >= lh){
				wbtj.obs.append(wbtj.nObstructed);
				return false;
			}
			else{ 
				wbtj.nObstructed = -1;
			}
		}

		return generateTraj2ballsShot(wbtj,wvList);
	}

	return false;
}*/

bool Numeric2b::generateTraj1ball(BallTraj* ballTraj, int lengthAhead, int hitsAhead)
{
	bool b = false;
	qreal lh = trajObstructedAtLength(*ballTraj); 
	if (lh==-1)
	{
		b = ballTraj->framesCrossing(); 
		if ((!b)&&(lengthAhead + ballTraj->length()<ballTraj->MAX_LENGTH())&&(hitsAhead + ballTraj->nSideHits<5))
			b = generateTraj1ball(ballTraj, lengthAhead, hitsAhead);
	}
	else
	{
		if (ballTraj->ballCrossing(mplst_obstructionBall->at(ballTraj->nObstructed), m_pAction->isGap())!=Vector(0,0)){
			ballTraj->obs.append(ballTraj->nObstructed);
			ballTraj->nObstructed = -1;
			ballTraj->nSideHits++;
			if ((lengthAhead + ballTraj->length()<ballTraj->MAX_LENGTH())&&(hitsAhead + ballTraj->nSideHits<5))
				b = generateTraj1ball(ballTraj, lengthAhead, hitsAhead);
		}
	}
	return b;
}

bool Numeric2b::generateTraj4ballsShot(BallTraj& wbtj, QList<BallTraj>* wList /*out*/, QList<BallTraj>* vList /*out*/, QList<BallTraj>* sList /*out*/, QList<BallTraj>* ssList /*out*/)
{
	qreal lh = trajObstructedAtLength(wbtj);
	Vector v = wbtj.ballCrossing(*mp_stepBall, m_pAction->isGap());

	if ((lh!=-1)&&(!v.isNull())) {
		if (wbtj.length() >= lh){ 
			wbtj.obs.append(wbtj.nObstructed);
			return false;
		}
		else{
			wbtj.nObstructed = -1;
		}
	}

	if (!v.isNull())
	{
		BallTraj sbtj(mp_pocketFrame,*mp_stepBall,v);

		lh = trajObstructedAtLength(sbtj);
		v = sbtj.ballCrossing(*mp_sndStepBall, m_pAction->isGap());

		if ((lh!=-1)&&(!v.isNull())) {
			if (sbtj.length() >= lh){ 
				sbtj.obs.append(sbtj.nObstructed);
				return false;
			}
			else{
				sbtj.nObstructed = -1;
			}
		}

		if (!v.isNull())
		{
			BallTraj ssbtj(mp_pocketFrame,*mp_sndStepBall,v);

			lh = trajObstructedAtLength(ssbtj);
			v = ssbtj.ballCrossing(*mp_victimBall, m_pAction->isGap());

			if ((lh!=-1)&&(!v.isNull())) {
				if (ssbtj.length() >= lh){ 
					ssbtj.obs.append(ssbtj.nObstructed);
					return false;
				}
				else{
					ssbtj.nObstructed = -1;
				}
			}

			if (!v.isNull())
			{
				BallTraj vbtj(mp_pocketFrame,*mp_victimBall,v);
				bool b = generateTraj1ball(&vbtj, wbtj.length()+sbtj.length()+ssbtj.length(), wbtj.nSideHits+sbtj.nSideHits+ssbtj.nSideHits);

				if (b)
				{
					wList->append(wbtj);
					vList->append(vbtj);
					sList->append(sbtj);
				}

				return true;
			}
			//else kick shot
		}
		//else kick shot
	}

	return false;
}

bool Numeric2b::generateTraj3ballsShot(BallTraj& wbtj, QList<BallTraj>* wList /*out*/, QList<BallTraj>* vList /*out*/, QList<BallTraj>* sList /*out*/)
{
	qreal lh = trajObstructedAtLength(wbtj);
	Vector v = wbtj.ballCrossing(*mp_stepBall, m_pAction->isGap());

	if ((lh!=-1)&&(!v.isNull())) {
		if (wbtj.length() >= lh){ 
			wbtj.obs.append(wbtj.nObstructed);
			return false;
		}
		else{
			wbtj.nObstructed = -1;
		}
	}

	if (!v.isNull())
	{
		TwinList<BallTraj> svTempList;

		BallTraj sbtj(mp_pocketFrame,*mp_stepBall,v);//shot
		bool notnull = generateTraj2ballsShot(sbtj, &svTempList, wbtj.length(), wbtj.nSideHits);

		if ((!notnull)&&(sbtj.nObstructed==-1))//kick shot
		{
			generateTraj2ballsKickShot(sbtj, &svTempList, wbtj.length(), wbtj.nSideHits);	
		}

		if (!svTempList.empty())
		{
			wList->append(wbtj);
			vList->append(svTempList.victim);
			sList->append(svTempList.white);
		}

		return notnull;
	}
	
	return false;
}

bool Numeric2b::generateTraj2ballsShot(BallTraj& wbtj, TwinList<BallTraj>* wvList /*out*/, int lengthAheadWbtj, int hitsAheadWbtj)
{
	qreal lh = trajObstructedAtLength(wbtj);
	Vector v = wbtj.ballCrossing(*mp_victimBall, m_pAction->isGap());

	if ((lh!=-1)&&(!v.isNull())) {
		if (wbtj.length() >= lh){ 
			wbtj.obs.append(wbtj.nObstructed);
			return false;
		}
		else{
			wbtj.nObstructed = -1;
		}
	}

	int temp = 0;
	if (!v.isNull())
	{
		BallTraj vbtj(mp_pocketFrame,*mp_victimBall,v);
		bool b = generateTraj1ball(&vbtj, lengthAheadWbtj + wbtj.length(), hitsAheadWbtj + wbtj.nSideHits);
		temp++;

		if (b || m_isGoTo)
		{
			temp = 0;
			wvList->white.append(wbtj);
			wvList->victim.append(vbtj);

			wvList->white.last().whitePocket = generateTraj1ball(&wbtj, lengthAheadWbtj + wbtj.length(), hitsAheadWbtj + wbtj.nSideHits);
		}

		return true;
	}
	
	return false;
}

bool Numeric2b::hit_4balls(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/, QList<BallTraj>* stepBallTraj /*out*/) 
{
	if ((!mp_whiteBall->isValid())||(!mp_victimBall->isValid())||(!mp_stepBall->isValid())||(!mp_sndStepBall->isValid()))
		return false; 

	if (WalkPoint(round(mp_whiteBall->center)).outOfRect(m_frames->sndFrameRect) || 
		WalkPoint(round(mp_victimBall->center)).outOfRect(m_frames->sndFrameRect) ||
		WalkPoint(round(mp_stepBall->center)).outOfRect(m_frames->sndFrameRect) || 
		WalkPoint(round(mp_sndStepBall->center)).outOfRect(m_frames->sndFrameRect))
		return false;

	QPoint wcenter = round(mp_whiteBall->center);
	WalkPoint p(wcenter);
	p.setX(m_frames->sndFrameCoord(Left));

	WalkPoint firstPoint(p);
	Vector localSearchingVector = firstPoint-wcenter;

	QList<BallTraj> wTempList;
	QList<BallTraj> vTempList;
	QList<BallTraj> sTempList;
	QList<BallTraj> ssTempList;
	int i = 0;

	do
	{
		//first shot
		BallTraj wbtj(mp_pocketFrame,*mp_whiteBall,localSearchingVector); 
		bool notnull = generateTraj4ballsShot(wbtj, &wTempList, &vTempList, &sTempList, &ssTempList);

		//kick shot 
		if ((!notnull)&&(wbtj.nObstructed == -1))
		{
			generateTraj4ballsKickShot(wbtj, &wTempList, &vTempList, &sTempList, &ssTempList);
		}

		p.nextGoRect(m_frames->sndFrameRect);
		localSearchingVector = p-wcenter;
		i++;

	} while ((p!=firstPoint)&&(i<=2*(2*m_frames->sndFrameRect.width()+2*m_frames->sndFrameRect.height())));

	if ( wTempList.empty() )
		return false;

	sorts2trajs(TwinList<BallTraj>(wTempList, vTempList), whiteBallTraj, victimBallTraj);
	
	for (int i=0;i<whiteBallTraj->count();i++)
		stepBallTraj->append(sTempList.at(wTempList.indexOf(whiteBallTraj->at(i))));

	return true;
}

bool Numeric2b::hit_4balls_anglesRotation(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/, QList<BallTraj>* stepBallTraj /*out*/) 
{
	if ((!mp_whiteBall->isValid())||(!mp_victimBall->isValid())||(!mp_stepBall->isValid())||(!mp_sndStepBall->isValid()))
		return false; 

	Vector firstVector(-1,0);
	long double firstAngle = firstVector.anglex();

	Vector localSearchingVector = firstVector;
	Number<double> phi (localSearchingVector.anglex());

	QList<BallTraj> wTempList;
	QList<BallTraj> vTempList;
	QList<BallTraj> sTempList;
	QList<BallTraj> ssTempList;
	int i = 0;

	do
	{
		//first shot
		BallTraj wbtj(mp_pocketFrame,*mp_whiteBall,localSearchingVector); 
		bool notnull = generateTraj4ballsShot(wbtj, &wTempList, &vTempList, &sTempList, &ssTempList);

		//kick shot 
		if ((!notnull)&&(wbtj.nObstructed == -1))
		{
			generateTraj4ballsKickShot(wbtj, &wTempList, &vTempList, &sTempList, &ssTempList);
		}

		localSearchingVector.rotate(long double(MIN_ANGLE));
		phi = localSearchingVector.anglex();
		i++;

	} while ((! (phi.in(firstAngle, firstAngle + long double(MIN_ANGLE) - long double(EPS_EX))))&&(i<=int(2*long double(PI)/long double(MIN_ANGLE)) + 2));

	if ( wTempList.empty() )
		return false;

	sorts2trajs(TwinList<BallTraj>(wTempList, vTempList), whiteBallTraj, victimBallTraj);
	
	for (int i=0;i<whiteBallTraj->count();i++)
		stepBallTraj->append(sTempList.at(wTempList.indexOf(whiteBallTraj->at(i))));

	return true;
}

bool Numeric2b::hit_3balls(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/, QList<BallTraj>* stepBallTraj /*out*/) 
{
	if ((!mp_whiteBall->isValid())||(!mp_victimBall->isValid())||(!mp_stepBall->isValid()))
		return false; 

	if (WalkPoint(round(mp_whiteBall->center)).outOfRect(m_frames->sndFrameRect) || 
		WalkPoint(round(mp_victimBall->center)).outOfRect(m_frames->sndFrameRect)||
		WalkPoint(round(mp_stepBall->center)).outOfRect(m_frames->sndFrameRect))
		return false;

	QPoint wcenter = round(mp_whiteBall->center);
	WalkPoint p(wcenter);
	p.setX(m_frames->sndFrameCoord(Left));

	WalkPoint firstPoint(p);
	Vector localSearchingVector = firstPoint-wcenter;

	QList<BallTraj> wTempList;
	QList<BallTraj> vTempList;
	QList<BallTraj> sTempList;
	int i = 0;

	do
	{
		//first shot
		BallTraj wbtj(mp_pocketFrame,*mp_whiteBall,localSearchingVector); 
		bool notnull = generateTraj3ballsShot(wbtj, &wTempList, &vTempList, &sTempList);

		//kick shot 
		if ((!notnull)&&(wbtj.nObstructed == -1))
		{
			generateTraj3ballsKickShot(wbtj, &wTempList, &vTempList, &sTempList);
		}

		p.nextGoRect(m_frames->sndFrameRect);
		localSearchingVector = p-wcenter;
		i++;

	} while ((p!=firstPoint)&&(i<=2*(2*m_frames->sndFrameRect.width()+2*m_frames->sndFrameRect.height())));

	if ( wTempList.empty() )
		return false;

	sorts2trajs(TwinList<BallTraj>(wTempList, vTempList), whiteBallTraj, victimBallTraj);
	
	for (int i=0;i<whiteBallTraj->count();i++)
		stepBallTraj->append(sTempList.at(wTempList.indexOf(whiteBallTraj->at(i))));

	return true;
}

bool Numeric2b::hit_3balls_anglesRotation(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/, QList<BallTraj>* stepBallTraj /*out*/) 
{
	if ((!mp_whiteBall->isValid())||(!mp_victimBall->isValid())||(!mp_stepBall->isValid()))
		return false; 

	Vector firstVector(-1,0);
	long double firstAngle = firstVector.anglex();

	Vector localSearchingVector = firstVector;
	Number<double> phi (localSearchingVector.anglex());

	QList<BallTraj> wTempList;
	QList<BallTraj> vTempList;
	QList<BallTraj> sTempList;
	int i = 0;

	do
	{
		//first shot
		BallTraj wbtj(mp_pocketFrame,*mp_whiteBall,localSearchingVector); 
		bool notnull = generateTraj3ballsShot(wbtj, &wTempList, &vTempList, &sTempList);

		//kick shot 
		if ((!notnull)&&(wbtj.nObstructed == -1))
		{
			generateTraj3ballsKickShot(wbtj, &wTempList, &vTempList, &sTempList);
		}

		localSearchingVector.rotate(long double(MIN_ANGLE));
		phi = localSearchingVector.anglex();
		i++;

	} while ((! (phi.in(firstAngle, firstAngle + long double(MIN_ANGLE) - long double(EPS_EX))))&&(i<=int(2*long double(PI)/long double(MIN_ANGLE)) + 2));

	if ( wTempList.empty() )
		return false;

	sorts2trajs(TwinList<BallTraj>(wTempList, vTempList), whiteBallTraj, victimBallTraj);
	
	for (int i=0;i<whiteBallTraj->count();i++)
		stepBallTraj->append(sTempList.at(wTempList.indexOf(whiteBallTraj->at(i))));

	return true;
}

/*

v.setv0(10.0);		 //0.5 pcs
v.setv0(18.9);		 //1.5 pcs
Velocity velocity(14.9963); //1 pc
v.setv0(22.1839);    //2 pcs
v.setv0(27.75);    //3 pcs

*/

void Numeric2b::makeVelocityMarks(QList<BallTraj>* whiteBallTraj, QList<BallTraj>* victimBallTraj, QList<BallTraj>* stepBallTraj, QList<BallTraj>* sndStepBallTraj)
{
	const int N = 5;
	double label[N] = {1.0, 2.0, 3.0, 4.0, 5.0}; //5.0
	double initial_velocity[N] = {14.9963, 22.1839, 27.55, 31.91, 35.7}; //~ x^(2.75/5)*15
	for (int i=0; i<whiteBallTraj->count(); i++)
	{
		for (int v=0; v<N; v++)
		{
			if ((stepBallTraj==NULL)&&(sndStepBallTraj==NULL))
			{
				QList<BallTraj*> trajList;
				trajList.append(&whiteBallTraj->operator[](i));
				trajList.append(&victimBallTraj->operator[](i));//за v
				Distance dist(trajList, initial_velocity[v]);
				Mark mark = dist.stopPoint();
				if (!mark.isNull())
				{
					int ntraj = mark.label;
					mark.label = label[v];
					if (ntraj==0)
						whiteBallTraj->operator[](i).velMarks.append(mark);
					else
						victimBallTraj->operator[](i).velMarks.append(mark);
				}


			}



		}
	}//for
}

bool Numeric2b::hit_2balls(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/) 
{
	if ((!mp_whiteBall->isValid())||(!mp_victimBall->isValid()))
		return false; 

	if (WalkPoint(round(mp_whiteBall->center)).outOfRect(m_frames->sndFrameRect) || 
		WalkPoint(round(mp_victimBall->center)).outOfRect(m_frames->sndFrameRect))
		return false;
	
	QPoint wcenter = round(mp_whiteBall->center);
	WalkPoint p(wcenter);
	p.setX(m_frames->sndFrameCoord(Left));

	WalkPoint firstPoint(p);
	Vector localSearchingVector = firstPoint-wcenter;

	TwinList<BallTraj> wvTempList;
	int i = 0;
	do
	{
		//shot
		BallTraj wbtj(mp_pocketFrame,*mp_whiteBall,localSearchingVector); 
		if (m_isGoTo) wbtj.isGoToCos = cos(m_pAction->impactAngle()*PI/180.0);
		bool notnull = generateTraj2ballsShot(wbtj,&wvTempList);

		//kick shot 
		if ((!notnull)&&(wbtj.nObstructed == -1))
		{
			generateTraj2ballsKickShot(wbtj,&wvTempList);
		}

		p.nextGoRect(m_frames->sndFrameRect);
		localSearchingVector = p-wcenter;
		i++;

	} while ((p!=firstPoint)&&(i<=2*(2*m_frames->sndFrameRect.width()+2*m_frames->sndFrameRect.height())));

	if ( wvTempList.white.empty() )
		return false;

	if (!m_isGoTo)
		sorts2trajs(wvTempList, whiteBallTraj, victimBallTraj);
	else
		removeUndue(7, &wvTempList, whiteBallTraj, victimBallTraj);
	makeVelocityMarks(whiteBallTraj, victimBallTraj, NULL, NULL);

	return true; 
}

bool Numeric2b::hit_2balls_anglesRotation(QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/) 
{
	if ((!mp_whiteBall->isValid())||(!mp_victimBall->isValid()))
		return false; 

	Vector firstVector(-1,0);
	long double firstAngle = firstVector.anglex();

	Vector localSearchingVector = firstVector;
	Number<double> phi (localSearchingVector.anglex());

	TwinList<BallTraj> wvTempList;
	int i = 0;
	do
	{
		//shot
		BallTraj wbtj(mp_pocketFrame,*mp_whiteBall,localSearchingVector); 
		bool notnull = generateTraj2ballsShot(wbtj,&wvTempList);

		//kick shot 
		if ((!notnull)&&(wbtj.nObstructed == -1))
		{
			generateTraj2ballsKickShot(wbtj,&wvTempList);
		}

		localSearchingVector.rotate(long double(MIN_ANGLE));
		phi = localSearchingVector.anglex();
		i++;

	} while ((! (phi.in(firstAngle, firstAngle + long double(MIN_ANGLE) - long double(EPS_EX))))&&(i<=int(2*long double(PI)/long double(MIN_ANGLE)) + 2));

	if ( wvTempList.white.empty() )
		return false;

	sorts2trajs(wvTempList, whiteBallTraj, victimBallTraj);

	return true; 
}

void Numeric2b::sorts2trajs(TwinList<BallTraj>& wvTempList, QList<BallTraj>* whiteBallTraj /*out*/, QList<BallTraj>* victimBallTraj /*out*/)
{
	if (wvTempList.white.empty())
		return;

	if (wvTempList.white.count()==1) {
		whiteBallTraj->append(wvTempList.white.first());
		victimBallTraj->append(wvTempList.victim.first());
		return;
	}
	
	TwinList<BallTraj> tmpList;
	
	digitalSortByPocketNumber_and_excludeBadAngles_and_excludeBySideHits(&wvTempList, &tmpList);
	wvTempList.clear();
	
	excludeByTargetLines(&tmpList, &wvTempList);
	//excludeByDistanceToCenter(&tmpList, &wvTempList);
	
	if (m_chosenPocket==0)
		removeUndue(3, &wvTempList, whiteBallTraj, victimBallTraj);
	else
		removeUndue(5, &wvTempList, whiteBallTraj, victimBallTraj);
	//debugWrite(whiteBallTraj);
}

void Numeric2b::removeUndue(int due, TwinList<BallTraj>* wvInList, QList<BallTraj>* outw, QList<BallTraj>* outv)
{
	int count = wvInList->white.count();
	if (count<=due){
		outw->append(wvInList->white);
		outv->append(wvInList->victim);
		return;
	}

	if (count-due==1){
		removeUndueBySideAndLength(wvInList, outw, outv);
	}
	else{
		TwinList<BallTraj> tmpList;
		for (int i=1;i<=count-due;i++)
		{
			if (count-due!=i){
				removeUndueBySideAndLength(wvInList, &tmpList.white, &tmpList.victim);
				wvInList->clear(); wvInList->append(tmpList); tmpList.clear();
			}
			else{
				removeUndueBySideAndLength(wvInList, outw, outv);
			}
		}
	}
}

void Numeric2b::debugWrite(const QList<BallTraj>* pList)
{
	debugFile.open(QIODevice::WriteOnly);
	QDataStream debug(&debugFile);
	for (int i=0;i<pList->count();i++)
	{
		BallTraj temp = pList->at(i);
		int i1 = temp.pocketBallnPocket;
		int i2 = temp.nSideHits;
		int i3 = temp.pocketBallDistToCenter;
		int i4 = temp.trList.getLength();
		Vector i5 = temp.trList.at(1)-temp.trList.first();
		QString c = QString("pock %1, side %2, dist %3, length %4, s (%5, %6)").arg(i1).arg(i2).arg(i3).arg(i4).arg(i5.x()).arg(i5.y());
		debug << c <<"\n";	
	}
	debugFile.close();
}

void Numeric2b::digitalSortByPocketNumber(const TwinList<BallTraj>* inList, TwinList<BallTraj>* outList, QList<int>* nLst)
{	
	TwinList<BallTraj> T[6];
	TwinListIteratorPtr<BallTraj> iter;
	Twin<BallTraj> item;

	for (int i=0;i<6;i++)
		T[i].clear();
	
	iter.newObjects(inList->white, inList->victim);
	while(iter.white->hasNext()){
		item = iter.next();
		T[item.victim.pocketBallnPocket-1].append(item);
	}
	iter.deleteObjects();

	int k = 0;
	for (int i=0;i<6;i++){
		outList->append(T[i]);
		if (!T[i].white.empty()){
			if (k==0)
				nLst->append(T[i].white.count());
			else
				nLst->append(nLst->at(k-1)+T[i].white.count());
			k++;
		}
	}
}

void Numeric2b::digitalSortByPocketNumber_and_excludeBadAngles(const TwinList<BallTraj>* inList, TwinList<BallTraj>* outList, QList<int>* nLst)
{	
	TwinList<BallTraj> T[6];
	TwinListIteratorPtr<BallTraj> iter;
	Twin<BallTraj> item;

	for (int i=0;i<6;i++)
		T[i].clear();
	int group[6] = {0,0,0,0,0,0};
	int prev_group[6] = {0,0,0,0,0,0};
	long double angle = -1;
	
	iter.newObjects(*inList);
	while(iter.white->hasNext()){
		item = iter.next();
		int n = item.victim.pocketBallnPocket-1;

		angle = item.white.shotAngleCos.last();
		prev_group[n] = group[n];
		if ((angle>=0.0)&&(angle<=0.5))
			group[n] = 1;
		else if ((angle>=0.5)&&(angle<=0.5*sqrt(3.0)))
			group[n] = 2;
		else if ((angle>=0.5*sqrt(3.0)&&(angle<=1.0 + EPS_EX)))
			group[n] = 3;
		else
			group[n] = 0; //error

		if (group[n] == prev_group[n]){
			T[n].append(item);
		}
		else if (group[n] > prev_group[n]){
			T[n].clear();
			T[n].append(item);
		}
		else{ //if (group[n] < prev_group[n])
		}
	}
	iter.deleteObjects();

	int k = 0;
	for (int i=0;i<6;i++){
		outList->append(T[i]);
		if (!T[i].white.empty()){
			if (k==0)
				nLst->append(T[i].white.count());
			else
				nLst->append(nLst->at(k-1)+T[i].white.count());
			k++;
		}
	}
}

void Numeric2b::digitalSortByPocketNumber_and_excludeBadAngles_and_excludeBySideHits(const TwinList<BallTraj>* inList, TwinList<BallTraj>* outList)
{	
	TwinList<BallTraj> T[6];
	TwinListIteratorPtr<BallTraj> iter;
	Twin<BallTraj> item;

	int n = 0;
	for (int i=0;i<6;i++)
		T[i].clear();
	int group[6] = {0,0,0,0,0,0};
	int prev_group[6] = {0,0,0,0,0,0};
	long double angle = -1;
	int minSideHits[6];
	for (int i=0;i<6;i++)
		minSideHits[i] = 1000;
	
	iter.newObjects(*inList);
	while(iter.white->hasNext()){ //1 проход
		item = iter.next();
		n = item.victim.pocketBallnPocket-1;

		if (m_chosenPocket==0)
		{
			angle = item.white.shotAngleCos.last();
			if ((angle>=0.0)&&(angle<=0.5))
				group[n] = 1;
			else if ((angle>=0.5)&&(angle<=0.5*sqrt(3.0)))
				group[n] = 2;
			else if ((angle>=0.5*sqrt(3.0)&&(angle<=1.0 + EPS_EX)))
				group[n] = 3;
			else
				group[n] = 0; //error

			if (group[n] == prev_group[n]){

				int s = item.victim.nSideHits + item.white.nSideHits;
				if (s < minSideHits[n]){
					minSideHits[n] = s;
					T[n].clear();
					T[n].append(item);
				}
				else if (s == minSideHits[n]){
					T[n].append(item);
				}
				else{
				}
			}
			else if (group[n] > prev_group[n]){
				minSideHits[n] = 1000;
				prev_group[n] = group[n];

				int s = item.victim.nSideHits + item.white.nSideHits;
				if (s < minSideHits[n]){
					minSideHits[n] = s;
					T[n].clear();
					T[n].append(item);
				}
				else if (s == minSideHits[n]){
					T[n].append(item);
				}
				else{
				}
			}
			else{ 
			}
		}
		else if (n+1==m_chosenPocket)
		{
			T[n].append(item);
		}
		else
		{
		}

	}
	iter.deleteObjects();

	for (int i=0;i<6;i++)
		outList->append(T[i]);
}

void Numeric2b::excludeByTargetLines(TwinList<BallTraj>* inList, TwinList<BallTraj>* outList)
{
	if (inList->empty())
		return;

	if (inList->white.count()==1){
		outList->append(*inList);
		return;
	}

	TwinListIteratorPtr<BallTraj> iter;
	Twin<BallTraj> item;
	iter.newObjects(*inList);

	int nPock = -1; 
	int nSide = -1;
	int np = 0;
	
	int n = 0;
	item = iter.next();
	nPock = item.victim.pocketBallnPocket;
	nSide = item.victim.nSideHits+item.white.nSideHits;
	while(iter.white->hasNext())
	{
		np = n;
		do
		{
			item = iter.next();
			n++;
		} while ((item.victim.pocketBallnPocket==nPock)&&(item.victim.nSideHits+item.white.nSideHits==nSide)&&(iter.white->hasNext()));
		nPock = item.victim.pocketBallnPocket;
		nSide = item.victim.nSideHits+item.white.nSideHits;
		int index = np + int(0.5*(n-np));
		if (n-np==0)
			(inList->victim)[index].incorrectness = 1;
		if (n-np==1)
			(inList->victim)[index].incorrectness = 2;
		outList->append(inList->at(index));
	}
	item = inList->at(inList->count()-2);
	if ((item.victim.pocketBallnPocket!=nPock)||(item.victim.nSideHits+item.white.nSideHits!=nSide)){
		int index = inList->count()-1;
		(inList->victim)[index].incorrectness = 1;
		outList->append(inList->at(index));
	}

	iter.deleteObjects();
}

void Numeric2b::excludeByDistanceToCenter(const TwinList<BallTraj>* inList, TwinList<BallTraj>* outList)
{
	if (inList->empty())
		return;

	if (inList->white.count()==1){
		outList->append(*inList);
		return;
	}

	qreal dist = m_frames->fstFrameDiag() + 1.0;
	int nPock = -1; int nSide = -1;
	int nByDist = 0;
	int n = 0;

	TwinListIteratorPtr<BallTraj> iter;
	Twin<BallTraj> item;
	iter.newObjects(*inList);

	while(iter.white->hasNext())
	{
		item = iter.next();
		if ((item.victim.pocketBallnPocket!=nPock)||(item.victim.nSideHits+item.white.nSideHits!=nSide)){
			if ((nPock!=-1)&&(nSide!=-1))
			{
				outList->append(inList->at(nByDist));
			}
			nPock = item.victim.pocketBallnPocket;
			nSide = item.victim.nSideHits+item.white.nSideHits;
			dist = item.victim.pocketBallDistToCenter;
			nByDist = n;
		}
		else{
			if (item.victim.pocketBallDistToCenter < dist)
			{
				dist = item.victim.pocketBallDistToCenter;
				nByDist = n;
			}
		}
		n++;
	}
	outList->append(inList->at(nByDist));

	iter.deleteObjects();
}

void Numeric2b::removeUndueBySideAndLength(const TwinList<BallTraj>* wvInList, QList<BallTraj>* mp_whiteBallTraj, QList<BallTraj>* mp_victimBallTraj)
{
	TwinListIteratorPtr<BallTraj> iter(*wvInList);
	double maxlength = -1; double length = 0;
	double maxside = -1; double sideh = 0;
	Twin<BallTraj> item;

	int k = 0; int index = -1;
	while(iter.victim->hasNext())
	{
		item = iter.next();
		sideh = item.white.nSideHits + item.victim.nSideHits;
		length = item.white.length() + item.victim.length();
		if ((sideh>maxside)||((sideh==maxside)&&(length>maxlength))){
			if (index!=-1){
				mp_whiteBallTraj->append(wvInList->white.at(index));
				mp_victimBallTraj->append(wvInList->victim.at(index));
			}
			maxlength = length;
			maxside = sideh;
			index = k;
		}
		else{
			mp_whiteBallTraj->append(item.white);
			mp_victimBallTraj->append(item.victim);
		}
		k++;
	}
}