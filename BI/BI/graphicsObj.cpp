#include "graphicsObj.h"

bool Ball::isValid() const
{
	if (center==QRealPoint(0,0))
		return false;
	else
		return true;
}

bool Ball::operator==(const Ball& b) const
{
	return ((type==b.type)&&(center==b.center)&&(radius==b.radius));
}

void Ball::clearBall()
{
	center = QPoint(0,0);
	m_cueTip = Vector(0,0);
}

Frame::Frame(): fstFrameSize(QSize(0, 0)), sndFrameDst(0)
{
	fstFrameRect = QRect(0, 0, 0, 0);
	sndFrameRect = QRect(0, 0, 0, 0);
	fstFrameVertCentr = QLine(0, 0, 0, 0);
	sndFrameVertCentr = QLine(0, 0, 0, 0);
}

Frame::Frame(QSize sz, int frameDst): fstFrameSize(sz), sndFrameDst(frameDst)
{
	resetAttrib(sz, frameDst);
}
	
//Frame::Frame(const Frame& fr): fstFrameSize(fr.fstFrameSize), sndFrameDst(fr.sndFrameDst)
//{
//	resetAttrib(fr);
//}

//void Frame::resetAttrib(const Frame& fr)
//{
//	fstFrameRect = QRect(0, 0, fr.fstFrameSize.width(), fr.fstFrameSize.height());
//	QPoint d = QPoint(sndFrameDst, sndFrameDst);
//	sndFrameRect = QRect(fstFrameRect.topLeft() + d, fstFrameRect.bottomRight() - d);
//	fstFrameVertCentr = QLine(QPoint(ceil(0.5*float(fstFrameRect.width())),0), QPoint(ceil(0.5*float(fstFrameRect.width())),fstFrameRect.height()));
//	d = QPoint(0, fr.sndFrameDst);
//	sndFrameVertCentr = QLine(fstFrameVertCentr.p1() + d, fstFrameVertCentr.p2() - d);
//}

void Frame::resetAttrib(QSize sz, int frameDst)
{
	fstFrameRect = QRect(0, 0, sz.width()-1, sz.height()-1);
	QPoint d = QPoint(sndFrameDst, sndFrameDst);
	sndFrameRect = QRect(fstFrameRect.topLeft() + d, fstFrameRect.bottomRight() - d);
	fstFrameVertCentr = QLine(QPoint(ceil(0.5*float(fstFrameRect.width())),0), QPoint(ceil(0.5*float(fstFrameRect.width())),fstFrameRect.height()));
	d = QPoint(0, sndFrameDst);
	sndFrameVertCentr = QLine(fstFrameVertCentr.p1() + d, fstFrameVertCentr.p2() - d);
}

int Frame::sndFrameCoord(WallPosition pos) const
{
	switch(pos){
	case(Top):{
		return sndFrameRect.top();
			  }
			  break;
	case(Bottom):{
		return sndFrameRect.bottom();
			  }
			  break;
	case(Left):{
		return sndFrameRect.left();
			  }
			  break;
	case(Right):{
		return sndFrameRect.right();
			  }
			  break;
	}
	return 0;
}

QPoint Frame::fstFrameCorner(int n) const
{
	if ((n<1)||(n>4))
		return QPoint(-1,-1); //error

	switch(n){
	case(1):{
		return fstFrameRect.topLeft();
			}
	case(2):{
		return fstFrameRect.topRight();
			}
	case(3):{
		return fstFrameRect.bottomRight();
			}
	case(4):{
		return fstFrameRect.bottomLeft();
			}
	}

	return QPoint(-1,-1);
}

QPoint Frame::sndFrameCorner(int n) const
{
	if ((n<1)||(n>4))
		return QPoint(-1,-1); //error

	switch(n){
	case(1):{
		return sndFrameRect.topLeft();
			}
	case(2):{
		return sndFrameRect.topRight();
			}
	case(3):{
		return sndFrameRect.bottomRight();
			}
	case(4):{
		return sndFrameRect.bottomLeft();
			}
	}

	return QPoint(-1,-1);
}

long double Frame::fstFrameDiag() const
{
	return sqrt(long double (Squared(fstFrameSize.width())+Squared(fstFrameSize.height())) );
}

long double Frame::sndFrameDiag() const
{
	return sqrt(long double (Squared(sndFrameRect.width())+Squared(sndFrameRect.height())) );
}

void Frame::set2ndFrameDst(int d)
{
	sndFrameDst = d;
	resetAttrib(fstFrameSize, d);
}

int Frame::get2ndFrameDst() const
{
	return sndFrameDst;
}

void Frame::set1stFrameSz(QSize sz)
{
	fstFrameSize = sz;
	resetAttrib(sz, sndFrameDst);
}

QSize Frame::get1stFrameSz() const
{
	return fstFrameSize;
}

bool HoleFrame::isValid() const
{
	if (chamf_width==0)
		return false;
	else
		return true;
}

QPoint HoleFrame::FstFramePoint() const
{	
	if ((number<=0)||(number>6))
	{
		//error
	}
	switch(number)
	{
	case 1:
		return fstFrameRect.topLeft();
	case 2:
		return fstFrameVertCentr.p1();
	case 3:
		return fstFrameRect.topRight();
	case 4:
		return fstFrameRect.bottomRight();
	case 5:
		return fstFrameVertCentr.p2();
	case 6:
		return fstFrameRect.bottomLeft();
	}
	return QPoint(0,0);
}

QPoint HoleFrame::SndFramePoint() const
{
	if ((number<=0)||(number>6))
	{
		//error
	}
	switch(number)
	{
	case 1:
		return sndFrameRect.topLeft();
	case 2:
		return sndFrameVertCentr.p1();
	case 3:
		return sndFrameRect.topRight();
	case 4:
		return sndFrameRect.bottomRight();
	case 5:
		return sndFrameVertCentr.p2();
	case 6:
		return sndFrameRect.bottomLeft();
	}
	return QPoint(0,0);
}

QPoint HoleFrame::CenterInCorner(qreal radius) const
{
	Vector v(SndFramePoint().x()-FstFramePoint().x(), SndFramePoint().y()-FstFramePoint().y());
	switch(type){
	case(corner):{
		v.setX(sqrt(2.0)*radius*v.x()/v.length()); 
		v.setY(sqrt(2.0)*radius*v.y()/v.length());
				 }
				 break;
	case(central):{
		v.setX(radius*v.x()/v.length()); 
		v.setY(radius*v.y()/v.length());
				  }
				  break;
	}

	return QPoint(SndFramePoint().x()+v.x(), SndFramePoint().y()+v.y());
}

void HoleFrame::setFrame(const Frame& f)
{
	set2ndFrameDst(f.get2ndFrameDst());
	set1stFrameSz(f.get1stFrameSz());
}

void HoleFrame::setChamfWidth(int w)
{
	chamf_width = w;
}

QLine HoleFrame::chamferLine(ChamferPosition pos) const
{
	QPoint v = QPoint(0,0);

	switch(number)
	{
	case (1):{
		if (pos == right)
			v = QPoint(chamf_width, 0);
		else
			v = QPoint(0, chamf_width);
		   }
		break;
	case (2):{
		if (pos == right)
			v = QPoint(chamf_width, 0);
		else
			v = QPoint(-chamf_width, 0);
		   }
		break;
	case (3):{
		if (pos == right)
			v = QPoint(0, chamf_width);
		else
			v = QPoint(-chamf_width, 0);
		   }
		break;
	case (4):{
		if (pos == right)
			v = QPoint(-chamf_width, 0);
		else
			v = QPoint(0, -chamf_width);
		   }
		break;
	case (5):{
		if (pos == right)
			v = QPoint(-chamf_width, 0);
		else
			v = QPoint(chamf_width, 0);
		   }
		break;
	case (6):{
		if (pos == right)
			v = QPoint(0, -chamf_width);
		else
			v = QPoint(chamf_width, 0);
		   }
		break;
	}

	QLine ln45(FstFramePoint()+v, SndFramePoint()+v);
	return ln45; 
}

bool HoleFrame::isPntNear(const QPoint& point) const
{
	AreaPoint area = (0.5*(FstFramePoint() + SndFramePoint()));
	return area.isNear(point, 0.5*QVector2D(chamferLine(left).p2()-chamferLine(right).p2()).length());
}

PocketFrame::PocketFrame()
{
	for (int n=1; n<=6; n++)
	{
		if ((n!=2)&&(n!=5))
			pHoleFrame[n-1] = new HoleFrame(Frame(), 0, 0, corner);
		else
			pHoleFrame[n-1] = new HoleFrame(Frame(), 0, 0, central);
	}
}

PocketFrame::PocketFrame(const Frame& f, int w): Frame(f)
{
	for (int n=1; n<=6; n++)
	{
		if ((n!=2)&&(n!=5))
			pHoleFrame[n-1] = new HoleFrame(f, w, n, corner);
		else
			pHoleFrame[n-1] = new HoleFrame(f, w, n, central);
	}
}

void PocketFrame::setFrame(const Frame& f)
{
	for (int n=1; n<=6; n++)
		pHoleFrame[n-1]->setFrame(f);
	
	set2ndFrameDst(f.get2ndFrameDst());
	set1stFrameSz(f.get1stFrameSz());
}

Frame* PocketFrame::getpFrame()
{
	return (Frame*) this;
}

void PocketFrame::setChamfWidth(int w)
{
	for (int n=1; n<=6; n++)
		pHoleFrame[n-1]->setChamfWidth(w);
}

void PocketFrame::setPocketFrame(const Frame& f, int w)
{
	setFrame(f);
	setChamfWidth(w);
}

PocketFrame::~PocketFrame()
{
	for (int n=1; n<=6; n++)
		if (pHoleFrame[n-1]) delete(pHoleFrame[n-1]);
}

HoleFrame* PocketFrame::getpHoleFrame(int n) const
{
	if ((n<1)||(n>6))
		return NULL; //error

	return pHoleFrame[n-1];
}

bool PocketFrame::isValid() const
{
	for (int i=1;i<=6;i++)
	{
		if (pHoleFrame[i-1]==NULL)
			return false;
		if (!(pHoleFrame[i-1]->isValid()))
			return false;
	}
	return true;
}

int PocketFrame::isPntNear(const QPoint& pnt) const
{
	int i=0; bool b = false;	
	do
	{
		if (pHoleFrame[i]->isValid()){
			b = pHoleFrame[i]->isPntNear(pnt);
			if (b) return i+1;
		}
		else{
			return 0; //error
		}
		i++;
	} while ((i<6)&&(!b));

	return 0;
}

BallTraj::BallTraj(): Ball()
{
	pPF = NULL;
	s = Vector(0,0);
	nSideHits = 0;
	pocketBallDistToCenter = 0;
	pocketBallnPocket = 0;
	nObstructed = -1;
	whitePocket = false;
	incorrectness = 0;
	isGoToCos = -1;
}

BallTraj::BallTraj(PocketFrame* ppf, Ball& b, const QVector2D v): Ball(b), pPF(ppf), s(v), 
	pocketBallDistToCenter(0), pocketBallnPocket(0), nSideHits(0), nObstructed(-1),
	whitePocket(false), incorrectness(0), isGoToCos(-1)
{
	if ((!b.isValid())||(v.isNull()))
		return; //error

	trList.clearTraj();
	trList.append(b.center);
	shotAngleCos.append(-1);
}

BallTraj::~BallTraj()
{
}

bool BallTraj::framesCrossing()
{
	switch( s.inIV() ){
	case(I):
		{
			if (frameCrossing(Top)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Top); // change s
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
			if (frameCrossing(Right)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Right); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
		}
		break;
	case(II):
		{
			if (frameCrossing(Right)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Right); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
			if (frameCrossing(Bottom)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Bottom); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
		}
		break;
	case(III):
		{
			if (frameCrossing(Bottom)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Bottom); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
			if (frameCrossing(Left)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Left); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
		}
		break;
	case(IV):
		{
			if (frameCrossing(Left)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Left); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
			if (frameCrossing(Top)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Top); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
		}
		break;
	case(zero):
		{
			if (frameCrossing(Right)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Right); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
		}
		break;
	case(half_pi):
		{
			if (frameCrossing(Top)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Top); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
		}
		break;
	case(pi):
		{
			if (frameCrossing(Left)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Left); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
		}
		break;
	case(threehalf_pi):
		{
			if (frameCrossing(Bottom)) {
				if ( isBallPocket() ) {
					return true;
				}
				else {
					s.wallMirror(Bottom); 
					shotAngleCos.append(-1);
					nSideHits++;
					return false;
				}
			}
		}
		break;
	}

	trList.setLength(MAX_LENGTH());
	return false; //error or out of sndFrame
}

long double BallTraj::MAX_LENGTH() const
{
	return 2.5*pPF->sndFrameDiag();
}

bool BallTraj::frameCrossing(WallPosition pos)
{
	const double eps_ex = 1e-2;

	if (pos == Top)
	{
		Line topline(QRealPoint(center.x(), center.y() - radius), s);
		QRealPoint ipt = topline.crossY(pPF->sndFrameCoord(Top));
		Number<double> x(ipt.x());
		if ( x.in(pPF->sndFrameCoord(Left)+radius - eps_ex, pPF->sndFrameCoord(Right)-radius + eps_ex) ){
			Line ln(center, ipt + QRealPoint(0,radius));
			center = QRealPoint(ipt.x(), ipt.y() + radius); //change center
			trList.append(center); // add Point and length
			return true;
		}
		else{
			return false;
		}
	}
	else if (pos == Bottom)
	{
		Line bottomline(QRealPoint(center.x(), center.y() + radius), s);
		QRealPoint ipt = bottomline.crossY(pPF->sndFrameCoord(Bottom));
		Number<double> x(ipt.x());
		if ( x.in(pPF->sndFrameCoord(Left)+radius - eps_ex, pPF->sndFrameCoord(Right)-radius + eps_ex) ){
			Line ln(center, ipt - QRealPoint(0,radius));
			center = QRealPoint(ipt.x(), ipt.y() - radius);
			trList.append(center);
			return true;
		}
		else{
			return false;
		}
	}
	else if (pos == Left)
	{
		Line leftline(QRealPoint(center.x() - radius, center.y()), s);
		QRealPoint ipt = leftline.crossX(pPF->sndFrameCoord(Left));
		Number<double> y(ipt.y());
		if ( y.in( pPF->sndFrameCoord(Top)+radius - eps_ex, pPF->sndFrameCoord(Bottom)-radius + eps_ex) ){
			Line ln(center, ipt + QRealPoint(radius,0));
			center = QRealPoint(ipt.x() + radius, ipt.y());
			trList.append(center);
			return true;
		}
		else{
			return false;
		}
	}
	else if (pos == Right)
	{
		Line rightline(QRealPoint(center.x() + radius, center.y()), s);
		QRealPoint ipt = rightline.crossX(pPF->sndFrameCoord(Right));
		Number<double> y(ipt.y());
		if ( y.in( pPF->sndFrameCoord(Top)+radius - eps_ex, pPF->sndFrameCoord(Bottom)-radius + eps_ex) ){
			Line ln(center, ipt - QRealPoint(radius,0));
			center = QRealPoint(ipt.x() - radius, ipt.y());
			trList.append(center);
			return true;
		}
		else{
			return false;
		}
	}
	else
	{
		return false;
	}
}

qreal BallTraj::obstructedAtLength(const Ball& ball, bool isInterballGap) const
{
	BallTraj traj(*this);

	if (traj.ballCrossing(ball, isInterballGap) == Vector(0,0))
		return -1; //false
	else
		return traj.length(); //true
}

Vector BallTraj::ballCrossing(const Ball& ball, bool isInterballGap)
{
	Line centerline(center, s);
	
	QRealPoint intersect = centerline.cross(Line(ball.center, QVector2D(-s.y(),s.x())));
	long double dist = sqrt(long double((intersect.x()-ball.center.x())*(intersect.x()-ball.center.x()) + (intersect.y()-ball.center.y())*(intersect.y()-ball.center.y())));

	if (dist>2*radius + (isInterballGap ? INTERBALL_GAP : 0))
		return Vector(0,0); //error

	Vector vecway(intersect.x()-center.x(),intersect.y()-center.y());
	if (!s.oneWay(vecway))
		return Vector(0,0); //error

	long double ro = sqrt( long double((2*radius + (isInterballGap ? INTERBALL_GAP : 0))*(2*radius + (isInterballGap ? INTERBALL_GAP : 0))) - dist*dist );
	long double sv = sqrt( long double(s.x()*s.x()) + long double(s.y()*s.y()) );
	Vector newballcenter =  Vector(intersect) - ro*Vector(s.x(), s.y())/sv;

	QRealPoint oldcenter(center);
	center.setX(round(newballcenter.x())); center.setY(round(newballcenter.y())); //change center 

	Vector res(ball.center.x() - newballcenter.x(), ball.center.y() - newballcenter.y());

	Vector olds(s);
	s.setX(res.y()); s.setY(-res.x()); //change s
	if ( olds.scalarproduct(s) < 0 )
	{
		s.setX(-res.y()); s.setY(res.x());
	}
	long double angleCos = abs(olds.scalarproduct(res))/(olds.length()*res.length());

	if ((isGoToCos!=-1)&&(abs(isGoToCos-angleCos)>10*EPS_EX))
		return Vector(0,0);

	shotAngleCos.append(angleCos);
	trList.append(center); // add Point
	
	return res;
} 

bool BallTraj::isBallPocket()
{
	//corner

	if (center.y() <= pPF->sndFrameCoord(Top) + radius + EPS_EX)
	{
		if ( center.x() <= (pPF->getpHoleFrame(1))->chamferLine(HoleFrame::ChamferPosition::right).p2().x() - radius )
		{
			pocketBallDistToCenter = abs(pPF->getpHoleFrame(1)->CenterInCorner(radius).x() - center.x());
			pocketBallnPocket = 1;
			return true;
		}
		if ( center.x() >= (pPF->getpHoleFrame(3))->chamferLine(HoleFrame::ChamferPosition::left).p2().x() + radius )
		{
			pocketBallDistToCenter = abs(pPF->getpHoleFrame(3)->CenterInCorner(radius).x() - center.x());
			pocketBallnPocket = 3;
			return true;
		}
	}

	if (center.y() >= pPF->sndFrameCoord(Bottom) - radius - EPS_EX)
	{
		if ( center.x() <= (pPF->getpHoleFrame(6))->chamferLine(HoleFrame::ChamferPosition::left).p2().x() - radius )
		{
			pocketBallDistToCenter = abs(pPF->getpHoleFrame(6)->CenterInCorner(radius).x() - center.x());
			pocketBallnPocket = 6;
			return true;
		}
		if ( center.x() >= (pPF->getpHoleFrame(4))->chamferLine(HoleFrame::ChamferPosition::right).p2().x() + radius )
		{
			pocketBallDistToCenter = abs(pPF->getpHoleFrame(4)->CenterInCorner(radius).x() - center.x());
			pocketBallnPocket = 4;
			return true;
		}
	}

	if (center.x() <= pPF->sndFrameCoord(Left) + radius + EPS_EX)
	{
		if ( center.y() <= (pPF->getpHoleFrame(1))->chamferLine(HoleFrame::ChamferPosition::left).p2().y() - radius )
		{
			pocketBallDistToCenter = abs(pPF->getpHoleFrame(1)->CenterInCorner(radius).y() - center.y());
			pocketBallnPocket = 1;
			return true;
		}
		if ( center.y() >= (pPF->getpHoleFrame(6))->chamferLine(HoleFrame::ChamferPosition::right).p2().y() + radius )
		{
			pocketBallDistToCenter = abs(pPF->getpHoleFrame(6)->CenterInCorner(radius).y() - center.y());
			pocketBallnPocket = 6;
			return true;
		}
	}

	if (center.x() >= pPF->sndFrameCoord(Right) - radius - EPS_EX)
	{
		if ( center.y() <= (pPF->getpHoleFrame(3))->chamferLine(HoleFrame::ChamferPosition::right).p2().y() - radius )
		{
			pocketBallDistToCenter = abs(pPF->getpHoleFrame(3)->CenterInCorner(radius).y() - center.y());
			pocketBallnPocket = 3;
			return true;
		}
		if ( center.y() >= (pPF->getpHoleFrame(4))->chamferLine(HoleFrame::ChamferPosition::left).p2().y() + radius )
		{
			pocketBallDistToCenter = abs(pPF->getpHoleFrame(4)->CenterInCorner(radius).y() - center.y());
			pocketBallnPocket = 4;
			return true;
		}
	}

	//central

	if (center.y() <= pPF->sndFrameCoord(Top) + radius + EPS_EX)
	{
		if (Number<double>(s.anglex()).in(0.25*PI, 0.75*PI))
			if ( Number<int>(center.x()).in( (pPF->getpHoleFrame(2))->chamferLine(HoleFrame::ChamferPosition::left).p1().x() + 1.25*radius, 
											 (pPF->getpHoleFrame(2))->chamferLine(HoleFrame::ChamferPosition::right).p1().x() - 1.25*radius ) )
			{
				pocketBallDistToCenter = abs(pPF->getpHoleFrame(2)->SndFramePoint().x() - center.x());
				pocketBallnPocket = 2;
				return true;
			}
	}

	if (center.y() >= pPF->sndFrameCoord(Bottom) - radius - EPS_EX)
	{
		if (Number<double>(s.anglex()).in(PI+0.25*PI, PI+0.75*PI))
			if ( Number<int>(center.x()).in( (pPF->getpHoleFrame(5))->chamferLine(HoleFrame::ChamferPosition::right).p1().x() + 1.25*radius, 
											 (pPF->getpHoleFrame(5))->chamferLine(HoleFrame::ChamferPosition::left).p1().x() - 1.25*radius ) )
			{
				pocketBallDistToCenter = abs(pPF->getpHoleFrame(5)->SndFramePoint().x() - center.x());
				pocketBallnPocket = 5;
				return true;
			}
	}

	return false;
}

bool BallTraj::isValid() const
{
	if ((!s.isNull())&&(!trList.emptyTraj()))
		return true;
	else
		return false;
}

void BallTraj::clear()
{
	s = Vector(0,0);
	trList.clearTraj();
	clearBall();
	nObstructed = -1;
	obs.clear();

	pPF = NULL;
	nSideHits = 0;
	pocketBallDistToCenter = 0;
	pocketBallnPocket = 0;
}

qreal BallTraj::length() const
{
	return trList.getLength();
}

PocketFrame* BallTraj::getpPF() const
{
	return pPF;
}

Vector BallTraj::gets() const
{
	return s;
}

void Trajectory::clearTraj()
{
	clear();
	length = 0;
}

bool Trajectory::emptyTraj() const
{
	return empty();
}

void Trajectory::setLength(int l)
{
	length = l;
}

void Trajectory::append(const QRealPoint& t)
{
	if (count()>0)
	{
		Line ln(last(), t);
		qreal link = sqrt(double(ln.dx()*ln.dx() + ln.dy()*ln.dy()));
		length = length + link; // add length
		QList<NaturalPoint>::append(NaturalPoint(t,length));
	}
	else
	{
		QList<NaturalPoint>::append(NaturalPoint(t,0.0));
	}
}
