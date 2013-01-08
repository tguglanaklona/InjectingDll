#include "misc.h"

AreaPoint::AreaPoint(const QPoint& point): QPoint(point), eps_pixels(5)
{
}

bool AreaPoint::isNear(const QPoint& pnt, int eps) const
{
	if (eps<0)
		return false; //error

	if (( abs(pnt.x()-(*this).x()) <= 2*eps ) && ( abs(pnt.y()-(*this).y()) <= 2*eps ))
		return true;
	else
		return false;
}

bool AreaPoint::isNear(const QPoint& pnt) const
{
	return isNear(pnt, eps_pixels);
}

Quarto Vector::inIV() const
{
	if (isNull())
		return zero; //error

	if ((abs(y())<double(EPS_EX)) || (abs(x())<double(EPS_EX)))
	{
		if (abs(y())<double(EPS_EX)){
			if (x()>0)
				return zero;
			else
				return pi;
		}
		if (abs(x())<double(EPS_EX)){
			if (y()<0)
				return half_pi;
			else
				return threehalf_pi;
		}
	}

	if ((x()>0)&&(y()<0))
		return I;
	if ((x()>0)&&(y()>0))
		return II;
	if ((x()<0)&&(y()>0))
		return III;
	if ((x()<0)&&(y()<0))
		return IV;

	return zero; //error
}

bool Vector::isNull() const
{
	if (*this==Vector(0,0))
		return true;
	else
		return false;
}

Line::Line(const QLine& ln):QLine(ln), pt(ln.p1()){
	s.setX(ln.x2()-ln.x1()); s.setY(ln.y2()-ln.y1());
}

Line::Line(const QPoint& pt1, const QPoint& pt2):QLine(pt1, pt2), pt(pt1){
	s.setX(pt2.x()-pt1.x()); s.setY(pt2.y()-pt1.y());
}

Line::Line(const QRealPoint& pt1, const QRealPoint& pt2):QLine(round(pt1), round(pt2)), pt(pt1){
	s.setX(pt2.x()-pt1.x()); s.setY(pt2.y()-pt1.y());
}

Line::Line(int x1, int y1, int x2, int y2):QLine(x1, y1, x2, y2), pt(x1, y1){
	s.setX(x2-x1); s.setY(y2-y1);
}

Line::Line(const QPoint& pt1, const QVector2D& ss):QLine(pt1.x(), pt1.y(), pt1.x()+ss.x(), pt1.y()+ss.y()), pt(pt1){
	s = ss; 
}

Line::Line(const QRealPoint& pt1, const QVector2D& ss):QLine(round(pt1.x()), round(pt1.y()), round(pt1.x()+ss.x()), round(pt1.y()+ss.y())), pt(pt1){
	s = ss; 
}

bool operator||(const Line& ln1, const Line& ln2)
{
	qreal l1 = ln1.getS().length();
	qreal l2 = ln2.getS().length();

	if ((ln1.s/l1 == ln2.s/l2)||(ln1.s/l1 == -ln2.s/l2))
		return true;
	else
		return false;
}


QRealPoint Line::crossY(int yval) const //QRealPoint?
{
	QRealPoint resultPnt(-1,-1);

	// s{A,B}; t{B,-A}; B*x - A*y + C = 0

	double A = s.x();
	double B = s.y();
	double C = A*pt.y()-B*pt.x();
	
	double x = (A*yval-C)/B;

	return QRealPoint(x,yval);
}

QRealPoint Line::crossX(int xval) const
{
	QRealPoint resultPnt(-1,-1);

	// s{A,B}; t{B,-A}; B*x - A*y + C = 0

	double A = s.x();
	double B = s.y();
	double C = A*pt.y()-B*pt.x();

	double y = (B*xval+C)/A;

	return QRealPoint(xval,y);
}

QVector2D Line::getS() const
{
	return s;
}

QRealPoint Line::getPt() const
{
	return pt;
}

QRealPoint Line::cross(const Line& ln) const
{
	if (*this||ln)
		return QRealPoint(-1,-1); //error

	if ((s==QVector2D(0,0))||(ln.getS()==QVector2D(0,0)))
		return QRealPoint(-1,-1); //error

	// s{A,B}; t{B,-A}; B*x - A*y + C = 0
	// x = A1*y/B1 - C1/B1
	// x = A2*y/B2 - C2/B2
	
	double A1 = s.x();
	double B1 = s.y();
	double C1 = A1*pt.y()-B1*pt.x();

	double A2 = ln.getS().x();
	double B2 = ln.getS().y();
	double C2 = A2*ln.getPt().y()-B2*ln.getPt().x();

	double x = 0;
	double y = 0;
	
	if ((B1!=0)&&(B2!=0))
	{
		y = (C1*B2 - C2*B1)/(A1*B2 - A2*B1);
		x = (A1*y - C1)/B1;
	}
	else
	{
		x = (C2*A1-C1*A2)/(B1*A2-B2*A1);
		y = (C1 + B1*x)/A1;
	}

	return QRealPoint(x,y);
}

/*long double Line::distance(const QPoint& pnt) const
{
	long double dist = 0;

	// s{A,B};  

	Line t(pnt,QVector2D(-s.y(),s.x()));
	QRealPoint intersect = cross(t);

	return sqrt(long double((intersect.x()-pnt.x())*(intersect.x()-pnt.x()) + (intersect.y()-pnt.y())*(intersect.y()-pnt.y())));
}*/

void Vector::wallMirror(WallPosition pos)
{
	if ((pos==Left)||(pos==Right)){
		setX(-x());
	}
	else if ((pos==Top)||(pos==Bottom)){
		setY(-y());
	}
	else{
		//error
	}
}

qreal Vector::scalarproduct(const Vector& v) const
{
	return x()*v.x() + y()*v.y();
}

qreal Vector::modvectorproduct(const Vector& v) const
{
	return abs(x()*v.y() - y()*v.x());
}

long double Vector::anglex() const
{
	Quarto q = inIV();

	switch (q)
	{
	case(zero):
		{
			return 0;
		}
		break;
	case(pi):
		{
			return PI;
		}
		break;
	case(half_pi):
		{
			return 0.5*PI;
		}
		break;
	case(threehalf_pi):
		{
			return 1.5*PI;
		}
		break;
	case(I):
		{
			return acos(cos_alpha());
		}
		break;
	case(II):
		{
			return acos(cos_beta()) + 1.5*PI;
		}
		break;
	case(III):
		{
			return acos(-cos_alpha()) + PI;
		}
		break;
	case(IV):
		{
			return acos(-cos_beta()) + 0.5*PI;
		}
		break;
	}

	return 0; //error
}

long double Vector::cosanglex() const
{
	Quarto q = inIV();

	switch (q)
	{
	case(zero):
		{
			return 1.0;
		}
		break;
	case(pi):
		{
			return -1.0;
		}
		break;
	case(half_pi):
		{
			return 0;
		}
		break;
	case(threehalf_pi):
		{
			return 0;
		}
		break;
	case(I):
		{
			return cos_alpha();
		}
		break;
	case(II):
		{
			return sqrt(long double(1-cos_beta()*cos_beta()));
		}
		break;
	case(III):
		{
			return -cos_alpha();
		}
		break;
	case(IV):
		{
			return -sqrt(long double(1-cos_beta()*cos_beta()));
		}
		break;
	}

	return 0; //error
}

void Vector::rotate(long double angle_rad)
{
	if (angle_rad > 0.25*PI)
		return; //error

	long double cosa = cos(angle_rad);
	long double sina = sqrt(long double(1-cosa*cosa));
	QMatrix rotationMatrix(cosa, -sina, sina, cosa, 0, 0);
	qreal newx, newy;
	rotationMatrix.map(x(), y(), &newx, &newy);
	setX(newx); setY(newy);
}

long double Vector::cos_alpha() const
{
	return x()/vectorLength();
}

long double Vector::cos_beta() const
{
	return y()/vectorLength();
}

long double Vector::vectorLength() const
{
	return sqrt(long double(x()*x() + y()*y()));
}

bool Vector::oneWay(const Vector& v) const
{
	if (modvectorproduct(v)>double(10.0))//10*EPS_EX
		return false;

	if (((x()>0)&&(v.x()>0))||((x()<0)&&(v.x()<0))||((x()==0)&&(v.x()==0)))
		if (((y()>0)&&(v.y()>0))||((y()<0)&&(v.y()<0))||((y()==0)&&(v.y()==0)))
			return true;

	return false;
}

double pow10(int decimals)
{
	double res = 1;
	for (int i=0;i<decimals;i++)
		res = res*10;
	return res;
}

/*double Round(double v, int decimals){
	if( v == 0 )	
		return 0;
	int sgn =v<0?-1:1;
	double eps = sgn*pow10(decimals);	
	double lim = pow10(int(log10(sgn*v))-DBL_DIG);	
	double rmr = fmod( v, eps );	
	v -= rmr;
	if( sgn*rmr - sgn*(eps / 2)>-lim )
		v += eps;
	return v;
}*/

int round(double v){
	if (v>=0)
		return floor(v+0.5);
	else
		return -floor(-v+0.5);
}

QPoint round(const QRealPoint& p){
	return QPoint(round(p.x()), round(p.y()));
}



int increment(long inc[], long size) {
	// inc[] массив, в которые заносятся инкременты
	// size размерность этого массива
	int p1, p2, p3, s;

	p1 = p2 = p3 = 1;
	s = -1;
	do {// заполняем массив элементов по формуле Роберта Седжвика
		if (++s % 2) {
			inc[s] = 8*p1 - 6*p2 + 1;
		} else {
			inc[s] = 9*p1 - 9*p3 + 1;
			p2 *= 2;
			p3 *= 2;
		}
		p1 *= 2;
		// заполняем массив, пока текущая инкремента хотя бы в 3 раза меньше количества элементов в массиве
	} while(3*inc[s] < size);  

	return s > 0 ? --s : 0;// возвращаем количество элементов в массиве
}
 
template<class T>
void shellSort(T a[], long size) {
// inc инкремент, расстояние между элементами сравнения
// i и j стандартные переменные цикла
// seq[40] массив, в котором хранятся инкременты
  long inc, i, j, seq[40];
  int s;//количество элементов в массиве seq[40]
 
  // вычисление последовательности приращений
  s = increment(seq, size);
  while (s >= 0) {
        //извлекаем из массива очередную инкременту
        inc = seq[s--];
// сортировка вставками с инкрементами inc
    for (i = inc; i < size; i++) {
      T temp = a[i];
// сдвигаем элементы до тех пор, пока не дойдем до конца или не упорядочим в нужном порядке
      for (j = i-inc; (j >= 0) && (a[j] > temp); j -= inc)
        a[j+inc] = a[j];
// после всех сдвигов ставим на место j+inc элемент, который находился на i месте
      a[j+inc] = temp;
    }
  }
}