#ifndef SCREEN_H
#define SCREEN_H

#include <QtCore>
#include <QtGui>
#ifndef GRAPHICSOBJ_H
#include "graphicsObj.h"
#endif GRAPHICSOBJ_H
#ifndef TIPSWND_H
#include "TipsWnd.h"
#endif TIPSWND_H
#ifndef HEADERS_H
#include "headers.h"
#endif HEADERS_H

enum ScreenshotMode{ScrMode, BallScrMode};

class Screenshot : public QWidget
 {
     Q_OBJECT

 public:
     Screenshot(TipsWnd* tipsWnd);
	 ~Screenshot();

	 enum TableSide{righthalf, lefthalf, processimage};

	 QPixmap getBackgroundScreenshot() {return *tablesidesPixmap[2];};
	 void setScreenshotMode(ScreenshotMode);
	 ScreenshotMode getScreenshotMode();
	 void processScreenshot(bool beep);

	 QCheckBox* whiteColorDiscountCheckBox;

 protected:
     void resizeEvent(QResizeEvent *event);
	 void closeEvent(QCloseEvent *event);

 private slots:
     void newScreenshot();
     void saveScreenshot();
	 bool loadScreenshot();
	 bool loadDefaultScreenshot();
     void shootScreen();
     void updateCheckBox();
	 void righthalfScreenshot();
	 void lefthalfScreenshot();
	 void process();

 private:
	 void saveScreenshotPixmap(QString fileName);
	 bool loadScreenshotPixmap(QString fileName);

     void createOptionsGroupBox();
     void createButtonsLayout();
     QPushButton *createButton(const QString &text, QWidget *receiver,
                               const char *member);
     void updateScreenshotLabel();

	 ScreenshotMode mode;
     QPixmap* tablesidesPixmap[3];

     QLabel *screenshotLabel;
     QGroupBox *optionsGroupBox;
     QSpinBox *delaySpinBox;
     QLabel *delaySpinBoxLabel;
     QCheckBox *hideThisWindowCheckBox;
	 //QCheckBox* interBallGapCheckBox;
     QPushButton *saveScreenshotButton;
	 QPushButton *loadScreenshotButton;
     QPushButton *quitScreenshotButton;
	 QPushButton *righthalfScreenshotButton;
	 QPushButton *lefthalfScreenshotButton;
	 QPushButton *processButton;

     QVBoxLayout *mainLayout;
     QGridLayout *optionsGroupBoxLayout;
	 QHBoxLayout *buttonsLayout;

	 TipsWnd *m_pTipsWnd;

	 TableSide chosenSide;
	 bool saved;
 };

#endif //SCREEN_H