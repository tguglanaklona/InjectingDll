#include "TipsPainter.h"

CursorPixmap::CursorPixmap(int r, BallType type): QPixmap(2*r+1,2*r+1)
{
	fill(Qt::transparent);
	TipsPainter p(this);
	p.drawBall(Ball(QPoint(r,r), r, type, 0));
}

QColor TipsPainter::ballTypeColor(const BallType type)
{
	if (type == white)
		return Qt::blue;
	else if (type == victim)
		return Qt::yellow; 
	else if (type == obstruction)
		return Qt::red;
	else if (type == stepping)
		return Qt::green;
	else
		return Qt::black;
}

void TipsPainter::setCircledPointPen(const BallType type)
{
	static QPen pen(Qt::white);
	pen.setColor(ballTypeColor(type));
	pen.setWidth(1); //2
	setPen(pen);
}

Qt::PenStyle TipsPainter::trajStyle(int queue_number)
{
	switch(queue_number){
	case(0):
		{
			return Qt::SolidLine;
		}
	case(1):
		{
			return Qt::DashLine;
		}
	case(2):
		{
			return Qt::DotLine;
		}
	case(3):
		{
			return Qt::DashDotLine;
		}
	case(4):
		{
			return Qt::DashDotDotLine;
		}
	case(5):
		{
			return Qt::CustomDashLine;
		}
	default:
		{
			return Qt::SolidLine;
		}

	}
}

void TipsPainter::setTipTrajectoryPen(const BallTraj& traj)
{
	setTipTrajectoryPen(traj, 0);
}

void TipsPainter::setTipTrajectoryPen(const BallTraj& traj, int queue_number) 
{
	QColor color;
	static QPen pen(Qt::black);
	if (traj.whitePocket) {
		color = Qt::black;
	}
	else{
		(traj.incorrectness == 1) ? color = Qt::red : color = ballTypeColor(traj.type);
		(traj.incorrectness == 2) ? color = QColor(255, 127, 0) : color = ballTypeColor(traj.type);
	}
	pen.setStyle(trajStyle(queue_number));
	pen.setColor(color);
	pen.setWidth(1);
	setPen(pen);
}

void TipsPainter::eraseRect(const int x, const int y, const int w, const int h)
{	
	setCompositionMode(QPainter::CompositionMode_Source);
	fillRect(x,y,w,h, Qt::transparent);
}

void TipsPainter::eraseRect(const QRect& rect)
{	
	setCompositionMode(QPainter::CompositionMode_Source);
	fillRect(rect, Qt::transparent);
}

void TipsPainter::repaintLine(const QPoint& pntbgn, const QPoint& pntend, const QPoint& prevpntbgn, const QPoint& prevpntend)
{
	int leftx = prevpntbgn.x();
	if (prevpntend.x() < prevpntbgn.x())
		leftx = prevpntend.x();
	int topy = prevpntbgn.y();
	if (prevpntend.y() > prevpntbgn.y())
		topy = prevpntend.y();
	eraseRect(QRect(leftx, topy, abs(prevpntend.x() - prevpntbgn.x()), abs(prevpntend.y() - prevpntbgn.y())));

	drawLine(pntbgn, pntend);
}

void TipsPainter::drawBall(const Ball& ball, bool drawCenter)
{
	QPen old = pen();
	setCircledPointPen(ball.type); 

	drawEllipse(round(ball.center),int(ball.radius),int(ball.radius));

	if (drawCenter){
		double koeff = 0.5;
		drawLine(QPoint(round(ball.center.x() + koeff*ball.radius), round(ball.center.y())), QPoint(round(ball.center.x() - koeff*ball.radius), round(ball.center.y())));
		drawLine(QPoint(round(ball.center.x()), round(ball.center.y() - koeff*ball.radius)), QPoint(round(ball.center.x()), round(ball.center.y() + koeff*ball.radius)));
	}

	setPen(old);
}

void TipsPainter::drawCueTip(const Ball& wb, const Vector& vector)
{
	QPen old = pen();
	setPen(Qt::red);

	drawLine(round(wb.center.x()), round(wb.center.y()), round(wb.center.x() + vector.x()), round(wb.center.y() + vector.y()));

	setPen(old);
}

void TipsPainter::draw1stFrame(const Frame& f)
{
	drawRect(f.fstFrameRect);
}

void TipsPainter::draw2ndFrame(const Frame& f)
{
	drawRect(f.sndFrameRect);
}

void TipsPainter::drawHoleChamfers(const HoleFrame& chamf)
{
	drawLine(chamf.chamferLine(HoleFrame::right));
	drawLine(chamf.chamferLine(HoleFrame::left));
}

void TipsPainter::drawHoleChamfers(const PocketFrame& pf)
{
	for (int i=1; i<=6; i++)
		drawHoleChamfers(*(pf.getpHoleFrame(i)));
}

void TipsPainter::drawChosenPocket(int cp, const PocketFrame& pf)
{
	if ((cp<1)||(cp>6))
		return; //error

	QPen oldpen = pen(); 
	setPen(Qt::yellow);
	QLine line1 = pf.getpHoleFrame(cp)->chamferLine(HoleFrame::right);
	QLine line2 = pf.getpHoleFrame(cp)->chamferLine(HoleFrame::left);
	drawLine(line1.p1(),line2.p2()); 
	drawLine(line1.p2(),line2.p1()); 
	setPen(oldpen);
}

void TipsPainter::drawBallTraj(const BallTraj& traj) 
{
	drawBallTraj(traj, 0);
}

void TipsPainter::drawBallTraj(const BallTraj& traj, int queue_number) 
{
	if ((traj.trList.emptyTraj())||(traj.trList.count()==1))
		return; //error

	QPen old = pen();
	setTipTrajectoryPen(traj, queue_number);

	QListIterator<NaturalPoint> iter(traj.trList);
	QRealPoint item2 = iter.next();
	QRealPoint item1(0,0);
	while(iter.hasNext())
	{
		item1 = item2;
		item2 = iter.next();
		drawLine(round(item1), round(item2));
	}

	if (traj.type!=stepping)
		drawBall(Ball(round(item2),traj.radius,traj.type,0), true);

	if (!traj.velMarks.empty())
		drawVelocityDots(traj);

	setPen(old);
}

void TipsPainter::drawVelocityDots(const BallTraj& traj)
{
	QListIterator<Mark> it(traj.velMarks);
	while(it.hasNext())
	{
		Mark item = it.next();
		if (!item.isNull())
		{
			drawEllipse(round(item), 2,2); 
			drawEllipse(round(item), 3,3); 
			//drawBall(Ball(round(item),traj.radius,traj.type,0), true);
		}
	}
}

