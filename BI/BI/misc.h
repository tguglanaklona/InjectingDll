#ifndef MISC_H
#define MISC_H

#include <math.h>
#include <QPoint>
#include <qline>
#include <qvector2d>
#include <qmatrix>

#define PI 3.1415926535897932384626433832795
#define MIN_ANGLE PI/3600 //3600
#define EPS_EX 1E-4

enum WallPosition{Top, Bottom, Left, Right};

class AreaPoint: public QPoint
{
public:
	AreaPoint(const QPoint& point);

	bool isNear(const QPoint& pnt) const;
	bool isNear(const QPoint& pnt, int eps) const;

protected:
	const int eps_pixels;
};

enum Quarto{I, II, III, IV, zero, half_pi, pi, threehalf_pi};

class Vector : public QVector2D
{
public:
	Vector():QVector2D(0, 0){};
	Vector(qreal x, qreal y):QVector2D(x, y){};
	Vector(const QPoint& point):QVector2D(point){};
	Vector(const QVector2D v):QVector2D(v){};
	void setVector(const QPoint& point){setX(point.x()); setY(point.y());};

	void wallMirror(WallPosition); //nonconst
	qreal scalarproduct(const Vector&) const;
	qreal modvectorproduct(const Vector&) const;
	void rotate(long double angle_rad); //nonconst
	long double anglex() const;
	bool oneWay(const Vector&) const;

	Quarto inIV() const;
	bool isNull() const;

private:
	long double cos_alpha() const;
	long double cos_beta() const;
	long double vectorLength() const;
	long double cosanglex() const;
};

class QRealPoint : public QVector2D
{
public:
	QRealPoint(): QVector2D(){};
	QRealPoint(const QPoint& p): QVector2D(p.x(),p.y()){};
	QRealPoint(const QRealPoint& p): QVector2D(p.x(),p.y()){};
	QRealPoint(qreal x, qreal y): QVector2D(x,y){};
};

class NaturalPoint : public QRealPoint
{
public:
	NaturalPoint(): QRealPoint(0,0), naturalLength(0){};
	NaturalPoint(const QRealPoint& p, double l): QRealPoint(p.x(),p.y()), naturalLength(l){};
	NaturalPoint(qreal x, qreal y, double l): QRealPoint(x,y), naturalLength(l){};

	bool isNull(){ return (QRealPoint::isNull())&&(naturalLength==0); }
	double naturalLength;
};

int round(double v);
QPoint round(const QRealPoint& p);
double pow10(int decimals);

class Line : public QLine 
{
public:
	Line(const QLine& ln);
	Line(const QPoint& pt1, const QPoint& pt2);
	Line(const QRealPoint& pt1, const QRealPoint& pt2);
	Line(int x1, int y1, int x2, int y2);
	Line(const QPoint& pt1, const QVector2D& ss);
	Line(const QRealPoint& pt1, const QVector2D& ss);

	QRealPoint crossY(int yval) const;
	QRealPoint crossX(int xval) const;
	QRealPoint cross(const Line& ln) const;

	//long double distance(const QPoint& pnt) const;

	QVector2D getS() const;
	QRealPoint getPt() const;

private:
	QVector2D s;
	QRealPoint pt;

public:
	friend bool operator||(const Line&, const Line&); 
};

template <class Type>
class Number 
{
public:
	Number(Type i):value(i){};

	bool in(Type a, Type b) const
	{
		if ((value>=a)&&(value<=b))
			return true;
		else
			return false;
	};

	bool isIntegerPowerOf(int n) const
	{
		if (value<n)
			return false;

		long double x = log(long double(value))/log(long double(n));
		if (abs(long double(round(x)) - x) < EPS_EX)
			return true;
		
		return false;
	};

	int findCellNumber(const QList<Type>& list) const
	{
		//dichotomy
		int maxi = list.count()-1;
		int mini = 0;
		int i = 0;

		while (abs(maxi-mini)!=1)
		{
			i = 0.5*(maxi+mini);
			if (value<list.at(i))
				maxi = i;
			else
				mini = i;
		}

		return mini;
	};
	
	Type value;

public:
	bool operator==(const QList<Type>& lst)
	{
		QListIterator<Type> it(lst);
		while(it.hasNext())
		{
			if (it.next() == value)
				return true;
		}

		return false;
	};

	bool operator!=(const QList<Type>& lst)
	{
		return !operator==(lst);
	};

	Number<int> operator+(Type a)
	{
		return value + a;
	};

	Number<int> operator*(Type a)
	{
		return value * a;
	};
};

template<class T>
T Max (const T &x, const T &y) 
{
    return ( x > y ) ? x : y;
}

template<class T>
T Min (const T &x, const T &y) 
{
    return ( x < y ) ? x : y;
}

template<class T>
T Squared (const T &x) 
{
    return x*x;
}

int increment(long inc[], long size);
template<class T>
void shellSort(T a[], long size);

#endif //MISC_H