#ifndef TIPSPAINTER_H
#define TIPSPAINTER_H

#include <QtGui>
#include "graphicsObj.h"
#ifndef DISTANCE_H
#include "distance.h"
#endif DISTANCE_H

class TipsPainter : public QPainter
{
public:
	TipsPainter(QPaintDevice* pd): QPainter(pd){};

	void drawBall(const Ball& ball, bool drawCenter = false);

	void draw1stFrame(const Frame& f);
	void draw2ndFrame(const Frame& f);
	void drawHoleChamfers(const HoleFrame&);
	void drawHoleChamfers(const PocketFrame&);
	void drawChosenPocket(int, const PocketFrame&);

	void drawBallTraj(const BallTraj&);
	void drawBallTraj(const BallTraj& traj, int queue_number);

	void drawCueTip(const Ball& wb, const Vector& vector);
	void drawVelocityDots(const BallTraj& traj);

protected:
	void repaintLine(const QPoint& pntbgn, const QPoint& pntend, const QPoint& prevpntbgn, const QPoint& prevpntend);

	void eraseRect(const int x, const int y, const int w, const int h);
	void eraseRect(const QRect& rect);

	QColor ballTypeColor(const BallType type);
	Qt::PenStyle trajStyle(int queue_number);
	void setTipTrajectoryPen(const BallTraj& traj);
	void setTipTrajectoryPen(const BallTraj& traj, int queue_number);

public:
	void setCircledPointPen(const BallType type);
	
};

class CursorPixmap : public QPixmap
{
public:
	CursorPixmap(int, BallType);

};

#endif //TIPSPAINTER_H