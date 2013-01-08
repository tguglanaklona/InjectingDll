#ifndef RECOGNITION_H
#define RECOGNITION_H

#include <QPixmap>
#include <QDir>
#include <QPainter>
#ifndef GRAPHICSOBJ_H
#include "graphicsObj.h"
#endif GRAPHICSOBJ_H
#include <QVector3D>

#include "mathem.h"

#define WHITE_RING_SHIFT_FOR_RECOGNITION 2

enum WalkPointPos{bottomrightPos, rightPos, toprightPos, topPos, topleftPos, leftPos, bottomleftPos, bottomPos};

class WalkPoint : public QPoint
{
public:
	WalkPoint(const QPoint& point):QPoint(point){};
	WalkPoint(int x, int y):QPoint(x,y){};
	~WalkPoint(){};

	void next (int n, WalkPointPos pos); //non-const
	void nextGoCircle (const QPoint& center, int radius); //non-const
	void nextGoRect (const QRect& rect); //non-const
	bool outOfRect(const QRect& rect) const;
	QList<QPoint> allNeighbours(int xmin, int xmax, int ymin, int ymax) const;
	QList<QPoint> allNeighbours(const QRect& rect) const;
};

class FourietTransform;
class Fragment;

class MonoImage : public QImage
{
public:
	MonoImage(const QImage& i);
	MonoImage(int w, int h);
	~MonoImage(){};

	void complexData(float* out, int N1, int N2) const;
	void complexData(const QPoint& point, const QSize& siz, float* out, int N1, int N2) const;
	//double correlation(const MonoImage&, const QPoint&, const QSize&) const;
	double correlation(const Fragment& frag, const QPoint& thisOrigin) const;
	double correlationFouriet(FourietTransform* w, const FourietTransform* f_star) const;
	
	int isWhiteRing(const QPoint&, int) const;
	int isWhiteRingFull(const QPoint&, int) const;
	void drawWhiteCircle(const QPoint&, int);
};

class Fragment : public MonoImage
{
public:
	Fragment(const QSize& s):MonoImage(s.width(),s.height()), fragBegin(QPoint(0,0)), fragSize(s){};

	QPoint center()
	{
		if ((fragSize.width()%2!=0)&&(fragSize.height()%2!=0))
			return QPoint(0.5*(fragSize.width()-1),0.5*(fragSize.height()-1));
		else
			return QPoint(0,0); //error
	};

	QPoint fragBegin;
	QSize fragSize;
};

class FourietTransform
{
public:
	FourietTransform();
	~FourietTransform();
	
	void rewriteImageData(const MonoImage& image);
	void rewriteImageData(const MonoImage& image, const QPoint& point, const QSize& siz);
	
	void transform(bool inverse = false);
	void multiply(const FourietTransform* im);
	void conjugate();

	float getData(int i) const;

private:
	float* data;
	int datasz;
	unsigned long nn[3];

	void test();
};

class Recognition : public QPixmap
{
public:
	Recognition(const QPixmap& pixmap, QPixmap& backgroundPixmap);	
	Recognition(int w, int h, QPixmap& backgroundPixmap);
	Recognition(const QSize& size, QPixmap& backgroundPixmap);
	Recognition(QPixmapData* data, QPixmap& backgroundPixmap);
	~Recognition();

	QPoint ballCognition(int ballRecognRadius);//double radius);
	QList<Ball> allBallsCognition(double ballRadius, int ballRecognRadius);//double radius);
	Vector cueCognition(const Ball& localWhite);

	bool isWhiteColorDiscount;

private:
	QImage m_background;
	QImage m_image;

	QList<QPoint> corrMethodBallCogn(int radius, double* max);
	double correlateWithFrag(const MonoImage* pMono, const QPoint& monoOrigin) const;
	QList<QPoint> corrMethodAllMaximas(int radius);
	QPoint ballCorrectionAsConstRadius(const QList<QPoint>&, int radius) const;
	double correlationMax(int radius);
	bool makeMonoImage(bool fillArray);
	void detectBallType(Ball* ball) const;
	void makeCircleFrag(int ballRadius);

	Vector houghMethodCueCogn(const Ball& whiteBall);

	void debugWrite(const MonoImage&) const;
	int discrepancy(QList<QPoint>& list, QSize maxPointsValue);
	QSize ballFragmentSize(int ballRadius) const;

	MonoImage* m_pMono;
	Fragment* m_pFrag;
	QList<QPoint> m_lstnWhitePixels;
	float** m_ppField;
	bool m_isFouriet;
};

#endif //RECOGNITION_H