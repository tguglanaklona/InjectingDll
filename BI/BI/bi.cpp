#include "bi.h"

class TipsWndActionBI: public TipsWndAction
{
public:
	TipsWndActionBI(BI* pClient): m_pClient(pClient) {};

	virtual void EnableBISettings(bool);
	virtual void secondFrameSpin_setValue(int);
	virtual void holeWidthSpin_setValue(int);
	virtual void EnableBITipsButton(bool);
	virtual void EnableBIChooseButtons(bool);
	virtual void EnableBIWhiteBallButton(bool);
	virtual void EnableBIVictimBallButton(bool);
	virtual void EnableBISteppingStoneBallButton(bool);
	virtual void EnableBISndSteppingStoneBallButton(bool);
	virtual void EnableBIPocketButton(bool);
	virtual void EnableBIObstructionButton(bool);
	virtual void EnableBIAllBallsButton(bool);
	virtual void EnableBIClearObstructionsButton(bool);
	virtual void EnableBIDiscardButton(bool);
	virtual void EnableBISaveFramesButton(bool);
	virtual void ballSizeSpin_setValue(double);
	virtual void ballRecognSizeSpin_setValue(int);
	virtual void interballGap_setChecked(bool);
	virtual void closeScreenWnd();
	virtual void EnableBIRecognitionMode(bool);
	virtual void semimanualMousePress();

	BI* m_pClient;
};

void TipsWndActionBI::EnableBISettings(bool b)
{
	m_pClient->EnableSettings(b);
}

void TipsWndActionBI::EnableBIChooseButtons(bool b)
{
	m_pClient->EnableChooseButtons(b);
}

void TipsWndActionBI::EnableBIAllBallsButton(bool b)
{
	m_pClient->EnableAllBallsButton(b);
}

void TipsWndActionBI::EnableBIObstructionButton(bool b)
{
	m_pClient->EnableObstructionButton(b);
}

void TipsWndActionBI::EnableBIDiscardButton(bool b)
{
	m_pClient->EnableDiscardButton(b);
}

void TipsWndActionBI::EnableBISaveFramesButton(bool b)
{
	m_pClient->EnableSaveFramesButton(b);
}

void TipsWndActionBI::EnableBIClearObstructionsButton(bool b)
{
	m_pClient->EnableClearObstructionsButton(b);
}

void TipsWndActionBI::EnableBIWhiteBallButton(bool b)
{
	m_pClient->EnableWhiteBallButton(b);
}

void TipsWndActionBI::EnableBIVictimBallButton(bool b)
{
	m_pClient->EnableVictimBallButton(b);
}

void TipsWndActionBI::EnableBISteppingStoneBallButton(bool b)
{
	m_pClient->EnableSteppingStoneBallButton(b);
}

void TipsWndActionBI::EnableBISndSteppingStoneBallButton(bool b)
{
	m_pClient->EnableSndSteppingStoneBallButton(b);
}

void TipsWndActionBI::EnableBIRecognitionMode(bool b)
{
	m_pClient->EnableRecognitionMode(b);
}

void TipsWndActionBI::EnableBIPocketButton(bool b)
{
	m_pClient->EnablePocketButton(b);
}

void TipsWndActionBI::secondFrameSpin_setValue(int val)
{
	m_pClient->ui.secondFrameSpinBox->setValue(val);
}

void TipsWndActionBI::holeWidthSpin_setValue(int val)
{
	m_pClient->ui.holeWidthSpinBox->setValue(val);
}

void TipsWndActionBI::ballSizeSpin_setValue(double val)
{
	m_pClient->ui.ballSizeSpinBox->setValue(val);
}

void TipsWndActionBI::ballRecognSizeSpin_setValue(int val)
{
	m_pClient->ui.ballRecognSizeSpinBox->setValue(val);
}

void TipsWndActionBI::interballGap_setChecked(bool b)
{
	m_pClient->ui.interBallGapCheckBox->setChecked(b);
}

void TipsWndActionBI::closeScreenWnd()
{
	m_pClient->closeScreenWnd();
}

void TipsWndActionBI::EnableBITipsButton(bool b)
{
	m_pClient->ui.tipsButton->setEnabled(b);
}

void TipsWndActionBI::semimanualMousePress()
{
	if (m_pClient->pScreenWnd==NULL)
		return;

	if (!m_pClient->pScreenWnd->isVisible())
		return;

	m_pClient->pScreenWnd->setScreenshotMode(BallScrMode);
	m_pClient->pScreenWnd->processScreenshot(false);
}

BI::BI(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags), pTipswnd(NULL), pScreenWnd(NULL)
{
	ui.setupUi(this);

	connect(ui.tipsButton, SIGNAL(clicked()),
                      this, SLOT(tipsButton_onClicked()));
	connect(ui.saveFramesButton, SIGNAL(clicked()),
                      this, SLOT(saveSettingsButton_onClicked()));
	connect(ui.whiteBallButton, SIGNAL(clicked()),
                      this, SLOT(whiteBallButton_onClicked()));
	connect(ui.victimBallButton, SIGNAL(clicked()),
                      this, SLOT(victimBallButton_onClicked()));
	connect(ui.steppingBallButton, SIGNAL(clicked()),
                      this, SLOT(stepBallButton_onClicked()));
	connect(ui.sndSteppingBallButton, SIGNAL(clicked()),
                      this, SLOT(sndStepBallButton_onClicked()));
	connect(ui.discardButton, SIGNAL(clicked()),
                      this, SLOT(discardButton_onClicked()));
	connect(ui.obstructionBallButton, SIGNAL(clicked()),
                      this, SLOT(obstructionBallButton_onClicked()));
	connect(ui.clearObstructionsButton, SIGNAL(clicked()),
                      this, SLOT(clearObstructionsButton_onClicked()));
	connect(ui.allBallsButton, SIGNAL(clicked()),
					  this, SLOT(allBallsButton_onClicked()));
	connect(ui.choosePocketButton, SIGNAL(clicked()),
                      this, SLOT(choosePocketButton_onClicked()));
	connect(ui.chooseClearPocketButton, SIGNAL(clicked()),
                      this, SLOT(chooseClearPocketButton_onClicked()));
	connect(ui.gotoButton, SIGNAL(clicked()),
					  this, SLOT(gotoButton_onClicked()));
	connect(ui.cueButton, SIGNAL(clicked()),
                      this, SLOT(cueButton_onClicked()));
	connect(ui.screenAnalysisButton, SIGNAL(clicked()),
                      this, SLOT(screenAnalysisButton_onClicked()));
	connect(ui.manualModeRadioButton, SIGNAL(pressed()),
					  this, SLOT(manualModeRadioButton_onChecked()));
	connect(ui.semimanualModeRadioButton, SIGNAL(pressed()),
					  this, SLOT(semimanualModeRadioButton_onChecked()));
	connect(ui.semiautomaticModeRadioButton, SIGNAL(pressed()),
					  this, SLOT(semiautomaticModeRadioButton_onChecked()));

	connect(ui.secondFrameSpinBox, SIGNAL(valueChanged(int)),
					  this, SLOT(settingsSpinBox_onChanged()));
	connect(ui.ballSizeSpinBox, SIGNAL(valueChanged(double)),
					  this, SLOT(settingsSpinBox_onChanged()));
	connect(ui.holeWidthSpinBox, SIGNAL(valueChanged(int)),
					  this, SLOT(settingsSpinBox_onChanged()));
	connect(ui.ballRecognSizeSpinBox, SIGNAL(valueChanged(double)),
					  this, SLOT(settingsSpinBox_onChanged()));
	connect(ui.interBallGapCheckBox, SIGNAL(stateChanged(int)),
					  this, SLOT(settingsSpinBox_onChanged()));

	QShortcut *shortcut_white = new QShortcut(QKeySequence("Ctrl+W"), this);
	connect(shortcut_white, SIGNAL(activated()), this, SLOT(whiteBallButton_onClicked()));
	QShortcut *shortcut_victim = new QShortcut(QKeySequence("Ctrl+V"), this);
	connect(shortcut_victim, SIGNAL(activated()), this, SLOT(victimBallButton_onClicked()));
	QShortcut *shortcut_obs = new QShortcut(QKeySequence("Ctrl+O"), this);
	connect(shortcut_obs, SIGNAL(activated()), this, SLOT(obstructionBallButton_onClicked()));
	QShortcut *shortcut_step = new QShortcut(QKeySequence("Ctrl+S"), this);
	connect(shortcut_step, SIGNAL(activated()), this, SLOT(stepBallButton_onClicked()));
	QShortcut *shortcut_pocket = new QShortcut(QKeySequence("Ctrl+P"), this);
	connect(shortcut_pocket, SIGNAL(activated()), this, SLOT(choosePocketButton_onClicked()));
	
	ui.manualModeRadioButton->setChecked(true);
	EnableSettings(false);
	EnableChooseButtons(false);
}

BI::~BI()
{
}

void BI::tipsButton_onClicked()
{
	static TipsWndActionBI action(this);

	EnableSettings(true);
	ui.tipsButton->setEnabled(false);
	EnableChooseButtons(true);

	if (pTipswnd==NULL)
		pTipswnd = new TipsWnd(NULL, &action); 
    pTipswnd->show();

	connect(ui.secondFrameSpinBox, SIGNAL(valueChanged(int)),
					  pTipswnd, SLOT(sndFrameDst_changeValue(int)));
	connect(ui.ballSizeSpinBox, SIGNAL(valueChanged(double)),
					  pTipswnd, SLOT(ballRadius_changeValue(double)));
	connect(ui.ballRecognSizeSpinBox, SIGNAL(valueChanged(double)),
					  pTipswnd, SLOT(ballRecognitionRadius_changeValue(double)));
	connect(ui.holeWidthSpinBox, SIGNAL(valueChanged(int)),
					  pTipswnd, SLOT(holeWidth_changeValue(int)));
	connect(ui.interBallGapCheckBox, SIGNAL(stateChanged(int)), 
					  pTipswnd, SLOT(interBallGapCheckBox_checked(int)));
	connect(ui.shotAngleSpinBox, SIGNAL(valueChanged(double)),
					  pTipswnd, SLOT(impactAngle_changeValue(double)));
}

void BI::EnableSettings(bool b)
{
	ui.saveFramesButton->setEnabled(b); 
	ui.secondFrameLabel->setEnabled(b);
	ui.secondFrameSpinBox->setEnabled(b); 
	ui.ballSizeSpinBox->setEnabled(b);
	ui.ballRecognSizeSpinBox->setEnabled(b);
	ui.ballSizeLabel->setEnabled(b);
	ui.holeWidthLabel->setEnabled(b);
	ui.holeWidthSpinBox->setEnabled(b);
	ui.interBallGapCheckBox->setEnabled(b);

	ui.screenAnalysisButton->setEnabled(b);
}

void BI::EnableChooseButtons(bool b)
{
	ui.whiteBallButton->setEnabled(b);
	ui.victimBallButton->setEnabled(b);
	ui.choosePocketButton->setEnabled(b);
	ui.chooseClearPocketButton->setEnabled(b);
	ui.cueButton->setEnabled(b);
	ui.manualModeRadioButton->setEnabled(b);
	ui.semimanualModeRadioButton->setEnabled(b);
	ui.semiautomaticModeRadioButton->setEnabled(b);
	ui.obstructionBallButton->setEnabled(b);
	if (((b)&&(ui.semiautomaticModeRadioButton->isChecked()))||(!b))
		ui.allBallsButton->setEnabled(b);
	ui.clearObstructionsButton->setEnabled(b);
	ui.steppingBallButton->setEnabled(b);
	ui.sndSteppingBallButton->setEnabled(b);
	ui.discardButton->setEnabled(b);
	ui.gotoButton->setEnabled(b);
}

void BI::EnableAllBallsButton(bool b)
{
	ui.allBallsButton->setEnabled(b);
}

void BI::EnableObstructionButton(bool b)
{
	ui.obstructionBallButton->setEnabled(b);
}

void BI::EnableDiscardButton(bool b)
{
	ui.discardButton->setEnabled(b);
}

void BI::EnableSaveFramesButton(bool b)
{
	ui.saveFramesButton->setEnabled(b);
}

void BI::EnableClearObstructionsButton(bool b)
{
	ui.clearObstructionsButton->setEnabled(b);
}

void BI::EnableWhiteBallButton(bool b)
{
	ui.whiteBallButton->setEnabled(b);
}

void BI::EnableVictimBallButton(bool b)
{
	ui.victimBallButton->setEnabled(b);
}

void BI::EnableSteppingStoneBallButton(bool b)
{
	ui.steppingBallButton->setEnabled(b);
}

void BI::EnableSndSteppingStoneBallButton(bool b)
{
	ui.sndSteppingBallButton->setEnabled(b);
}

void BI::EnableRecognitionMode(bool b)
{
	ui.semimanualModeRadioButton->setEnabled(b);
	ui.semiautomaticModeRadioButton->setEnabled(b);
}

void BI::EnablePocketButton(bool b)
{
	ui.choosePocketButton->setEnabled(b);
}

void BI::saveSettingsButton_onClicked()
{
	pTipswnd->saveSettings(ui.secondFrameSpinBox->value(), ui.ballSizeSpinBox->value(), ui.holeWidthSpinBox->value(), ui.ballRecognSizeSpinBox->value(), ui.interBallGapCheckBox->isChecked());
	ui.saveFramesButton->setDisabled(true);
}

void BI::whiteBallButton_onClicked()
{
	pTipswnd->pointWhiteBall(); 
	ui.whiteBallButton->setDisabled(true);
}

void BI::victimBallButton_onClicked()
{
	pTipswnd->pointVictimBall(); 
	ui.victimBallButton->setDisabled(true);
}

void BI::stepBallButton_onClicked()
{
	pTipswnd->pointSteppingBall();
	ui.steppingBallButton->setDisabled(true);
}

void BI::sndStepBallButton_onClicked()
{
	pTipswnd->pointSndSteppingBall();
	ui.sndSteppingBallButton->setDisabled(true);
}

void BI::discardButton_onClicked()
{
	pTipswnd->pressDiscard();
	ui.discardButton->setDisabled(true);
}

void BI::obstructionBallButton_onClicked()
{
	pTipswnd->pointObstructionBall(); 
	ui.obstructionBallButton->setDisabled(true);
}

void BI::allBallsButton_onClicked()
{
	pTipswnd->pointAllBalls(); 
	ui.allBallsButton->setDisabled(true);
}

void BI::clearObstructionsButton_onClicked()
{
	pTipswnd->pointClearObstructions(); 
	ui.clearObstructionsButton->setDisabled(true);
	if (ui.semiautomaticModeRadioButton->isChecked())
		ui.allBallsButton->setEnabled(true);
}

void BI::choosePocketButton_onClicked()
{
	pTipswnd->pointPocket(); 
	ui.choosePocketButton->setDisabled(true);
}

void BI::gotoButton_onClicked()
{
	pTipswnd->pointGotoButton(); 
	ui.gotoButton->setDisabled(true);
}

void BI::chooseClearPocketButton_onClicked()
{
	pTipswnd->pointClearPocket(); 
	ui.chooseClearPocketButton->setDisabled(true);
}

void BI::cueButton_onClicked()
{
	ui.cueButton->setDisabled(true);
	pTipswnd->pointCue(); 
	ui.cueButton->setDisabled(false);
}

void BI::screenAnalysisButton_onClicked()
{
	if (pTipswnd==NULL)
		return; //error

	if (pScreenWnd==NULL)
		pScreenWnd = new Screenshot(pTipswnd);
	pScreenWnd->show();
}

void BI::manualModeRadioButton_onChecked()
{
	if (pTipswnd==NULL)
		return; //error

	pTipswnd->setManualTipsMode();
	ui.semimanualModeRadioButton->setChecked(false);

	if (pScreenWnd){
		pScreenWnd->whiteColorDiscountCheckBox->setChecked(true);//
		pScreenWnd->whiteColorDiscountCheckBox->setDisabled(false);
	}
}

void BI::semimanualModeRadioButton_onChecked()
{
	if (pTipswnd==NULL)
		return; //error

	pTipswnd->setSemimanualTipsMode();
	ui.manualModeRadioButton->setChecked(false);

	if (pScreenWnd){
		pScreenWnd->whiteColorDiscountCheckBox->setChecked(true);//
		pScreenWnd->whiteColorDiscountCheckBox->setDisabled(false);
	}
}

void BI::semiautomaticModeRadioButton_onChecked()
{
	if (pTipswnd==NULL)
		return; //error

	pTipswnd->setSemiautomaticTipsMode();
	ui.semiautomaticModeRadioButton->setChecked(false);

	if (pScreenWnd){
		pScreenWnd->whiteColorDiscountCheckBox->setChecked(false);
		pScreenWnd->whiteColorDiscountCheckBox->setDisabled(true);
	}
}

void BI::settingsSpinBox_onChanged()
{
	ui.saveFramesButton->setDisabled(false);
}

void BI::closeEvent(QCloseEvent* event)
{
	closeTipsWnd();
	closeScreenWnd();
}

void BI::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		if ((pTipswnd->getWndMode() == TipsWnd::WindowMode::chooseWhiteBall)||(pTipswnd->getWndMode() == TipsWnd::WindowMode::chooseVictimBall)||(pTipswnd->getWndMode() == TipsWnd::WindowMode::choosePocket)||(pTipswnd->getWndMode() == TipsWnd::WindowMode::chooseObstructionBall)||(pTipswnd->getWndMode() == TipsWnd::WindowMode::chooseStepBall)||(pTipswnd->getWndMode() == TipsWnd::WindowMode::chooseSndStepBall))
		{ 
			pTipswnd->setWndMode(TipsWnd::WindowMode::transparent);
			pTipswnd->repaint();
			pTipswnd->setCursor(QCursor(Qt::ArrowCursor));
			EnableChooseButtons(true);
		}
	}
	
	QWidget::keyPressEvent(event);
}

void BI::closeScreenWnd()
{
	if (pScreenWnd)
		if (pScreenWnd->isVisible())
			pScreenWnd->close();
	if (pScreenWnd) {delete(pScreenWnd); pScreenWnd=NULL;}
}

void BI::closeTipsWnd()
{
	if (pTipswnd)
		if (pTipswnd->isVisible())
			pTipswnd->close();
	if (pTipswnd) {delete(pTipswnd); pTipswnd=NULL;}
}

