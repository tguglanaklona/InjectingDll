#ifndef GRAPHICSOBJ_H
#define GRAPHICSOBJ_H

#include <math.h>
#include <QPoint>
#include <QSize>
#include <qline>
#include <qwidget>
#include <qrect>
#include <qvector2d>
#include "misc.h"
#ifndef HEADERS_H
#include "headers.h"
#endif

enum BallType {white, victim, obstruction, stepping};
enum PocketType {corner, central};
enum BallColor {undefinedColor, whiteColor, blackColor, zonatedColor, selfColor};

class Ball
{
public:
	Ball(): center(QPoint(0,0)), radius(10), type(white), color(undefinedColor), recognRadius(10){};
	Ball(QRealPoint c, double r, BallType t, int rr): center(c), radius(r), type(t), color(undefinedColor), recognRadius(rr){};
	bool operator==(const Ball& b) const;

	bool isValid() const;
	void clearBall();

	BallType type;
	QRealPoint center; 
	double radius;
	BallColor color;
	int recognRadius;

	Vector m_cueTip;		
};

class Frame
{
public:
	Frame();
	Frame(QSize sz, int frameDst);
	//Frame(const Frame& fr);

	int sndFrameCoord(WallPosition pos) const;
	QPoint fstFrameCorner(int) const;
	QPoint sndFrameCorner(int) const;
	long double fstFrameDiag() const;
	long double sndFrameDiag() const;

	void set2ndFrameDst(int);
	int get2ndFrameDst() const;
	void set1stFrameSz(QSize);
	QSize get1stFrameSz() const;

public:
	QRect fstFrameRect;
	QRect sndFrameRect;
	QLine fstFrameVertCentr;
	QLine sndFrameVertCentr;

private:
	//void resetAttrib(const Frame& fr);
	void resetAttrib(QSize sz, int frameDst);

private:
	QSize fstFrameSize; 
	int sndFrameDst;
};

class HoleFrame: public Frame 
{		   
public:
	HoleFrame(const Frame& frame, int w, int n, PocketType t): Frame(frame), chamf_width(w), number(n), type(t){};
	//HoleFrame(const HoleFrame& cpy);

	enum ChamferPosition{right, left};

	QLine chamferLine(ChamferPosition) const;
	bool isValid() const;

	bool isPntNear(const QPoint&) const;
	QPoint CenterInCorner(qreal radius) const;

	void setFrame(const Frame& f);
	void setChamfWidth(int w);

	QPoint FstFramePoint() const;
	QPoint SndFramePoint() const;  
	
private:
	int number;
	int chamf_width; 
	PocketType type;
};

class PocketFrame : public Frame
{
public:
	PocketFrame();
	PocketFrame(const Frame& f, int w);
	//PocketFrame(const PocketFrame& cpy);
	~PocketFrame();

	HoleFrame* getpHoleFrame(int) const;
	bool isValid() const;
	int isPntNear(const QPoint&) const;

	void setPocketFrame(const Frame& f, int w);
	Frame* getpFrame();

protected:
	void setFrame(const Frame& f);
	void setChamfWidth(int w);
	
private:
	HoleFrame* pHoleFrame[6];
};

class Trajectory : public QList<NaturalPoint>
{
public:
	Trajectory(QList<NaturalPoint>& lst, int l): QList<NaturalPoint>(lst), length(l){};
	Trajectory():QList<NaturalPoint>(), length(0){clearTraj();};

	void append(const QRealPoint& t);
	void clearTraj();
	bool emptyTraj() const;
	qreal getLength() const{return length;};
	
	void setLength(int l);

private:
	qreal length;
};

class Mark: public NaturalPoint
{
public:
	Mark():NaturalPoint(),label(0){};
	Mark(NaturalPoint pnt, double l):NaturalPoint(pnt),label(l){};
	~Mark(){};

	double label;
};

class BallTraj: public Ball 
{
public:
	BallTraj();
	BallTraj(PocketFrame* ppf, Ball& moving_ball, const QVector2D initial_direction);
	~BallTraj();

	bool framesCrossing();
	Vector ballCrossing(const Ball& ball, bool isInterballGap);

	bool isBallPocket();
	qreal obstructedAtLength(const Ball& ball, bool isInterballGap) const;

	bool isValid() const;
	void clear();
	qreal length() const;

	PocketFrame* getpPF() const;
	Vector gets() const;
	long double MAX_LENGTH() const;

protected:
	bool frameCrossing(WallPosition pos);

public:
	Trajectory trList;
	QList<Mark> velMarks;

	int nSideHits;
	QList<long double> shotAngleCos;
	bool whitePocket;
	qreal pocketBallDistToCenter;
	int pocketBallnPocket;
	int incorrectness;

	QList<int> obs;
	int nObstructed;

	long double isGoToCos;

private:
	Vector s; 
	PocketFrame* pPF;
};

template <class T>
class Twin
{
public:
	Twin(){};
	Twin(const T& wtraj, const T& vtraj):white(wtraj), victim(vtraj){};
	~Twin(){};

	T white;
	T victim;
};

template <class T>
class TwinList
{
public:
	TwinList(){};
	TwinList(const QList<T>& wtraj, const QList<T>& vtraj):white(wtraj), victim(vtraj){};
	~TwinList(){};

	Twin<T> at(int i) const
	{
		return Twin<T>(white.at(i), victim.at(i));
	}

	Twin<T> last() const
	{
		return Twin<T>(white.last(), victim.last());
	}

	Twin<T> first() const
	{
		return Twin<T>(white.first(), victim.first());
	}

	void append(const Twin<T>& it)
	{
		white.append(it.white);
		victim.append(it.victim);
	}
	void append(const TwinList<T>& lt)
	{
		white.append(lt.white);
		victim.append(lt.victim);
	}

	void clear()
	{
		white.clear();
		victim.clear();
	}

	bool empty() const
	{
		return white.empty()||victim.empty();
	}

	int count() const
	{
		return white.count();
	}

	QList<T> white;
	QList<T> victim;
};

template <class T>
class TwinListIteratorPtr
{
public:
	TwinListIteratorPtr():white(NULL), victim(NULL){};
	TwinListIteratorPtr(const QList<T>& wList, const QList<T>& vList):white(NULL), victim(NULL){newObjects(wList, vList);};
	TwinListIteratorPtr(const TwinList<T>& list):white(NULL), victim(NULL){newObjects(list);};
	~TwinListIteratorPtr(){deleteObjects();};

	void deleteObjects()
	{
		if (white!=NULL){
			delete(white); white = NULL;
		}
		if (victim!=NULL){
			delete(victim); victim = NULL;
		}
	}

	void newObjects(const QList<T>& wList, const QList<T>& vList)
	{
		deleteObjects();
		white = new QListIterator<T>(wList);
		victim = new QListIterator<T>(vList);
	}
	void newObjects(const TwinList<T>& list)
	{
		newObjects(list.white, list.victim);
	}

	Twin<T> next()
	{
		return Twin<T>(white->next(), victim->next());
	}

	Twin<T> prev()
	{
		return Twin<T>(white->previous(), victim->previous());
	}

	QListIterator<T>* white;
	QListIterator<T>* victim;
};


#endif //GRAPHICSOBJ_H