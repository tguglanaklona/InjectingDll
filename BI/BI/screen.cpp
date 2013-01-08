#include "screen.h"

Screenshot::Screenshot(TipsWnd* tipsWnd): m_pTipsWnd(tipsWnd), chosenSide(righthalf), saved(false), mode(ScrMode)
 {
	 for (int i=0;i<3;i++)
		tablesidesPixmap[i]=NULL;

     screenshotLabel = new QLabel;
     screenshotLabel->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Expanding);
     screenshotLabel->setAlignment(Qt::AlignCenter);
     screenshotLabel->setMinimumSize(240, 160);

     createOptionsGroupBox();
     createButtonsLayout();

     mainLayout = new QVBoxLayout;
     mainLayout->addWidget(screenshotLabel);
     mainLayout->addWidget(optionsGroupBox);
     mainLayout->addLayout(buttonsLayout);
     setLayout(mainLayout);

	 righthalfScreenshotButton->setChecked(true);
     shootScreen();

     delaySpinBox->setValue(1);

     setWindowTitle(tr("Screenshot"));
     resize(300, 200);
 }

Screenshot::~Screenshot()
{
	for (int i=0;i<3;i++)
	{
		if (tablesidesPixmap[i]){
			delete (tablesidesPixmap[i]);
			tablesidesPixmap[i] = NULL;}
	}
}

void Screenshot::resizeEvent(QResizeEvent * /* event */)
 {
	 if (!tablesidesPixmap[chosenSide])
		 return;

     QSize scaledSize = tablesidesPixmap[chosenSide]->size();
     scaledSize.scale(screenshotLabel->size(), Qt::KeepAspectRatio);
     if (!screenshotLabel->pixmap()
             || scaledSize != screenshotLabel->pixmap()->size())
         updateScreenshotLabel();
 }

void Screenshot::closeEvent(QCloseEvent *event)
{
	if (saved) {
		if (m_pTipsWnd->loadBackground())
			m_pTipsWnd->m_pAction->EnableBIRecognitionMode(true);
		else
			m_pTipsWnd->m_pAction->EnableBIRecognitionMode(false);
	}
	QWidget::close();
};

void Screenshot::newScreenshot()
 {
     if (hideThisWindowCheckBox->isChecked())
         hide();

     QTimer::singleShot(delaySpinBox->value() * 1000, this, SLOT(shootScreen()));
 }

void Screenshot::processScreenshot(bool beep)
{
	if (beep) {
		if (delaySpinBox->value() != 0)
			qApp->beep();
	}

	updateScreenshotLabel();
}

void Screenshot::saveScreenshot()
 {
	 if (mode != ScrMode)
		 return;

	 if (!tablesidesPixmap[processimage])
		 return;

     QString format = "png";
	 QString defaultFileName = defaultBackgrFileName; 
	 int n = defaultFileName.size()-defaultFileName.lastIndexOf(QString("."));
	 defaultFileName.chop(n);
	 QString initial = QDir::currentPath() + tr("/") + defaultFileName;

     QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                initial + tr(".") + format,
                                tr("%1 Files (*.%2);;All Files (*)")
                                .arg(format.toUpper())
                                .arg(format));
     if (!fileName.isEmpty()) {
        tablesidesPixmap[processimage]->save(fileName, format.toAscii().constData());
		saveScreenshotPixmap(fileName);
		
		fileName.chop(n);
		if (fileName == initial)
			saved = true;
	 }
 }

bool Screenshot::loadScreenshot()	
{
	if (mode != ScrMode)
		return false;

	if (!loadScreenshotButton->isChecked())
		return false;

    QString initialPath = QDir::currentPath();

	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Screenshot"), 
								initialPath, tr("Screenshot Files (*.pxm)"));
	if (!fileName.isEmpty()) {
		if (loadScreenshotPixmap(fileName)) {

			chosenSide = processimage;
			processScreenshot(true);

			lefthalfScreenshotButton->setChecked(false);
			righthalfScreenshotButton->setChecked(false);
			processButton->setChecked(false);
			processButton->setDisabled(true);
			saveScreenshotButton->setEnabled(true);
			return true;
		}
	}
	return false;
}

bool Screenshot::loadDefaultScreenshot()
{
	if (!loadScreenshotButton->isChecked())
		return false;

	QString initialPath = QDir::currentPath() + tr("/");
	QString fileName = initialPath + defaultBackgrFileName;

	if (loadScreenshotPixmap(fileName)) {

		chosenSide = processimage;
		processScreenshot(true);

		lefthalfScreenshotButton->setChecked(false);
		righthalfScreenshotButton->setChecked(false);
		processButton->setChecked(false);
		processButton->setDisabled(true);
		saveScreenshotButton->setEnabled(true);
		return true;
	}
	return false;
}

bool Screenshot::loadScreenshotPixmap(QString fileName)
{
	if (fileName.isEmpty())
		 return false;

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{
		QDataStream data(&file);
		QSize sz(0,0);
		data >> sz;
		if (tablesidesPixmap[processimage]!=NULL)
			delete tablesidesPixmap[processimage];
		tablesidesPixmap[processimage] = new QPixmap(sz.width(), sz.height());
		data >> *tablesidesPixmap[processimage]; 
		file.close();
		return true;
	}
	else
	{
		return false;
	}	
}

void Screenshot::saveScreenshotPixmap(QString fileName)
 {
	 if (!tablesidesPixmap[processimage])
		 return;
	 if (fileName.isEmpty())
		 return;

	 fileName.chop(fileName.size()-fileName.lastIndexOf(QString(".")));
	 fileName.append(QString(".pxm"));
	 QFile file(fileName);

	 if (file.open(QIODevice::WriteOnly))
	{
		QDataStream pix(&file);
		pix << tablesidesPixmap[processimage]->size() << *tablesidesPixmap[processimage]; 
		file.close();
	}
	else
	{
		//error
	}
 }

void Screenshot::righthalfScreenshot()
{
	setScreenshotMode(ScrMode);

	if (!righthalfScreenshotButton->isChecked()) {
		if (processButton->isEnabled()){
			if (processButton->isChecked()) {
				processButton->setChecked(false);
				saveScreenshotButton->setDisabled(true);
			}
			processButton->setDisabled(true);
		}
		return;
	}
	else {
		if ((lefthalfScreenshotButton->isChecked())&&(!processButton->isEnabled()))
			processButton->setEnabled(true);
		if (loadScreenshotButton->isChecked())
			loadScreenshotButton->setChecked(false);
	}

	chosenSide = righthalf;
	newScreenshot();
}

void Screenshot::lefthalfScreenshot()
{
	setScreenshotMode(ScrMode);

	if (!lefthalfScreenshotButton->isChecked()) {
		if (processButton->isEnabled()){
			if (processButton->isChecked()) {
				processButton->setChecked(false);
				saveScreenshotButton->setDisabled(true);
			}
			processButton->setDisabled(true);
		}
		return;
	}
	else {
		if ((righthalfScreenshotButton->isChecked())&&(!processButton->isEnabled())) 
			processButton->setEnabled(true);
		if (loadScreenshotButton->isChecked())
			loadScreenshotButton->setChecked(false);
	}

	chosenSide = lefthalf;
	newScreenshot();
}

void Screenshot::process()
{
	setScreenshotMode(ScrMode);

	if (!processButton->isChecked()) {
		saveScreenshotButton->setDisabled(true);
		return;
	}
	else {
		saveScreenshotButton->setEnabled(true);
		if (loadScreenshotButton->isChecked())
			loadScreenshotButton->setChecked(false);
	}

	if ((!tablesidesPixmap[righthalf])||(!tablesidesPixmap[lefthalf]))
		return;

	chosenSide = processimage;

	if (tablesidesPixmap[chosenSide])
		delete (tablesidesPixmap[chosenSide]);
	
	tablesidesPixmap[processimage] = new QPixmap(tablesidesPixmap[righthalf]->width(), tablesidesPixmap[righthalf]->height());
	tablesidesPixmap[processimage]->fill(Qt::red);
	QPainter p(tablesidesPixmap[processimage]);
	QPixmap pixmap_left = tablesidesPixmap[lefthalf]->copy(0,0,0.5*tablesidesPixmap[lefthalf]->width(),tablesidesPixmap[lefthalf]->height());
	QPixmap pixmap_right = tablesidesPixmap[righthalf]->copy(0.5*tablesidesPixmap[righthalf]->width(),0,0.5*tablesidesPixmap[righthalf]->width(),tablesidesPixmap[righthalf]->height());
	p.drawPixmap(0, 0, pixmap_left);
	p.drawPixmap(pixmap_left.width(), 0, pixmap_right);

	processScreenshot(true);
}

void Screenshot::shootScreen()
 {
     if (delaySpinBox->value() != 0)
         qApp->beep();

	 Frame frames = m_pTipsWnd->getFrame();
	 QPoint pos = m_pTipsWnd->pos();

	 TipsWnd::WindowMode old = m_pTipsWnd->getWndMode();
	 m_pTipsWnd->setWndMode(TipsWnd::WindowMode::transparent);
	 m_pTipsWnd->isTrajsPaint = false;
	 m_pTipsWnd->repaint();

	 if (tablesidesPixmap[chosenSide]!=NULL)
		delete (tablesidesPixmap[chosenSide]);
	 tablesidesPixmap[chosenSide] = new QPixmap(QPixmap::grabWindow( QApplication::desktop()->winId(), 
																	 pos.x() + frames.fstFrameCorner(1).x(), //pos.x() + frames.sndFrameCorner(1).x() + 1, 
																	 pos.y() + frames.fstFrameCorner(1).y(), //pos.y() + frames.sndFrameCorner(1).y() + 1, 
																	 frames.fstFrameRect.width(), frames.fstFrameRect.height() )); //frames.sndFrameRect.width() - 2, frames.sndFrameRect.height() - 2));
	 m_pTipsWnd->setWndMode(old);
	 m_pTipsWnd->isTrajsPaint = true;
	 
	 updateScreenshotLabel();
	 
     if (hideThisWindowCheckBox->isChecked())
         show();
 }

void Screenshot::updateCheckBox()
 {
     if (delaySpinBox->value() == 0) {
         hideThisWindowCheckBox->setDisabled(true);
         hideThisWindowCheckBox->setChecked(false);
     }
     else
         hideThisWindowCheckBox->setDisabled(false);
 }

 void Screenshot::createOptionsGroupBox()
 {
     optionsGroupBox = new QGroupBox(tr("Options"));

     delaySpinBox = new QSpinBox;
     delaySpinBox->setSuffix(tr(" s"));
     delaySpinBox->setMaximum(60);
     connect(delaySpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCheckBox()));

     delaySpinBoxLabel = new QLabel(tr("Screenshot Delay:"));

     hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"));
	 hideThisWindowCheckBox->setChecked(true);

	 whiteColorDiscountCheckBox = new QCheckBox(tr("Discount Tip (255,255,255) Color if Recognize a Ball"));
	 connect(whiteColorDiscountCheckBox, SIGNAL(stateChanged(int)), m_pTipsWnd, SLOT(colorDiscountCheckBox_checked(int)));
	 whiteColorDiscountCheckBox->setChecked(true);

     optionsGroupBoxLayout = new QGridLayout;
     optionsGroupBoxLayout->addWidget(delaySpinBoxLabel, 0, 0);
     optionsGroupBoxLayout->addWidget(delaySpinBox, 0, 1);
     optionsGroupBoxLayout->addWidget(hideThisWindowCheckBox, 1, 0, 1, 2);
	 optionsGroupBoxLayout->addWidget(whiteColorDiscountCheckBox,1,1);
     optionsGroupBox->setLayout(optionsGroupBoxLayout);
 }

 void Screenshot::setScreenshotMode(ScreenshotMode m)
 {
	 mode = m;

	 if (mode==BallScrMode)
	 {
		 processButton->setDisabled(true);
		 saveScreenshotButton->setDisabled(true);
		 loadScreenshotButton->setDisabled(true);
	 }
	 else if (mode==ScrMode)
	 {
		 if (!loadScreenshotButton->isEnabled())
			loadScreenshotButton->setEnabled(true);
	 }
	 else
	 {
	 }
 }

 ScreenshotMode Screenshot::getScreenshotMode()
 {
	 return mode;
 }

 void Screenshot::createButtonsLayout()
 {
	 righthalfScreenshotButton = createButton(tr("Right-half Side Screenshot"),
                                        this, SLOT(righthalfScreenshot()));
	 righthalfScreenshotButton->setCheckable(true);

	 lefthalfScreenshotButton = createButton(tr("Left-half Side Screenshot"),
                                        this, SLOT(lefthalfScreenshot()));
	 lefthalfScreenshotButton->setCheckable(true);

	 processButton = createButton(tr("Process"),
                                        this, SLOT(process()));
	 processButton->setCheckable(true);
	 processButton->setDisabled(true);

     saveScreenshotButton = createButton(tr("Save Screenshot"),
                                         this, SLOT(saveScreenshot()));
	 saveScreenshotButton->setDisabled(true);

	 loadScreenshotButton = createButton(tr("Load Screenshot"),
                                         this, SLOT(loadScreenshot()));
	 loadScreenshotButton->setCheckable(true);

     quitScreenshotButton = createButton(tr("Quit"), this, SLOT(close()));

	 buttonsLayout = new QHBoxLayout;
     buttonsLayout->addStretch();
	 buttonsLayout->addWidget(lefthalfScreenshotButton);
     buttonsLayout->addWidget(righthalfScreenshotButton);
     buttonsLayout->addWidget(processButton);

     buttonsLayout->addStretch();
     buttonsLayout->addWidget(saveScreenshotButton);
	 buttonsLayout->addWidget(loadScreenshotButton);
     buttonsLayout->addWidget(quitScreenshotButton);
 }


 QPushButton *Screenshot::createButton(const QString &text, QWidget *receiver,
                                       const char *member)
 {
     QPushButton *button = new QPushButton(text);
     button->connect(button, SIGNAL(clicked()), receiver, member);
     return button;
 }


 void Screenshot::updateScreenshotLabel()
 {
	 if (mode == ScrMode)
	 {
		 if (!tablesidesPixmap[chosenSide])
			 return;

		 screenshotLabel->setPixmap(tablesidesPixmap[chosenSide]->scaled(screenshotLabel->size(),
																			 Qt::KeepAspectRatio,
																			 Qt::SmoothTransformation));
	 }
	 else if (mode == BallScrMode)
	 {
		 if (!m_pTipsWnd->m_pBallPixmapRecogn)
			 return;

		 screenshotLabel->setPixmap(m_pTipsWnd->m_pBallPixmapRecogn->scaled(screenshotLabel->size(),
																		  Qt::KeepAspectRatio,
																		  Qt::SmoothTransformation));
	 }
	 else
	 {
		 //error
	 }
 }
