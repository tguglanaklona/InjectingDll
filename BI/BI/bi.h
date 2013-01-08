#ifndef BI_H
#define BI_H

#include <QtGui/QDialog>
#include "ui_bi.h"
#include "TipsWnd.h"
#include "screen.h"

class BI : public QDialog
{
	Q_OBJECT

public:
	BI(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BI();
	
	Ui::BIClass ui;

	TipsWnd* pTipswnd;
	Screenshot* pScreenWnd;

	void EnableSettings(bool);
	void EnableChooseButtons(bool);
	void EnableWhiteBallButton(bool);
	void EnableVictimBallButton(bool);
	void EnableSteppingStoneBallButton(bool);
	void EnableSndSteppingStoneBallButton(bool);
	void EnableObstructionButton(bool);
	void EnableAllBallsButton(bool);
	void EnableClearObstructionsButton(bool);
	void EnableRecognitionMode(bool);
	void EnablePocketButton(bool);
	void EnableDiscardButton(bool);
	void EnableSaveFramesButton(bool);

	void closeScreenWnd();
	void closeTipsWnd();

protected:
	void closeEvent(QCloseEvent* );
	void keyPressEvent(QKeyEvent* );

private slots:
	void tipsButton_onClicked();
	void saveSettingsButton_onClicked();
	void whiteBallButton_onClicked();
	void victimBallButton_onClicked();
	void stepBallButton_onClicked();
	void sndStepBallButton_onClicked();
	void discardButton_onClicked();
	void obstructionBallButton_onClicked();
	void allBallsButton_onClicked();
	void clearObstructionsButton_onClicked();
	void choosePocketButton_onClicked();
	void chooseClearPocketButton_onClicked();
	void gotoButton_onClicked();
	void cueButton_onClicked();
	void screenAnalysisButton_onClicked();
	void manualModeRadioButton_onChecked();
	void semimanualModeRadioButton_onChecked();
	void semiautomaticModeRadioButton_onChecked();
	void settingsSpinBox_onChanged();
};

#endif // BI_H
