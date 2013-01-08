#include "recognition.h"

void WalkPoint::next (int n, WalkPointPos pos)
{
	if (n<1)
		return; //error

	switch(pos)
	{
	case(bottomrightPos):
		{
			setX(x()+n);
			setY(y()+n);
		}
		break;
	case(toprightPos):
		{
			setX(x()+n);
			setY(y()-n);
		}
		break;
	case(topleftPos):
		{
			setX(x()-n);
			setY(y()-n);
		}
		break;
	case(bottomleftPos):
		{
			setX(x()-n);
			setY(y()+n);
		}
		break;
	case(rightPos):
		{
			setX(x()+n);
		}
		break;
	case(topPos):
		{
			setY(y()-n);
		}
		break;
	case(leftPos):
		{
			setX(x()-n);
		}
		break;
	case(bottomPos):
		{
			setY(y()+n);
		}
		break;
	default:
		{
			//error
		}
		break;
	}
}

bool WalkPoint::outOfRect(const QRect& rect) const
{
	if ( (!(Number<qreal>(x()).in(rect.left(),rect.right()))) ||
		(!(Number<qreal>(y()).in(rect.top(),rect.bottom()))) )
		 return true;
	else
		 return false;
}

void WalkPoint::nextGoRect (const QRect& rect)
{
	if ((x()==rect.left()) && ((y()>=rect.top())&&(y()<rect.bottom())))
		setY(y()+1);
	else if ((x()==rect.right()) && ((y()>rect.top())&&(y()<=rect.bottom())))
		setY(y()-1);
	else if ((y()==rect.bottom()) && ((x()>=rect.left())&&(x()<rect.right())))
		setX(x()+1);
	else if ((y()==rect.top()) && ((x()>rect.left())&&(x()<=rect.right())))
		setX(x()-1);
	else
		return;
}

void WalkPoint::nextGoCircle (const QPoint& center, int radius)
{
	if (*this == center) //first
	{
		next(radius, rightPos);
		return;
	}

	Vector vector(*this - center);
	Number<double> phi = vector.anglex();
	if ((phi.in(1.75*PI, 2*PI) || phi.in(0, 0.25*PI))&&(phi.value!=0.25*PI))
	{
		int yval = 1;
		int xval = int(sqrt(radius*radius - double(yval*yval)));
		setX(center.x()+xval);
		setY(y()-yval);
	}
	else if (phi.in(0.25*PI, 0.75*PI) && phi.value!=0.75*PI)
	{
		int xval = 1;
		int yval = int(sqrt(radius*radius - double(xval*xval)));
		setX(x()-xval);
		setY(center.y()-yval);
	}
	else if (phi.in(0.75*PI, 1.25*PI) && phi.value!=1.25*PI)
	{
		int yval = 1;
		int xval = int(sqrt(radius*radius - double(yval*yval)));
		setX(center.x()-xval);
		setY(y()+yval);
	}
	else if (phi.in(1.25*PI, 1.75*PI) && phi.value!=1.75*PI)
	{
		int xval = 1;
		int yval = int(sqrt(radius*radius - double(xval*xval)));
		setX(x()+xval);
		setY(center.y()+yval);
	}
	else
	{
	}
}

QList<QPoint> WalkPoint::allNeighbours(const QRect& rect) const
{
	return allNeighbours(rect.left(), rect.right(), rect.top(), rect.bottom());
}

QList<QPoint> WalkPoint::allNeighbours(int xmin, int xmax, int ymin, int ymax) const
{
	if ((x()<xmin)||(x()>xmax)||(y()<ymin)||(y()>ymax))
		return QList<QPoint>();

	QList<QPoint> list;

	if (x()-1>=xmin)
		list.append(QPoint(x()-1,y()));
	if (x()+1<=xmax)
		list.append(QPoint(x()+1,y()));
	if (y()-1>=ymin){
		list.append(QPoint(x(),y()-1));
		if (x()-1>=xmin)
			list.append(QPoint(x()-1,y()-1));
		if (x()+1<=xmax)
			list.append(QPoint(x()+1,y()-1));
	}
	if (y()+1<=ymax){
		list.append(QPoint(x(),y()+1));
		if (x()-1>=xmin)
			list.append(QPoint(x()-1,y()+1));
		if (x()+1<=xmax)
			list.append(QPoint(x()+1,y()+1));
	}

	return list;
}

Recognition::Recognition(const QPixmap& pixmap, QPixmap& backgroundPixmap):QPixmap(pixmap), isWhiteColorDiscount(false), 
	m_pMono(NULL), m_ppField(NULL), m_pFrag(NULL), m_isFouriet(false)
{
	if ((pixmap.isNull())||(backgroundPixmap.isNull()))
		return; 

	//if (backgroundPixmap.size()!=pixmap.size())
	//	return;

	m_background = backgroundPixmap.toImage();
	m_image = this->toImage();
}

Recognition::Recognition(int w, int h, QPixmap& backgroundPixmap):QPixmap(w,h), isWhiteColorDiscount(false), 
	m_pMono(NULL), m_ppField(NULL), m_pFrag(NULL), m_isFouriet(false)
{
	if (((w==0)&&(h==0))||(backgroundPixmap.isNull()))
		return; 

	m_background = backgroundPixmap.toImage();
	m_image = this->toImage();
}

Recognition::Recognition(const QSize& size, QPixmap& backgroundPixmap):QPixmap(size), isWhiteColorDiscount(false), 
	m_pMono(NULL), m_ppField(NULL), m_pFrag(NULL), m_isFouriet(false)
{
	if ((size == QSize(0,0))||(backgroundPixmap.isNull()))
		return; 

	m_background = backgroundPixmap.toImage();
	m_image = this->toImage();
}

Recognition::Recognition(QPixmapData* data, QPixmap& backgroundPixmap):QPixmap(data), isWhiteColorDiscount(false), 
	m_pMono(NULL), m_ppField(NULL), m_pFrag(NULL), m_isFouriet(false)
{
	if ((data==NULL)||(backgroundPixmap.isNull()))
		return; 

	m_background = backgroundPixmap.toImage();
	m_image = this->toImage();
}

Recognition::~Recognition()
{
	if (m_pMono!=NULL) {delete m_pMono; m_pMono=NULL;}
	if (m_pFrag!=NULL) {delete(m_pFrag); m_pFrag=NULL;}
}

int Recognition::discrepancy(QList<QPoint>& list, QSize maxPointsValue)
{
	QPoint min(maxPointsValue.width()*2, maxPointsValue.height()*2), max(-1,-1);
	for (int i=0;i<list.count();i++)
	{
		QPoint center = list.at(i);
		if (center.x()<min.x())
			min.setX(center.x());
		if (center.y()<min.y())
			min.setY(center.y());
		if (center.x()>max.x())
			max.setX(center.x());
		if (center.y()>max.y())
			max.setY(center.y());
	}

	return qMax(max.x() - min.x(), max.y() - min.y());
}

double Recognition::correlateWithFrag(const MonoImage* pMono, const QPoint& monoOrigin) const
{
	if (m_pFrag==NULL)
		return 0; //error

	double max = 0;

	FourietTransform f_star;
	if (m_isFouriet){
		f_star.rewriteImageData(*pMono, monoOrigin, m_pFrag->fragSize);
		f_star.transform(); 
		f_star.conjugate();
	
		FourietTransform w;
		w.rewriteImageData(*m_pFrag, m_pFrag->fragBegin, m_pFrag->fragSize);
		w.transform();
		max = pMono->correlationFouriet(&w, &f_star);
	}
	else{
		max = pMono->correlation(*m_pFrag, monoOrigin);
	}

	return max;
}

QSize Recognition::ballFragmentSize(int ballRadius) const
{
	return QSize(2*ballRadius+1, 2*ballRadius+1); //2*int+1
}

void Recognition::detectBallType(Ball* ball)  const
{
	if (!m_pFrag)
		return; //error
	
	//detecting all white (here maybe all black)
	QPoint cnt = m_pFrag->center();
	int halfwidth = cnt.x();
	int halfheight = cnt.y();

	int h, s, l; 
	QColor value;
	double countw = 0;
	int k = 0;
	int xfrag = 0;
	QPoint center = round(ball->center); //added
	for (int x=center.x()-halfwidth; x<=center.x()+halfwidth; x++){
		int yfrag = 0;
		for (int y=center.y()-halfheight; y<=center.y()+halfheight; y++)
		{
			if (m_pFrag->pixelIndex(xfrag, yfrag)==1)
			{
				value = m_image.pixel(x,y);
				value.getHsl(&h, &s, &l);
				
				if ((s<170)&&(l>30)){
						countw++;
				}
				k++;
			}
			yfrag++;
		}
		xfrag++;
	}
	countw = countw/k;

	if (countw>0.8){
		ball->type = white;
		ball->color = whiteColor;
	}
	else{
		ball->type = obstruction;
	}
}

QList<QPoint> Recognition::corrMethodAllMaximas(int ballRadius) 
{
	if ((m_background.size())!=m_image.size())
		return QList<QPoint>(); //error
	if ((2*ballRadius+1>ballFragmentSize(ballRadius).width())||(2*ballRadius+1>ballFragmentSize(ballRadius).height()))
		return QList<QPoint>(); //error

	int width = m_image.width();
	int height = m_image.height();
	QSize cycle_size(width-ballFragmentSize(ballRadius).width(), height-ballFragmentSize(ballRadius).height());

	QList<QPoint> result;
	QList<double> resultmax;

	makeCircleFrag(ballRadius);
	if (m_pFrag==NULL)
		return QList<QPoint>(); //error

	double supremum = correlationMax(ballRadius); 
	
	if (makeMonoImage(true)) 
	{
		if (m_lstnWhitePixels.empty())
			return QList<QPoint>(); //error
		if (m_ppField!=NULL){
			for (int i=width-1;i>=0;i--)
				delete [](m_ppField[i]);
			delete []m_ppField;
		}
		m_ppField = new float*[width];
		for (int i=0;i<width;i++){
			m_ppField[i] = new float[height];
			for (int j=0; j<height; j++)
				m_ppField[i][j] = 0.0;
		}

		QListIterator<QPoint> nWhite(m_lstnWhitePixels);
		double corr = 0;
		while(nWhite.hasNext())
		{
			QPoint centerPoint = nWhite.next();
			QPoint cornerPoint(centerPoint.x() - ballRadius, centerPoint.y() - ballRadius);  
			if ((Number<int>(cornerPoint.y()).in(0,cycle_size.height()))&&(Number<int>(cornerPoint.x()).in(0,cycle_size.width())))
			{
				corr = correlateWithFrag(m_pMono, cornerPoint); 
				if (corr>0.8*supremum)
					m_ppField[centerPoint.x()][centerPoint.y()] = corr;
			}
		}

		//maxima suppression
		for (int k=0; k<m_lstnWhitePixels.count(); k++)
		{
			WalkPoint central(m_lstnWhitePixels[k]);
			if (m_ppField[central.x()][central.y()]!=0)
			{
				QList<QPoint> lstWhiteNeighbours = central.allNeighbours(0,width,0,height);
				QListIterator<QPoint> ngh(lstWhiteNeighbours);
				bool max = true;
				while (ngh.hasNext())
				{
					QPoint neighbour = ngh.next();
					if (m_ppField[central.x()][central.y()]<m_ppField[neighbour.x()][neighbour.y()]){	
						max = false;
						break;
					}
				}
				if (max)
				{
					resultmax.append(m_ppField[central.x()][central.y()]);
					result.append(central);
				}
			}
		}

		for (int i=width-1;i>=0;i--)
			delete [](m_ppField[i]);
		delete []m_ppField;
	}

	return result;
}

void Recognition::makeCircleFrag(int ballRadius)
{
	if (m_pFrag!=NULL) {delete(m_pFrag); m_pFrag=NULL;}
	m_pFrag = new Fragment(ballFragmentSize(ballRadius));
	m_pFrag->drawWhiteCircle(m_pFrag->center(), ballRadius);
}

Vector Recognition::cueCognition(const Ball& localWhite)
{
	return houghMethodCueCogn(localWhite);
}

QList<Ball> Recognition::allBallsCognition(double ballRadius, int ballRecognRadius)//double ballRadius)
{
	m_isFouriet = false;
	QList<QPoint> locals = corrMethodAllMaximas(ballRecognRadius);//round(ballRadius));
	
	if (locals.empty())
		return QList<Ball>(); //error

	makeCircleFrag(ballRecognRadius);//round(ballRadius));
	if (m_pFrag==NULL)
		return QList<Ball>(); //error

	QList<Ball> listlocals;
	for (int k=0;k<locals.count();k++)
	{
		//correction as const radius
		int z = m_pMono->isWhiteRingFull(locals[k], ballRecognRadius);//int(ballRadius)-1); 
		if (z==0) 
		{
			Ball ball(locals[k], ballRadius, obstruction, ballRecognRadius);
			detectBallType(&ball);
			listlocals.append(ball);
		}
	}

	return listlocals;
}

QPoint Recognition::ballCognition(int ballRecognRadius)//double ballRadius) 
{
	//int intradius = round(ballRadius); //a bit more is better
	double temp = 0;
	m_isFouriet = false;
	QList<QPoint> localcenter = corrMethodBallCogn(ballRecognRadius, &temp);//intradius, &temp);

	if (localcenter.empty())
		return QPoint(0,0); //error
	if (localcenter.count()==1)
		return localcenter.at(0);

	if (discrepancy(localcenter, m_pMono->size())>0.25*ballRecognRadius)//ballRadius)
		return QPoint(0,0); //error

	return ballCorrectionAsConstRadius(localcenter, ballRecognRadius);//int(ballRadius));
}

QPoint Recognition::ballCorrectionAsConstRadius(const QList<QPoint>& list, int radius) const
{
	if ((list.empty())||(!m_pMono))
		return QPoint(0,0); //error
	if (list.count()==1)
		return list.at(0);

	int k=0;
	int min = 2*qMax(m_pMono->size().width(), m_pMono->size().height());
	for (int i=0;i<list.count();i++)
	{
		QPoint center = list.at(i);
		int nonwhite = m_pMono->isWhiteRingFull(center, radius); //m_pMono->isWhiteRing(center,radius);
		if (nonwhite == 0)
			return center;
		if (nonwhite<min){
			min = nonwhite;
			k = i;
		}
	}

	return list.at(k); //warning
}

bool Recognition::makeMonoImage(bool fillArray) 
{	
	//this function includes isWhiteColorDiscount

	int rv, gv, bv, rbv, gbv, bbv; 
	QColor value, bckvalue;
	bool allblack = true;

	int w = m_image.width();
	int h = m_image.height();

	if (fillArray){
		if (!m_lstnWhitePixels.empty())
			m_lstnWhitePixels.clear();
	}

	if (m_pMono!=NULL) {delete m_pMono; m_pMono=NULL;}
	m_pMono = new MonoImage(w,h);
	m_pMono->fill(0); QPoint p(0,0);
	do
	{
		value = m_image.pixel(p);
		bckvalue = m_background.pixel(p);
		value.getRgb(&rv, &gv, &bv);
		bckvalue.getRgb(&rbv, &gbv, &bbv);
		if ((rv!=rbv)||(gv!=gbv)||(bv!=bbv)) {
			if ((!isWhiteColorDiscount)||((isWhiteColorDiscount)&&((rv!=255)||(gv!=255)||(bv!=255)))){
				m_pMono->setPixel(p,1);
				if (fillArray){
					m_lstnWhitePixels.append(p);
				}
				allblack = false;
			}
		}

		p.setX(p.x()+1);
		if (p.x()==m_pMono->width()) {
			p.setX(0);
			p.setY(p.y()+1);
		}
	} while(p.y()<m_pMono->height());

	return !allblack;
}

Vector Recognition::houghMethodCueCogn(const Ball& whiteBall)
{
	if ((m_background.size())!=m_image.size())
		return QPoint(); //error
	
	QPoint center = round(whiteBall.center);
	if (m_pFrag!=NULL) {delete(m_pFrag); m_pFrag=NULL;}
	m_pFrag = new Fragment(m_image.size());
	m_pFrag->drawWhiteCircle(center, round(whiteBall.radius));

	//углы разбиени€ области
	WalkPoint goCircle(center);
	int walkRadius = qMin(qMin(center.x(), m_image.width() - center.x()), qMin(center.y(), m_image.height() - center.y()));
	QList<long double> angles;
	
	goCircle.nextGoCircle(center, walkRadius);
	Vector v(goCircle-center);
	long double phi = v.anglex();
	angles.append(phi);
	do
	{
		goCircle.nextGoCircle(center, walkRadius);
		v.setX(goCircle.x()-center.x());
		v.setY(goCircle.y()-center.y());
		phi = v.anglex();
		if (phi!=0)
			angles.append(phi);
		else
			angles.append(2.0*PI);
	}while(phi!=0);

	//€чейки накоплени€
	int cellsz = angles.count()-1; 
	int* cell = new int[cellsz];
	for (int i=0;i<cellsz;i++)
		cell[i] = 0;

	QColor value, bckvalue; 
	int rv, bv, gv, rbv, gbv, bbv;
	int ncell = 0;
	for (int x=0; x<m_image.width(); x++)
	{
		for (int y=0; y<m_image.height(); y++)
		{
			if (m_pFrag->pixelIndex(x,y)!=1)
			{
				QPoint p = QPoint(x,y);
				value = m_image.pixel(p);
				bckvalue = m_background.pixel(p);
				value.getRgb(&rv, &gv, &bv);
				bckvalue.getRgb(&rbv, &gbv, &bbv);
				if ((rv!=rbv)||(gv!=gbv)||(bv!=bbv)) {
					if ((rv>250)&&(gv>250)&&(bv>250)) { //white pixel lies outside the ball
						
						Number<long double> phi = Vector(QRealPoint(p)-whiteBall.center).anglex();
						ncell = phi.findCellNumber(angles);
						cell[ncell]++; //+ соседи?
					}
				}
			}
		}
	}

	int max = 0; int maxi = 0;
	for (int i=0;i<cellsz;i++){
		if (cell[i]>max){
			max = cell[i];
			maxi = i;
		}
	}

	phi = angles.at(maxi);
	delete(cell);

	if (max==0)
		return Vector(0,0);
	else
		return Vector(long double(walkRadius)*cos(phi), -long double(walkRadius)*sin(phi));
}

double Recognition::correlationMax(int radius)
{
	if (m_pFrag==NULL)
		return 0.0;

	return correlateWithFrag(m_pFrag, m_pFrag->fragBegin);	
}

QList<QPoint> Recognition::corrMethodBallCogn(int radius, double* max)
{
	if ((m_background.size())!=m_image.size())
		return QList<QPoint>(); //error

	if (!makeMonoImage(false))
		return QList<QPoint>();
	
	if (m_pFrag!=NULL) {delete(m_pFrag); m_pFrag=NULL;}
	m_pFrag = new Fragment(m_image.size());

	FourietTransform f_star;
	if (m_isFouriet){
		f_star.rewriteImageData(*m_pMono, QPoint(0,0), m_pMono->size());
		f_star.transform(); 
		f_star.conjugate();
	}

	*max = 0; 
	QList<QPoint> maxp; 
	for (int x=radius; x<=m_image.width()-radius-1; x++) 
	{
		for (int y=radius; y<=m_image.height()-radius-1; y++)
		{
			m_pFrag->drawWhiteCircle(QPoint(x,y),radius);

			double m = 0;
			if (m_isFouriet){
   				FourietTransform w;
				w.rewriteImageData(*m_pFrag, QPoint(0,0), m_pFrag->size());
				w.transform();
				m = m_pMono->correlationFouriet(&w, &f_star);
			}
			else{
				m = m_pMono->correlation(*m_pFrag, m_pFrag->fragBegin);
			}

			if (m!=0) {
				if (m>*max){
					*max = m;
					maxp.clear();
					maxp.append(QPoint(x,y));
				}
				else if (m==*max){
					maxp.append(QPoint(x,y));
				}
				else{
				}
			}
		}
	}

	return maxp;
}

void Recognition::debugWrite(const MonoImage& im) const
{
	QFile debugFile("debug.log");
	debugFile.open(QIODevice::WriteOnly);
	QDataStream debug(&debugFile);
	QVector<QRgb> table = im.colorTable();
	for (int i=0;i<im.width();i++)
	{
		for (int j=0;j<im.height();j++)
		{
			QRgb rgb = im.color(im.pixelIndex(i,j));
			QString c;
			if (rgb == table.at(0))
				c = QString("0 ");
			else
				c = QString("1 ");
			debug << c;	
		}
		debug << "\n";
	}
	debugFile.close();
}

FourietTransform::FourietTransform() :data(NULL)
{
	datasz = 0;
	for (int i=0;i<3;i++)
		nn[i] = -1;
}

FourietTransform::~FourietTransform()
{
	if (data!=NULL) {delete []data; data = NULL;}
}

void FourietTransform::test()
{
	float REAL[8]; float IMAG[8];
	REAL[0]=  1.000;   IMAG[0]=  0.000;
	REAL[1]=  0.000;   IMAG[1]=  0.000;
	REAL[2]=  0.000;   IMAG[2]=  0.000;
	REAL[3]=  0.000;   IMAG[3]=  0.000;
	REAL[4]=  0.000;   IMAG[4]=  0.000;
	REAL[5]=  0.000;   IMAG[5]=  0.000;
	REAL[6]=  0.000;   IMAG[6]=  0.000;
	REAL[7]=  0.000;   IMAG[7]=  0.000;

	int num = 8; // 8 = 2^3

	int h = num; int w = num;
	datasz = 2*num*num+1;

	if (data!=NULL) {delete []data; data = NULL;}
	data = new float[datasz];

	int k = 1; data[0] = 0.0f;
	for (int i=0;i<num;i++){
		for (int j=0;j<num;j++){
			if (k==1)
				data[k] = 1.0f;
			else
				data[k] = 0.0f;
			k++;
			data[k] = 0.0f;
			k++;
		}
	}

	nn[0] = 0;
	nn[1] = num;
	nn[2] = num;

	fourn(data, nn, 2, 1);

	nn[0] = 0;
}

void FourietTransform::rewriteImageData(const MonoImage& image)
{
	nn[0] = 0;
	Number<int> h = image.size().height();
	Number<int> w = image.size().width();	

	if (h.value%2!=0)   
		h = h+1;
	if (w.value%2!=0)
		w = w+1;
	while(!h.isIntegerPowerOf(2))
		h = h+2;
	while(!w.isIntegerPowerOf(2))
		w = w+2;

	nn[1] = h.value; 
	nn[2] = w.value;
	datasz = 2*nn[1]*nn[2]+1; 
	
	if (data!=NULL) {delete []data; data = NULL;}
	data = new float[datasz];
	image.complexData(data, nn[1], nn[2]);	
}

void FourietTransform::rewriteImageData(const MonoImage& image, const QPoint& point, const QSize& siz)
{
	nn[0] = 0;
	Number<int> h = siz.height();
	Number<int> w = siz.width();	

	if (h.value%2!=0)   
		h = h+1;
	if (w.value%2!=0)
		w = w+1;
	while(!h.isIntegerPowerOf(2))
		h = h+2;
	while(!w.isIntegerPowerOf(2))
		w = w+2;

	nn[1] = h.value; 
	nn[2] = w.value;
	datasz = 2*nn[1]*nn[2]+1; 
	
	if (data!=NULL) {delete []data; data = NULL;}
	data = new float[datasz];
	image.complexData(point, siz, data, nn[1], nn[2]);	
}

void FourietTransform::transform(bool inverse)
{
	if (!data)
		return; //error

	fourn(data, nn, 2, (inverse)?-1:1);

	if (inverse){
		for (int i=0; i<datasz; i++) 
			data[i] = data[i]/((float)(nn[1]*nn[2]));
	}
}

void FourietTransform::conjugate()
{
	for (int i=2;i<datasz;i+=2)
		data[i] = -data[i];
}

void FourietTransform::multiply(const FourietTransform* im)
{
	if (!data)
		return; //error

	bool re = true;
	for (int i=1; i<=nn[1]*nn[2]; i++)
	{
		if (re)
			data[i] = data[i]*im->getData(i) - data[i+1]*im->getData(i+1);
		else
			data[i] = data[i+1]*im->getData(i) + im->getData(i+1)*data[i];
		re = !re;
	}
}

float FourietTransform::getData(int i) const
{
	if (i>=datasz)
		return 0;//error

	return data[i];
}

MonoImage::MonoImage(const QImage& i):QImage(i)
{
	if (i.format()!=QImage::Format_MonoLSB)
		return;
}

MonoImage::MonoImage(int w, int h):QImage(w,h,QImage::Format_MonoLSB)
{
}

double MonoImage::correlation(const Fragment& frag, const QPoint& thisOrigin) const
{
	double mult = 0; 
	int w = frag.fragSize.width(); int h = frag.fragSize.height();
	int i = thisOrigin.x(); 
	for (int ifrag=frag.fragBegin.x();ifrag<frag.fragBegin.x()+w;ifrag++){
		int j = thisOrigin.y();
		for (int jfrag=frag.fragBegin.y();jfrag<frag.fragBegin.y()+h;jfrag++){
			if ((frag.pixelIndex(ifrag,jfrag))!=0){
				if ((pixelIndex(i,j))!=0)
					mult = mult + 1; 	
			}
			j++;
		}
		i++;
	}
	return mult;
}

/*double MonoImage::correlation(const MonoImage& im, const QPoint& begin, const QSize& siz) const
{
	double mult = 0; 
	int w = siz.width(); int h = siz.height();
	for (int i=begin.x();i<begin.x()+w;i++)
		for (int j=begin.y();j<begin.y()+h;j++)
			if ((im.pixelIndex(i,j))!=0)
				if ((pixelIndex(i,j))!=0)
					mult = mult + 1; 	
	return mult;
}*/

double MonoImage::correlationFouriet(FourietTransform* W, const FourietTransform* F_star) const
{
	W->multiply(F_star);   	
	W->transform(true);	   

	return W->getData(1);
}

void MonoImage::complexData(float* out, int N1, int N2) const
{
	int h = size().height();
	int w = size().width();	

	int k = 1; out[0] = 0.0f;
	for (int i=0;i<N2;i++){
		for (int j=0;j<N1;j++){
			if ((i<w)&&(j<h)){
				out[k] = pixelIndex(i,j); //re
				k++;
				out[k] = 0.0f;	//im
			}
			else{
				out[k] = 0.0f;
				k++;
				out[k] = 0.0f;
			}
			k++;
		}
	}
}

void MonoImage::complexData(const QPoint& point, const QSize& siz, float* out, int N1, int N2) const
{
	int h = siz.height();
	int w = siz.width();	

	int k = 1; out[0] = 0.0f;
	for (int i=point.x();i<point.x()+N2;i++){
		for (int j=point.y();j<point.y()+N1;j++){
			if ((i<point.x()+w)&&(j<point.y()+h)){
				out[k] = pixelIndex(i,j); //re
				k++;
				out[k] = 0.0f;	//im
			}
			else{
				out[k] = 0.0f;
				k++;
				out[k] = 0.0f;
			}
			k++;
		}
	}
}

int MonoImage::isWhiteRing(const QPoint& pnt, int radius) const
{
	if ((pnt.x()<0)||(pnt.x()>=size().width())||(pnt.y()<0)||(pnt.y()>=size().height()))
		return 0; //error

	QPoint center(pnt);
	
	int k = 0;
	for (int wpp = 1; wpp<8; wpp+=2){
		WalkPoint point(center);
		point.next(radius, static_cast<WalkPointPos>(wpp));
		if (pixelIndex(point)==0)
			k++;
	}
	for (int wpp = 0; wpp<8; wpp+=2){
		WalkPoint point(center);
		point.next(int(double(radius)*sqrt(2.0)/2.0), static_cast<WalkPointPos>(wpp));
		if (pixelIndex(point)==0)
			k++;
	}

	return k; //0==true
}

int MonoImage::isWhiteRingFull(const QPoint& pnt, int radius) const
{
	if ((pnt.x()<0)||(pnt.x()>=size().width())||(pnt.y()<0)||(pnt.y()>=size().height()))
		return 0; //error

	radius-=WHITE_RING_SHIFT_FOR_RECOGNITION;
	QPoint center(pnt);

	int k = 0;
	WalkPoint point(center);
	point.nextGoCircle(center, radius);
	Vector v(point-center);
	double phi = v.anglex();
	do
	{
		point.nextGoCircle(center, radius);
		v.setX(point.x()-center.x());
		v.setY(point.y()-center.y());
		phi = v.anglex();
		if (pixelIndex(point)==0)
			k++;
	}while(phi!=0);

	return k; //0==true
}

void MonoImage::drawWhiteCircle(const QPoint& point, int radius)
{
	fill(0);
	QPainter painter(this);
	painter.setPen(Qt::white);
	painter.setBrush(QBrush(Qt::white));
	painter.drawEllipse(point,radius,radius);
}
