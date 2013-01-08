#include "TipsWnd.h"

#ifndef BI_H
#include "bi.h"
#endif BI_H

class Numeric2bActionWnd: public Numeric2bAction
{
public:
	Numeric2bActionWnd(TipsWnd* pClient): m_pClient(pClient) {};

	virtual bool isGap();
	virtual double impactAngle();

	TipsWnd* m_pClient;
};

bool Numeric2bActionWnd::isGap()
{
	return m_pClient->isInterballGap;
}

double Numeric2bActionWnd::impactAngle()
{
	return m_pClient->impactAngle;
}

TipsWnd::TipsWnd(QWidget* parent, TipsWndAction* act): QWidget(NULL), m_pAction(act),  
	offset(0,0), settFile("settings.txt"), m_wndMode(transparent), m_chamferWidth(0), 
	m_frames(Frame(size(), 0)), m_pocketFrame(m_frames,0), m_tipsMode(manual), m_pBackground(NULL),
	m_pBallPixmapRecogn(NULL), m_pAutoPixmapRecogn(NULL)
{
	moving = false;
	resizing = false;
	chosenWhite= false;
	chosenVictim = false;
	chosenPocket = false;
	chosenObstruction = false;
	chosenStepping = false;
	chosenSndStepping = false;
	chosenCue = false;
	chosenGoTo = false;
	isTrajsPaint = true;
	isColorDiscount = true;
	isInterballGap = false;
	impactAngle = 0.0;
	m_movePoint = QPoint(size().width()-1,0);
	m_resizePoint = QPoint(size().width()-1,size().height()-1);

	//transparency block
	setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);

	setMouseTracking (true);

	if (readSettings())
		m_pAction->EnableBISaveFramesButton(false);

	m_whiteBall.type = white;
	m_victimBall.type = victim;
	m_stepBall.type = stepping;
	m_sndStepBall.type = stepping;
	m_chosenPocket = 0;
	m_obstructionBall.clear();

	if (!loadBackground())
		m_pAction->EnableBIRecognitionMode(false);
}

TipsWnd::~TipsWnd()
{
	if (m_pBackground){
		delete(m_pBackground); m_pBackground = NULL;
	}
	if (m_pBallPixmapRecogn){
		delete(m_pBallPixmapRecogn); m_pBallPixmapRecogn = NULL;
	}
	if (m_pAutoPixmapRecogn){
		delete(m_pAutoPixmapRecogn); m_pAutoPixmapRecogn = NULL;
	}
}

bool TipsWnd::readSettFile(QPoint* pFramePt, QSize* pFrameSz)
{
	if (settFile.open(QIODevice::ReadOnly))
	{
		QDataStream frames(&settFile);
		int temp; int tempr; 
		frames >> *pFramePt >> *pFrameSz >> temp >> m_whiteBall.radius >> m_chamferWidth >> tempr >> isInterballGap;
		m_frames.set2ndFrameDst(temp);
		m_pocketFrame.setPocketFrame(m_frames, m_chamferWidth);
		m_victimBall.radius = m_whiteBall.radius;
		m_stepBall.radius = m_whiteBall.radius;
		m_sndStepBall.radius = m_whiteBall.radius;
		if (!m_obstructionBall.empty())
			for (int i=0;i<m_obstructionBall.count();i++)
				m_obstructionBall[i].radius = m_whiteBall.radius; 
		m_whiteBall.recognRadius = tempr;
		m_victimBall.recognRadius = tempr;
		m_stepBall.recognRadius = tempr;
		m_sndStepBall.recognRadius = tempr;
		if (!m_obstructionBall.empty())
			for (int i=0;i<m_obstructionBall.count();i++)
				m_obstructionBall[i].recognRadius = tempr;
		settFile.close();
		return true;
	}
	else
	{
		return false;
	}
	
}

bool TipsWnd::defaultSettingsVistaBook(QPoint* pFramePt, QSize* pFrameSz)
{
	{
		/*
		*pFramePt = QPoint(330, 191); 
		*pFrameSz = QSize(612, 303);
		m_frames.set2ndFrameDst(13);
		m_whiteBall.radius = 11.0;
		m_chamferWidth = 24;
		m_pocketFrame.setPocketFrame(m_frames, m_chamferWidth);
		*/

		*pFramePt = QPoint(331, 192);
		*pFrameSz = QSize(611, 301);
		m_frames.set2ndFrameDst(12);
		m_whiteBall.radius = 10.5;
		m_chamferWidth = 24;
		m_pocketFrame.setPocketFrame(m_frames, m_chamferWidth);
	}

	m_pocketFrame.setPocketFrame(m_frames, m_chamferWidth);
	m_victimBall.radius = m_whiteBall.radius;
	m_stepBall.radius = m_whiteBall.radius;
	m_sndStepBall.radius = m_whiteBall.radius;
	if (!m_obstructionBall.empty())
		for (int i=0;i<m_obstructionBall.count();i++)
			m_obstructionBall[i].radius = m_whiteBall.radius; 
	m_whiteBall.recognRadius = 10.0;
	m_victimBall.recognRadius = 10.0;
	m_stepBall.recognRadius = 10.0;
	m_sndStepBall.recognRadius = 10.0;
	if (!m_obstructionBall.empty())
		for (int i=0;i<m_obstructionBall.count();i++)
			m_obstructionBall[i].recognRadius = 10.0;

	return true;
}

void TipsWnd::writeSettFile(QPoint framePt, QSize frameSz, int SndFrameDst, double ballRadius, int holeWidth, int recognBallRadius, bool isIG)
{
	if (settFile.open(QIODevice::WriteOnly))
	{
		QDataStream frames(&settFile);
		frames << framePt << frameSz << SndFrameDst << ballRadius << holeWidth << recognBallRadius << isIG; 
		settFile.close();
	}
	else
	{
		//error
	}
	return;
}

void TipsWnd::saveSettings(int SndFrameDst, double ballRadius, int holeWidth, int recognBallRadius, bool isIG)
{
	writeSettFile(pos(), size(), SndFrameDst, ballRadius, holeWidth, recognBallRadius, isIG);
}

bool TipsWnd::readSettings()
{
	QPoint framePt;
	QSize frameSz;
	if (readSettFile(&framePt, &frameSz))
	//if (defaultSettingsVistaBook(&framePt, &frameSz)) 
	{
		m_pAction->secondFrameSpin_setValue(m_frames.get2ndFrameDst());
		m_pAction->ballSizeSpin_setValue(m_whiteBall.radius);
		m_pAction->holeWidthSpin_setValue(m_chamferWidth);
		m_pAction->ballRecognSizeSpin_setValue(m_whiteBall.recognRadius);
		m_pAction->interballGap_setChecked(isInterballGap);
		move(framePt);
		resize(frameSz);
		onResize();
		repaint();
		return true;
	}
	return false;
}

void TipsWnd::sndFrameDst_changeValue(int i)
{
	m_frames.set2ndFrameDst(i);
	m_pocketFrame.setPocketFrame(m_frames, m_chamferWidth);
	repaint();
	loadBackground();
}

void TipsWnd::ballRecognitionRadius_changeValue(double r)
{
	m_whiteBall.recognRadius = r;
	m_victimBall.recognRadius = r;
	m_stepBall.recognRadius = r;
	m_sndStepBall.recognRadius = r;
	if (!m_obstructionBall.empty())
		for (int i=0;i<m_obstructionBall.count();i++)
			m_obstructionBall[i].recognRadius = r;
}

void TipsWnd::ballRadius_changeValue(double r)
{
	m_whiteBall.radius = r;
	m_victimBall.radius = r;
	m_stepBall.radius = r;
	m_sndStepBall.radius = r;
	if (!m_obstructionBall.empty())
			for (int i=0;i<m_obstructionBall.count();i++)
				m_obstructionBall[i].radius = r; 
	if ((chosenWhite)||(chosenVictim)||(chosenObstruction)||(chosenStepping)||(chosenSndStepping)||(chosenCue))
		repaint();
}

void TipsWnd::holeWidth_changeValue(int w)
{
	m_chamferWidth = w;
	m_pocketFrame.setPocketFrame(m_frames, m_chamferWidth);
	repaint();
}

void TipsWnd::colorDiscountCheckBox_checked(int newstate)
{
	if (newstate == Qt::Unchecked)
		isColorDiscount = false;
	else
		isColorDiscount = true;
}

void TipsWnd::interBallGapCheckBox_checked(int newstate)
{
	if (newstate == Qt::Unchecked)
		isInterballGap = false;
	else
		isInterballGap = true;
}

void TipsWnd::impactAngle_changeValue(double value)
{
	impactAngle = value;
}

void TipsWnd::setManualTipsMode()
{
	m_tipsMode = manual;
	m_pAction->EnableBIAllBallsButton(false);
}

void TipsWnd::setSemimanualTipsMode()
{
	m_tipsMode = semimanual;
	m_pAction->EnableBIAllBallsButton(false);
}

void TipsWnd::setSemiautomaticTipsMode()
{
	m_tipsMode = semiautomatic;
	m_pAction->EnableBIAllBallsButton(true);
}

void TipsWnd::paintEvent( QPaintEvent* pev )
{
	TipsPainter p(this);

	//frames
	p.draw1stFrame(m_frames);
	p.draw2ndFrame(m_frames); 

	//holes  
	if (m_pocketFrame.isValid())
		p.drawHoleChamfers(m_pocketFrame);

	//window mode
	if ((m_wndMode == chooseWhiteBall)||(m_wndMode == chooseVictimBall)||(m_wndMode == choosePocket)||(m_wndMode == chooseObstructionBall)||(m_wndMode == chooseStepBall)||(m_wndMode == chooseSndStepBall)) {
		setCatchMouse(true, p); }
	else if (m_wndMode == transparent) {
		setCatchMouse(false, p); }
	else {
	}

	if (isTrajsPaint)
	{
		//chosen balls
		if (chosenWhite)
		{
			if (m_whiteBall.isValid())
				p.drawBall(m_whiteBall);
		}
		if (chosenVictim)
		{
			if (m_victimBall.isValid())
				p.drawBall(m_victimBall);
		}
		if (chosenStepping)
		{
			if (m_stepBall.isValid())
				p.drawBall(m_stepBall);
		}
		if (chosenSndStepping)
		{
			if (m_sndStepBall.isValid())
				p.drawBall(m_sndStepBall);
		}
		if (chosenObstruction)
		{
			if (!m_obstructionBall.empty())
				for (int i=0;i<m_obstructionBall.count();i++)
					if (m_obstructionBall[i].isValid())
						p.drawBall(m_obstructionBall[i]); 
		}

		//cue tip
		if (chosenCue)
		{
			if (!m_whiteBall.m_cueTip.isNull())
				p.drawCueTip(m_whiteBall, m_whiteBall.m_cueTip);
		}

		//chosen pocket
		if (chosenPocket)
		{
			if (m_chosenPocket!=0)
				p.drawChosenPocket(m_chosenPocket, m_pocketFrame);
		}

		//ball trajectory 
		if ((!m_whiteBallTraj.empty())&&(!m_victimBallTraj.empty()))
		{
			QListIterator<BallTraj> iterw(m_whiteBallTraj);
			QListIterator<BallTraj> iterv(m_victimBallTraj);
			QListIterator<BallTraj> iters(m_stepBallTraj);
			int i=0;
			while ((iterw.hasNext())&&(iterv.hasNext()))
			{
				p.drawBallTraj(iterw.next(), i);
				p.drawBallTraj(iterv.next(), i);
				if (!m_stepBallTraj.empty())
					p.drawBallTraj(iters.next(), i);
				i++;
			}
		}
	}

	return QWidget::paintEvent(pev);
}

void TipsWnd::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
    if ( (event->button() == Qt::LeftButton) && (AreaPoint(event->pos()).isNear(m_movePoint)) ) {
        moving = true;
        offset = event->pos();
    }
	if ( (event->button() == Qt::LeftButton) && (AreaPoint(event->pos()).isNear(m_resizePoint)) ) {
		resizing = true;
	}

	if (event->button() == Qt::LeftButton){

		switch(m_wndMode){
		case (chooseWhiteBall):
			{
				m_whiteBall.center = event->pos();

				if ((m_tipsMode==semimanual)||(m_tipsMode==semiautomatic)){
					QVector2D shift = setBallPixmapRecogn(m_whiteBall); 
					QPoint local = m_pBallPixmapRecogn->ballCognition(m_whiteBall.recognRadius);//.radius); 
					if (local!=QPoint(0,0)){
						QPoint point(local.x() + shift.x(), local.y() + shift.y());
						m_whiteBall = Ball(point,m_whiteBall.radius,m_whiteBall.type,m_whiteBall.recognRadius);
						
						if (m_tipsMode==semiautomatic)
							removeFromObstructions(point);
					}
					m_pAction->semimanualMousePress();
				}

				chosenWhite = true;
				m_pAction->EnableBIWhiteBallButton(true);
				if (!chosenVictim) {
					m_wndMode = chooseVictimBall;
					m_pAction->EnableBIVictimBallButton(false);
				}
				else {
					m_wndMode = transparent;
				}
				m_pAction->EnableBIObstructionButton(true);
				m_pAction->EnableBIDiscardButton(true);
			}
			break;
		case (chooseVictimBall):
			{
				m_victimBall.center = event->pos();

				if ((m_tipsMode==semimanual)||(m_tipsMode==semiautomatic)){
					QVector2D shift = setBallPixmapRecogn(m_victimBall);
					QPoint local = m_pBallPixmapRecogn->ballCognition(m_victimBall.recognRadius);//.radius);
					if (local!=QPoint(0,0)){
						QPoint point(local.x() + shift.x(), local.y() + shift.y());
						m_victimBall = Ball(point,m_victimBall.radius,m_victimBall.type,m_victimBall.recognRadius);

						if (m_tipsMode==semiautomatic)
							removeFromObstructions(point);
					}
					m_pAction->semimanualMousePress();
				}

				chosenVictim = true; 
				m_wndMode = transparent;
				m_pAction->EnableBIChooseButtons(true);
			}
			break;
		case (chooseObstructionBall):
			{
				m_obstructionBall.append(Ball(event->pos(),m_whiteBall.radius,obstruction,m_whiteBall.recognRadius));

				if ((m_tipsMode==semimanual)||(m_tipsMode==semiautomatic)){
					QVector2D shift = setBallPixmapRecogn(m_obstructionBall.last());
					QPoint local = m_pBallPixmapRecogn->ballCognition(m_obstructionBall.last().recognRadius);//.radius);
					if (local!=QPoint(0,0)){
						QPoint point(local.x() + shift.x(), local.y() + shift.y());
						m_obstructionBall.last() = Ball(point,m_obstructionBall.last().radius,m_obstructionBall.last().type,m_obstructionBall.last().recognRadius);
					}
					m_pAction->semimanualMousePress();
				}

				chosenObstruction = true; 
				m_wndMode = chooseObstructionBall;	
				m_pAction->EnableBIClearObstructionsButton(true);
			}
			break;
		case (chooseStepBall):
			{
				m_stepBall.center = event->pos();
				
				if ((m_tipsMode==semimanual)||(m_tipsMode==semiautomatic)){
					QVector2D shift = setBallPixmapRecogn(m_stepBall);
					QPoint local = m_pBallPixmapRecogn->ballCognition(m_stepBall.recognRadius);//.radius);
					if (local!=QPoint(0,0)){
						QPoint point(local.x() + shift.x(), local.y() + shift.y());
						m_stepBall = Ball(point,m_stepBall.radius,m_stepBall.type,m_stepBall.recognRadius);

						if (m_tipsMode==semiautomatic)
							removeFromObstructions(point);
					}
					m_pAction->semimanualMousePress();
				}

				chosenStepping = true; 
				m_wndMode = transparent;
				m_pAction->EnableBIChooseButtons(true);
			}
			break;
		case (chooseSndStepBall):
			{
				m_sndStepBall.center = event->pos();
				
				if ((m_tipsMode==semimanual)||(m_tipsMode==semiautomatic)){
					QVector2D shift = setBallPixmapRecogn(m_sndStepBall);
					QPoint local = m_pBallPixmapRecogn->ballCognition(m_sndStepBall.recognRadius);//.radius);
					if (local!=QPoint(0,0)){
						QPoint point(local.x() + shift.x(), local.y() + shift.y());
						m_sndStepBall = Ball(point,m_sndStepBall.radius,m_sndStepBall.type,m_sndStepBall.recognRadius);

						if (m_tipsMode==semiautomatic)
							removeFromObstructions(point);
					}
					m_pAction->semimanualMousePress();
				}

				chosenSndStepping = true; 
				m_wndMode = transparent;
				m_pAction->EnableBIChooseButtons(true);
			}
			break;
		case (choosePocket):
			{
				int b = m_pocketFrame.isPntNear(event->pos());
				if (b!=0)
				{
					m_chosenPocket = b;
					chosenPocket = true;

					m_wndMode = transparent;
					m_pAction->EnableBIChooseButtons(true);
				}
			}
			break;
		case (transparent):
			{
				m_wndMode = transparent;
				m_pAction->EnableBIChooseButtons(true);
			}
			break;
		default:
			{
			}
			break;
		}

		if ((chosenWhite)&&(chosenVictim))
			recalc();

		repaint();
	}

	if (event->button() == Qt::RightButton) {
		escapeMode();
	}
}

void TipsWnd::removeFromObstructions(const QPoint& point)
{
	if (!m_obstructionBall.empty()){
		m_obstructionBall.removeOne(Ball(point,m_whiteBall.radius,obstruction,m_whiteBall.recognRadius));
		QList<QPoint> ngh = WalkPoint(point).allNeighbours(m_frames.sndFrameRect);
		for (int i=0;i<ngh.count();i++)
			m_obstructionBall.removeOne(Ball(ngh[i],m_whiteBall.radius,obstruction,m_whiteBall.recognRadius));
	}
}

void TipsWnd::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);

	if (AreaPoint(event->pos()).isNear(m_movePoint)){
		setCursor(QCursor(Qt::SizeAllCursor));
	}
	else if (AreaPoint(event->pos()).isNear(m_resizePoint)){
		setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (m_wndMode == chooseWhiteBall){
		setCursor(QCursor(CursorPixmap(m_whiteBall.radius, m_whiteBall.type)));
	}
	else if (m_wndMode == chooseVictimBall){
		setCursor(QCursor(CursorPixmap(m_victimBall.radius, m_victimBall.type)));
	}
	else if (m_wndMode == chooseObstructionBall){
		setCursor(QCursor(CursorPixmap(m_whiteBall.radius, obstruction)));
	}
	else if ((m_wndMode == chooseStepBall)||(m_wndMode == chooseSndStepBall)){
		setCursor(QCursor(CursorPixmap(m_stepBall.radius, m_stepBall.type)));
	}
	else if (m_wndMode == choosePocket){
		int b = m_pocketFrame.isPntNear(event->pos());
		if (b!=0)
			setCursor(QCursor(Qt::PointingHandCursor));
		else
			setCursor(QCursor(Qt::ArrowCursor));
	}
	else{
		setCursor(QCursor(Qt::ArrowCursor));
	}

    if (moving)
        move(event->globalPos() - offset);
	if (resizing)
		resize(event->pos().x(), event->pos().y());
}

void TipsWnd::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mouseReleaseEvent(event);
    if (moving) 
        moving = false;
    if (resizing)
		resizing = false;
}

void TipsWnd::onResize()
{
	m_movePoint = QPoint(size().width()-1,0);
	m_resizePoint = QPoint(size().width()-1,size().height()-1);

	m_frames = Frame(size(), m_frames.get2ndFrameDst());
	m_pocketFrame.setPocketFrame(m_frames, m_chamferWidth);
}

void TipsWnd::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	onResize();
}

void TipsWnd::closeEvent(QCloseEvent* event)
{
	m_pAction->EnableBISettings(false);
	m_pAction->EnableBIChooseButtons(false);
	m_pAction->EnableBITipsButton(true);
	m_pAction->closeScreenWnd();
}

void TipsWnd::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		if ((m_wndMode == chooseWhiteBall)||(m_wndMode == chooseVictimBall)||(m_wndMode == choosePocket)||(m_wndMode == chooseObstructionBall)||(m_wndMode == chooseStepBall)||(m_wndMode == chooseSndStepBall))
		{ 
			escapeMode();
		}
	}
	QWidget::keyPressEvent(event);
}

void TipsWnd::escapeMode()
{
	m_wndMode = transparent;
	recalc();
	repaint();
	setCursor(QCursor(Qt::ArrowCursor));
	m_pAction->EnableBIChooseButtons(true);
}

void TipsWnd::setCatchMouse(bool b, TipsPainter& p)
{
	if (b)
	{
		QColor colortr = QColor(1,0,1);
		colortr.setAlpha(1);
		p.fillRect(0,0,size().width()-1,size().height()-1, colortr);
	}
	else
	{
		p.fillRect(0,0,size().width()-1,size().height()-1, QColor(Qt::transparent));
	}

}

void TipsWnd::pointWhiteBall()
{
	chosenWhite = false;
	clearTrajectories();
	m_whiteBall.clearBall();
	m_wndMode = chooseWhiteBall;
	repaint();
}

void TipsWnd::pointVictimBall()
{
	if (!m_obstructionBall.empty())
		m_obstructionBall.append(Ball(m_victimBall.center,m_victimBall.radius,obstruction,m_victimBall.recognRadius));

	chosenVictim = false;
	clearTrajectories();
	m_victimBall.clearBall();
	m_wndMode = chooseVictimBall;
	repaint();
}

void TipsWnd::pointSteppingBall()
{
	chosenStepping = false;
	clearTrajectories();
	m_stepBall.clearBall();
	m_wndMode = chooseStepBall;
	repaint();
}

void TipsWnd::pointSndSteppingBall()
{
	chosenSndStepping = false;
	clearTrajectories();
	m_sndStepBall.clearBall();
	m_wndMode = chooseSndStepBall;
	repaint();
}

void TipsWnd::pointAllBalls()
{
	m_obstructionBall.clear();
	chosenObstruction = false;
	m_wndMode = transparent;
	setCursor(QCursor(Qt::ArrowCursor));

	recognizeBalls();

	clearTrajectories();
	recalc();
	repaint();
}

void TipsWnd::pointObstructionBall()
{
	if (m_obstructionBall.empty())
		chosenObstruction = false;
	clearTrajectories();
	m_wndMode = chooseObstructionBall;
	repaint();
}

void TipsWnd::recognizeBalls()
{
	m_pAction->EnableBIAllBallsButton(false);

	QVector2D shift = setPixmapRecogn();
	QList<Ball> list = m_pAutoPixmapRecogn->allBallsCognition(m_whiteBall.radius, m_whiteBall.recognRadius); 

	if (!list.empty())
	{
		for (int k=0;k<list.count();k++)
		{
			QPoint point(list[k].center.x() + shift.x(), list[k].center.y() + shift.y());
			list[k].center.setX(point.x());
			list[k].center.setY(point.y());
			bool b = true;
			if ((point==round(m_whiteBall.center))||(point==round(m_victimBall.center))||(point==round(m_stepBall.center))||(point==round(m_sndStepBall.center))){
				b = false;
			}
			else{
				QList<QPoint> ngh = WalkPoint(point).allNeighbours(m_frames.sndFrameRect);
				for (int i=0;i<ngh.count();i++)
				{
					point = ngh.at(i);
					if ((point==round(m_whiteBall.center))||(point==round(m_victimBall.center))||(point==round(m_stepBall.center))||(point==round(m_sndStepBall.center)))
						b = false;
				}
			}

			if (b) //((point!=round(m_whiteBall.center))&&(point!=round(m_victimBall.center))&&(point!=round(m_stepBall.center))&&(point!=round(m_sndStepBall.center)))
			{
				if (list[k].type==white){
					m_whiteBall.clearBall();
					m_whiteBall = list[k];
					chosenWhite = true;
				}
				else{
					m_obstructionBall.append(list[k]);
				}
			}
		}
		chosenObstruction = true; 
	
		if (!chosenVictim) {
			m_wndMode = chooseVictimBall;
			m_pAction->EnableBIVictimBallButton(false);
		}
		m_pAction->EnableBIObstructionButton(true);
		m_pAction->EnableBIDiscardButton(true);

		m_pAction->EnableBIClearObstructionsButton(true);
	}

	m_pAction->EnableBIAllBallsButton(true);
}

void TipsWnd::pointClearObstructions()
{
	if (m_obstructionBall.empty())
		return;

	m_obstructionBall.clear();
	chosenObstruction = false;
	recalc();
	repaint();
}

void TipsWnd::clearTrajectories()
{
	m_whiteBallTraj.clear(); 
	m_victimBallTraj.clear(); 
	m_stepBallTraj.clear();
}

void TipsWnd::recalc()
{
	if ((chosenWhite)&&(chosenVictim))
	{
		static Numeric2bActionWnd action(this);

		clearTrajectories();
		Numeric2b num(&m_whiteBall, &m_victimBall, &m_stepBall, &m_sndStepBall, &m_pocketFrame, &m_obstructionBall, m_chosenPocket, chosenGoTo, &action);

		if (!chosenStepping)
		{
			num.hit_2balls(&m_whiteBallTraj, &m_victimBallTraj);
		}
		else if (!chosenSndStepping)
		{
			num.hit_3balls(&m_whiteBallTraj, &m_victimBallTraj, &m_stepBallTraj);
		}
		else
		{
			num.hit_4balls(&m_whiteBallTraj, &m_victimBallTraj, &m_stepBallTraj); //sndTraj
		}
	}
}

void TipsWnd::pressDiscard()
{
	chosenWhite = false;
	chosenVictim = false;
	chosenStepping = false;
	chosenSndStepping = false;
	chosenObstruction = false;
	chosenPocket = false;
	chosenCue = false;
	chosenGoTo = false;
	clearTrajectories();
	m_obstructionBall.clear();
	m_stepBall.clearBall();
	m_victimBall.clearBall();
	m_whiteBall.clearBall();
	m_chosenPocket = 0;
	escapeMode();
}

void TipsWnd::pointGotoButton()
{
	if ((!chosenWhite)||(!chosenVictim))
		return;

	chosenGoTo = true;
	m_wndMode = transparent;
	recalc();
	repaint();
}

void TipsWnd::pointPocket()
{
	chosenPocket = false;
	m_wndMode = choosePocket;
	repaint();
}

void TipsWnd::pointClearPocket()
{
	if (!chosenPocket)
		return;

	m_chosenPocket = 0;
	chosenPocket = false;
	recalc();
	repaint();
}

void TipsWnd::pointCue()
{
	if (!chosenWhite)
		return;

	Ball localBall;
	QVector2D shift = setBallPixmapRecogn(m_whiteBall, &localBall); 
	m_whiteBall.m_cueTip = m_pBallPixmapRecogn->cueCognition(localBall);
	
	m_pAction->semimanualMousePress();
	chosenCue = true;
	repaint();

	cueShot();
}

void TipsWnd::cueShot()
{	
	if ((m_whiteBall.m_cueTip.isNull())||(!m_whiteBall.isValid()))
		return;

	if (m_wndMode != transparent){
		m_wndMode = transparent; 
		repaint();
	}

	QPoint center = round(m_whiteBall.center);
	Vector cueV = -m_whiteBall.m_cueTip;
	cueV = (cueV*HALF_VELOCITY)/cueV.length();

	double x = double(center.x()) + 2.0*m_whiteBall.radius*cueV.x()/cueV.length();
	if (x<=m_frames.sndFrameCoord(Left)) x = m_frames.sndFrameCoord(Left)+1;
	if (x>=m_frames.sndFrameCoord(Right)) x = m_frames.sndFrameCoord(Right)-1;
	double y = double(center.y()) + 2.0*m_whiteBall.radius*cueV.y()/cueV.length();
	if (y<=m_frames.sndFrameCoord(Top)) y = m_frames.sndFrameCoord(Top)+1;
	if (y>=m_frames.sndFrameCoord(Bottom)) y = m_frames.sndFrameCoord(Bottom)-1;
	QPoint position(pos().x() + round(x), pos().y() + round(y));

#ifdef Q_WS_WIN
	Sleep(1);
	SetCursorPos(position.x(), position.y());
	Sleep(1);
	mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, GetMessageExtraInfo());
	Sleep(1);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo());
	Sleep(1);
	mouse_event(MOUSEEVENTF_MOVE, round(cueV.x()), round(cueV.y()), 0, GetMessageExtraInfo());
	Sleep(1);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo());
#endif
}

Frame TipsWnd::getFrame() const 
{
	return m_frames;
}

TipsWnd::WindowMode TipsWnd::getWndMode() const
{
	return m_wndMode;
}

void TipsWnd::setWndMode(WindowMode mode)
{
	m_wndMode = mode;
}

bool TipsWnd::loadBackground() 
{
	QString initialPath = QDir::currentPath() + QString("/");
	QString fileName = initialPath + defaultBackgrFileName;

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{
		QDataStream data(&file);
		QSize sz(0,0);
		data >> sz;
		QPixmap allPix(sz.width(), sz.height());
		data >> allPix; 
		file.close();
		QSize siz(m_frames.sndFrameRect.width() - 2, m_frames.sndFrameRect.height() - 2);
		if (m_pBackground=NULL)
			delete m_pBackground;
		m_pBackground = new QPixmap(siz.width(), siz.height());
		m_pBackground->fill(Qt::red);
		QPainter p(m_pBackground);
		p.drawPixmap(0, 0, allPix.copy(m_frames.sndFrameCorner(1).x() + 1 - m_frames.fstFrameCorner(1).x(),
									   m_frames.sndFrameCorner(1).y() + 1 - m_frames.fstFrameCorner(1).y(), siz.width(), siz.height()));
		return true;
	}
	else
	{
		return false;
	}	
}

QVector2D TipsWnd::setPixmapRecogn()
{
	if (!m_pBackground)
		return QVector2D(0,0); //error 

	WindowMode old = m_wndMode;
	m_wndMode = transparent;
	isTrajsPaint = false;
	repaint();

	if (m_pAutoPixmapRecogn!=NULL)
		delete m_pAutoPixmapRecogn;
	m_pAutoPixmapRecogn = new Recognition(QPixmap::grabWindow( QApplication::desktop()->winId(), pos().x() + m_frames.sndFrameCorner(1).x() + 1, pos().y() + m_frames.sndFrameCorner(1).y() + 1, 
										  m_frames.sndFrameRect.width() - 2, m_frames.sndFrameRect.height() - 2 ), *m_pBackground);

	m_wndMode = old;
	isTrajsPaint = true;
	repaint();

	m_pAutoPixmapRecogn->isWhiteColorDiscount = false;
	if (isColorDiscount!=false)
	{/* warning */}

	return QVector2D(m_frames.sndFrameCorner(1).x() + 1, m_frames.sndFrameCorner(1).y() + 1);
}

/*QVector2D TipsWnd::setBallPixmapRecogn(const Ball& hypotheticBall, Ball* pLocalHypotheticBall)
{
	if (!m_pBackground)
		return QVector2D(0,0); //error 

	QSize sz (4.0*m_whiteBall.radius, 4.0*m_whiteBall.radius);
	QPoint localpos (hypotheticBall.center.x() - 0.5*sz.width(), hypotheticBall.center.y() - 0.5*sz.height());

	QRect rect (localpos, sz);
	QRect rect_frame (m_frames.sndFrameCorner(1).x() + 1, m_frames.sndFrameCorner(1).y() + 1, 
					  m_frames.sndFrameRect.width() - 2, m_frames.sndFrameRect.height() - 2);
	QRect r = rect_frame & rect;
	if (r.isEmpty())
		return QVector2D(0,0);
	if (r!=rect)
	{
		localpos = r.topLeft();
		sz = r.size();
	}

	QPoint pos (pos().x() + localpos.x(), pos().y() + localpos.y());

	WindowMode old = m_wndMode;
	m_wndMode = transparent;
	isTrajsPaint = false;
	repaint();

	if (m_pBallPixmapRecogn!=NULL)
		delete m_pBallPixmapRecogn;
	m_pBallPixmapRecogn = new Recognition(QPixmap::grabWindow(QApplication::desktop()->winId(), pos.x(), pos.y(), sz.width(), sz.height()),
										  m_pBackground->copy(localpos.x() - rect_frame.left(), localpos.y() - rect_frame.top(), sz.width(), sz.height()));

	m_wndMode = old;
	isTrajsPaint = true;

	if (pLocalHypotheticBall!=NULL)
		*pLocalHypotheticBall = Ball(QPoint(hypotheticBall.center.x() - localpos.x(), hypotheticBall.center.y() - localpos.y()), hypotheticBall.radius, hypotheticBall.type);

	if (hypotheticBall.type==white)
		m_pBallPixmapRecogn->isWhiteColorDiscount = isColorDiscount;
	else
		m_pBallPixmapRecogn->isWhiteColorDiscount = false;

	return QVector2D(localpos.x(),localpos.y());
}*/

QVector2D TipsWnd::setBallPixmapRecogn(const Ball& hypotheticBall, Ball* pLocalHypotheticBall /*out*/)
{
	if (!m_pBackground)
		return QVector2D(0,0); //error 

	QSize halfsz (2.0*m_whiteBall.recognRadius, 2.0*m_whiteBall.recognRadius);//(2.0*m_whiteBall.radius, 2.0*m_whiteBall.radius);
	QPoint localpos (hypotheticBall.center.x() - halfsz.width(), hypotheticBall.center.y() - halfsz.height());
	QSize sz (2.0*halfsz.width()+1, 2.0*halfsz.height()+1);

	QRect rect (localpos, sz);
	QRect rect_frame (m_frames.sndFrameCorner(1).x() + 1, m_frames.sndFrameCorner(1).y() + 1, 
					  m_frames.sndFrameRect.width() - 2, m_frames.sndFrameRect.height() - 2);
	QRect r = rect_frame & rect;
	if (r.isEmpty())
		return QVector2D(0,0);
	if (r!=rect)
	{
		localpos = r.topLeft();
		sz = r.size();
	}

	QPoint pos (pos().x() + localpos.x(), pos().y() + localpos.y());

	WindowMode old = m_wndMode;
	m_wndMode = transparent;
	isTrajsPaint = false;
	repaint();

	if (m_pBallPixmapRecogn!=NULL)
		delete m_pBallPixmapRecogn;
	m_pBallPixmapRecogn = new Recognition(QPixmap::grabWindow(QApplication::desktop()->winId(), pos.x(), pos.y(), sz.width(), sz.height()),
										  m_pBackground->copy(localpos.x() - rect_frame.left(), localpos.y() - rect_frame.top(), sz.width(), sz.height()));

	m_wndMode = old;
	isTrajsPaint = true;

	if (pLocalHypotheticBall!=NULL)
		*pLocalHypotheticBall = Ball(QPoint(hypotheticBall.center.x() - localpos.x(), hypotheticBall.center.y() - localpos.y()), hypotheticBall.radius, hypotheticBall.type, hypotheticBall.recognRadius);

	if (hypotheticBall.type==white)
		m_pBallPixmapRecogn->isWhiteColorDiscount = isColorDiscount;
	else
		m_pBallPixmapRecogn->isWhiteColorDiscount = false;

	return QVector2D(localpos.x(),localpos.y());
}