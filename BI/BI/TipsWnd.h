#ifndef TIPSWND_H
#define TIPSWND_H

#include <QtGui>
#include <QtCore>
#include "TipsPainter.h"
#include "graphicsObj.h"
#include "numeric.h"
#include "recognition.h"

#ifdef Q_WS_WIN
#include <windows.h>
#endif

#define HALF_VELOCITY 24.0

class TipsWndAction
{
public:
	virtual void EnableBISettings(bool) = 0;
	virtual void secondFrameSpin_setValue(int) = 0;
	virtual void holeWidthSpin_setValue(int) = 0;
	virtual void EnableBITipsButton(bool b) = 0;
	virtual void EnableBIChooseButtons(bool) = 0;
	virtual void EnableBIAllBallsButton(bool) = 0;
	virtual void EnableBIWhiteBallButton(bool) = 0;
	virtual void EnableBIVictimBallButton(bool) = 0;
	virtual void EnableBISteppingStoneBallButton(bool) = 0;
	virtual void EnableBISndSteppingStoneBallButton(bool) = 0;
	virtual void EnableBIPocketButton(bool) = 0;
	virtual void EnableBIObstructionButton(bool) = 0;
	virtual void EnableBIClearObstructionsButton(bool) = 0;
	virtual void EnableBIDiscardButton(bool) = 0;
	virtual void EnableBISaveFramesButton(bool) = 0;
	virtual void ballSizeSpin_setValue(double) = 0;
	virtual void ballRecognSizeSpin_setValue(int) = 0;
	virtual void interballGap_setChecked(bool) = 0;
	virtual void closeScreenWnd() = 0;
	virtual void EnableBIRecognitionMode(bool) = 0;
	virtual void semimanualMousePress() = 0;
};

class TipsWnd : public QWidget
{
	Q_OBJECT

public:
	TipsWnd(QWidget* pBI, TipsWndAction* act);
	~TipsWnd();

	enum WindowMode {chooseWhiteBall, chooseVictimBall, chooseObstructionBall, chooseStepBall, chooseSndStepBall, choosePocket, transparent};
	enum TipsMode {manual, semimanual, semiautomatic};

protected:
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent * ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void mouseReleaseEvent(QMouseEvent* ev);
	void resizeEvent(QResizeEvent* ev);
	void closeEvent(QCloseEvent* ev);
	void keyPressEvent(QKeyEvent* ev);
	void onResize();
	
private:
	bool readSettFile(QPoint* pFramePt, QSize* pFrameSz);
	void writeSettFile(QPoint, QSize, int, double, int, int, bool);
	bool defaultSettingsVistaBook(QPoint* pFramePt, QSize* pFrameSz);
	void setCatchMouse(bool, TipsPainter&);
	void resizeHolesPosition();
	void escapeMode();
	void clearTrajectories();
	void recalc();
	void removeFromObstructions(const QPoint&);
	void cueShot();

	QVector2D setBallPixmapRecogn(const Ball&, Ball* loc = 0);
	QVector2D setPixmapRecogn();

	QPixmap* m_pBackground;
	Recognition* m_pAutoPixmapRecogn;

	bool moving;
	bool resizing;
	bool chosenWhite;
	bool chosenVictim;
	bool chosenPocket;
	bool chosenStepping;
	bool chosenSndStepping;
	bool chosenObstruction;
	bool chosenCue;
	bool chosenGoTo;
	QPoint offset;
	QPoint m_movePoint;
	QPoint m_resizePoint;

	QFile settFile;
	Frame m_frames;
	PocketFrame m_pocketFrame;
	int m_chamferWidth;

	WindowMode m_wndMode;
	TipsMode m_tipsMode;

	Ball m_whiteBall;
	Ball m_victimBall;
	Ball m_stepBall;
	Ball m_sndStepBall;
	QList<Ball> m_obstructionBall;
	int m_chosenPocket;
	QList<BallTraj> m_whiteBallTraj;
	QList<BallTraj> m_victimBallTraj;
	QList<BallTraj> m_stepBallTraj;

public:
	void saveSettings(int, double, int, int, bool);
	bool readSettings();
	void pointWhiteBall();
	void pointVictimBall();
	void pointSteppingBall();
	void pointSndSteppingBall();
	void pointObstructionBall();
	void pointAllBalls();
	void pointClearObstructions();
	void pointPocket();
	void pointClearPocket();
	void pointGotoButton();
	void pointCue();
	void pressDiscard();
	void recognizeBalls();
	void setManualTipsMode();
	void setSemimanualTipsMode();
	void setSemiautomaticTipsMode();

	WindowMode getWndMode() const;
	void setWndMode(WindowMode);
	Frame getFrame() const;

	bool loadBackground();

	TipsWndAction* m_pAction;
	Recognition* m_pBallPixmapRecogn;
	bool isTrajsPaint;
	bool isInterballGap;
	bool isColorDiscount;
	double impactAngle;

public slots:
	void sndFrameDst_changeValue(int);
	void ballRadius_changeValue(double);
	void ballRecognitionRadius_changeValue(double);
	void holeWidth_changeValue(int);
	void colorDiscountCheckBox_checked(int);
	void interBallGapCheckBox_checked(int);
	void impactAngle_changeValue(double);
};





#endif //TIPSWND_H