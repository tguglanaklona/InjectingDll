/********************************************************************************
** Form generated from reading UI file 'bi.ui'
**
** Created: Wed 31. Oct 20:37:55 2012
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BI_H
#define UI_BI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BIClass
{
public:
    QGroupBox *groupBox;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLabel *ballSizeLabel;
    QSpinBox *secondFrameSpinBox;
    QPushButton *saveFramesButton;
    QLabel *secondFrameLabel;
    QLabel *holeWidthLabel;
    QSpinBox *holeWidthSpinBox;
    QDoubleSpinBox *ballSizeSpinBox;
    QDoubleSpinBox *ballRecognSizeSpinBox;
    QLabel *ballSizeLabel_2;
    QSpacerItem *verticalSpacer;
    QCheckBox *interBallGapCheckBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QToolButton *whiteBallButton;
    QToolButton *victimBallButton;
    QToolButton *obstructionBallButton;
    QToolButton *clearObstructionsButton;
    QToolButton *allBallsButton;
    QToolButton *steppingBallButton;
    QToolButton *discardButton;
    QToolButton *sndSteppingBallButton;
    QToolButton *choosePocketButton;
    QToolButton *chooseClearPocketButton;
    QVBoxLayout *verticalLayout_3;
    QPushButton *tipsButton;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_3;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *manualModeRadioButton;
    QRadioButton *semimanualModeRadioButton;
    QRadioButton *semiautomaticModeRadioButton;
    QLabel *shotAngleLabel;
    QDoubleSpinBox *shotAngleSpinBox;
    QToolButton *gotoButton;
    QSpacerItem *verticalSpacer_4;
    QGroupBox *groupBox_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_3;
    QPushButton *screenAnalysisButton;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_4;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_4;
    QSpacerItem *verticalSpacer_5;
    QToolButton *cueButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *BIClass)
    {
        if (BIClass->objectName().isEmpty())
            BIClass->setObjectName(QString::fromUtf8("BIClass"));
        BIClass->resize(648, 696);
        groupBox = new QGroupBox(BIClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(300, 10, 281, 221));
        verticalLayoutWidget_2 = new QWidget(groupBox);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(20, 20, 241, 191));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ballSizeLabel = new QLabel(verticalLayoutWidget_2);
        ballSizeLabel->setObjectName(QString::fromUtf8("ballSizeLabel"));

        gridLayout->addWidget(ballSizeLabel, 1, 1, 1, 1);

        secondFrameSpinBox = new QSpinBox(verticalLayoutWidget_2);
        secondFrameSpinBox->setObjectName(QString::fromUtf8("secondFrameSpinBox"));

        gridLayout->addWidget(secondFrameSpinBox, 2, 0, 1, 1);

        saveFramesButton = new QPushButton(verticalLayoutWidget_2);
        saveFramesButton->setObjectName(QString::fromUtf8("saveFramesButton"));

        gridLayout->addWidget(saveFramesButton, 0, 0, 1, 1);

        secondFrameLabel = new QLabel(verticalLayoutWidget_2);
        secondFrameLabel->setObjectName(QString::fromUtf8("secondFrameLabel"));

        gridLayout->addWidget(secondFrameLabel, 1, 0, 1, 1);

        holeWidthLabel = new QLabel(verticalLayoutWidget_2);
        holeWidthLabel->setObjectName(QString::fromUtf8("holeWidthLabel"));

        gridLayout->addWidget(holeWidthLabel, 1, 2, 1, 1);

        holeWidthSpinBox = new QSpinBox(verticalLayoutWidget_2);
        holeWidthSpinBox->setObjectName(QString::fromUtf8("holeWidthSpinBox"));

        gridLayout->addWidget(holeWidthSpinBox, 2, 2, 1, 1);

        ballSizeSpinBox = new QDoubleSpinBox(verticalLayoutWidget_2);
        ballSizeSpinBox->setObjectName(QString::fromUtf8("ballSizeSpinBox"));
        ballSizeSpinBox->setInputMethodHints(Qt::ImhDigitsOnly);
        ballSizeSpinBox->setDecimals(2);
        ballSizeSpinBox->setSingleStep(0.1);

        gridLayout->addWidget(ballSizeSpinBox, 2, 1, 1, 1);

        ballRecognSizeSpinBox = new QDoubleSpinBox(verticalLayoutWidget_2);
        ballRecognSizeSpinBox->setObjectName(QString::fromUtf8("ballRecognSizeSpinBox"));
        ballRecognSizeSpinBox->setInputMethodHints(Qt::ImhDigitsOnly);
        ballRecognSizeSpinBox->setDecimals(0);

        gridLayout->addWidget(ballRecognSizeSpinBox, 4, 1, 1, 1);

        ballSizeLabel_2 = new QLabel(verticalLayoutWidget_2);
        ballSizeLabel_2->setObjectName(QString::fromUtf8("ballSizeLabel_2"));

        gridLayout->addWidget(ballSizeLabel_2, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        interBallGapCheckBox = new QCheckBox(verticalLayoutWidget_2);
        interBallGapCheckBox->setObjectName(QString::fromUtf8("interBallGapCheckBox"));

        verticalLayout_2->addWidget(interBallGapCheckBox);

        gridLayoutWidget = new QWidget(BIClass);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 281, 651));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        whiteBallButton = new QToolButton(gridLayoutWidget);
        whiteBallButton->setObjectName(QString::fromUtf8("whiteBallButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(whiteBallButton->sizePolicy().hasHeightForWidth());
        whiteBallButton->setSizePolicy(sizePolicy);
        whiteBallButton->setMinimumSize(QSize(0, 0));

        gridLayout_2->addWidget(whiteBallButton, 3, 0, 1, 1);

        victimBallButton = new QToolButton(gridLayoutWidget);
        victimBallButton->setObjectName(QString::fromUtf8("victimBallButton"));
        sizePolicy.setHeightForWidth(victimBallButton->sizePolicy().hasHeightForWidth());
        victimBallButton->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(victimBallButton, 3, 1, 1, 1);

        obstructionBallButton = new QToolButton(gridLayoutWidget);
        obstructionBallButton->setObjectName(QString::fromUtf8("obstructionBallButton"));
        sizePolicy.setHeightForWidth(obstructionBallButton->sizePolicy().hasHeightForWidth());
        obstructionBallButton->setSizePolicy(sizePolicy);
        obstructionBallButton->setMinimumSize(QSize(0, 0));

        gridLayout_2->addWidget(obstructionBallButton, 4, 0, 1, 1);

        clearObstructionsButton = new QToolButton(gridLayoutWidget);
        clearObstructionsButton->setObjectName(QString::fromUtf8("clearObstructionsButton"));
        sizePolicy.setHeightForWidth(clearObstructionsButton->sizePolicy().hasHeightForWidth());
        clearObstructionsButton->setSizePolicy(sizePolicy);
        clearObstructionsButton->setMinimumSize(QSize(0, 0));

        gridLayout_2->addWidget(clearObstructionsButton, 4, 1, 1, 1);

        allBallsButton = new QToolButton(gridLayoutWidget);
        allBallsButton->setObjectName(QString::fromUtf8("allBallsButton"));
        sizePolicy.setHeightForWidth(allBallsButton->sizePolicy().hasHeightForWidth());
        allBallsButton->setSizePolicy(sizePolicy);
        allBallsButton->setMinimumSize(QSize(0, 0));

        gridLayout_2->addWidget(allBallsButton, 6, 0, 1, 1);

        steppingBallButton = new QToolButton(gridLayoutWidget);
        steppingBallButton->setObjectName(QString::fromUtf8("steppingBallButton"));
        sizePolicy.setHeightForWidth(steppingBallButton->sizePolicy().hasHeightForWidth());
        steppingBallButton->setSizePolicy(sizePolicy);
        steppingBallButton->setMinimumSize(QSize(0, 0));

        gridLayout_2->addWidget(steppingBallButton, 5, 0, 1, 1);

        discardButton = new QToolButton(gridLayoutWidget);
        discardButton->setObjectName(QString::fromUtf8("discardButton"));
        sizePolicy.setHeightForWidth(discardButton->sizePolicy().hasHeightForWidth());
        discardButton->setSizePolicy(sizePolicy);
        discardButton->setMinimumSize(QSize(0, 0));

        gridLayout_2->addWidget(discardButton, 6, 1, 1, 1);

        sndSteppingBallButton = new QToolButton(gridLayoutWidget);
        sndSteppingBallButton->setObjectName(QString::fromUtf8("sndSteppingBallButton"));
        sizePolicy.setHeightForWidth(sndSteppingBallButton->sizePolicy().hasHeightForWidth());
        sndSteppingBallButton->setSizePolicy(sizePolicy);
        sndSteppingBallButton->setMinimumSize(QSize(0, 0));

        gridLayout_2->addWidget(sndSteppingBallButton, 5, 1, 1, 1);

        choosePocketButton = new QToolButton(gridLayoutWidget);
        choosePocketButton->setObjectName(QString::fromUtf8("choosePocketButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(choosePocketButton->sizePolicy().hasHeightForWidth());
        choosePocketButton->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(choosePocketButton, 8, 0, 1, 1);

        chooseClearPocketButton = new QToolButton(gridLayoutWidget);
        chooseClearPocketButton->setObjectName(QString::fromUtf8("chooseClearPocketButton"));
        sizePolicy1.setHeightForWidth(chooseClearPocketButton->sizePolicy().hasHeightForWidth());
        chooseClearPocketButton->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(chooseClearPocketButton, 8, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tipsButton = new QPushButton(gridLayoutWidget);
        tipsButton->setObjectName(QString::fromUtf8("tipsButton"));
        sizePolicy1.setHeightForWidth(tipsButton->sizePolicy().hasHeightForWidth());
        tipsButton->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(tipsButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        gridLayout_2->addLayout(verticalLayout_3, 2, 0, 1, 1);

        groupBox_3 = new QGroupBox(gridLayoutWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayoutWidget = new QWidget(groupBox_3);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 20, 94, 65));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        manualModeRadioButton = new QRadioButton(verticalLayoutWidget);
        manualModeRadioButton->setObjectName(QString::fromUtf8("manualModeRadioButton"));

        verticalLayout->addWidget(manualModeRadioButton);

        semimanualModeRadioButton = new QRadioButton(verticalLayoutWidget);
        semimanualModeRadioButton->setObjectName(QString::fromUtf8("semimanualModeRadioButton"));

        verticalLayout->addWidget(semimanualModeRadioButton);

        semiautomaticModeRadioButton = new QRadioButton(verticalLayoutWidget);
        semiautomaticModeRadioButton->setObjectName(QString::fromUtf8("semiautomaticModeRadioButton"));

        verticalLayout->addWidget(semiautomaticModeRadioButton);


        gridLayout_2->addWidget(groupBox_3, 2, 1, 1, 1);

        shotAngleLabel = new QLabel(gridLayoutWidget);
        shotAngleLabel->setObjectName(QString::fromUtf8("shotAngleLabel"));

        gridLayout_2->addWidget(shotAngleLabel, 10, 1, 1, 1);

        shotAngleSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        shotAngleSpinBox->setObjectName(QString::fromUtf8("shotAngleSpinBox"));
        shotAngleSpinBox->setInputMethodHints(Qt::ImhDigitsOnly);
        shotAngleSpinBox->setDecimals(2);
        shotAngleSpinBox->setMaximum(90);
        shotAngleSpinBox->setSingleStep(1);

        gridLayout_2->addWidget(shotAngleSpinBox, 11, 1, 1, 1);

        gotoButton = new QToolButton(gridLayoutWidget);
        gotoButton->setObjectName(QString::fromUtf8("gotoButton"));
        sizePolicy1.setHeightForWidth(gotoButton->sizePolicy().hasHeightForWidth());
        gotoButton->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(gotoButton, 11, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_4, 12, 0, 1, 1);

        groupBox_2 = new QGroupBox(BIClass);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(300, 260, 281, 161));
        gridLayoutWidget_2 = new QWidget(groupBox_2);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(20, 20, 241, 131));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        screenAnalysisButton = new QPushButton(gridLayoutWidget_2);
        screenAnalysisButton->setObjectName(QString::fromUtf8("screenAnalysisButton"));

        gridLayout_3->addWidget(screenAnalysisButton, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 1, 1, 1, 1);

        groupBox_4 = new QGroupBox(BIClass);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(300, 450, 281, 161));
        gridLayoutWidget_3 = new QWidget(groupBox_4);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(20, 20, 241, 131));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_5, 1, 0, 1, 1);

        cueButton = new QToolButton(gridLayoutWidget_3);
        cueButton->setObjectName(QString::fromUtf8("cueButton"));
        sizePolicy1.setHeightForWidth(cueButton->sizePolicy().hasHeightForWidth());
        cueButton->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(cueButton, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 1, 1, 1, 1);


        retranslateUi(BIClass);

        QMetaObject::connectSlotsByName(BIClass);
    } // setupUi

    void retranslateUi(QDialog *BIClass)
    {
        BIClass->setWindowTitle(QApplication::translate("BIClass", "BI", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("BIClass", "Settings", 0, QApplication::UnicodeUTF8));
        ballSizeLabel->setText(QApplication::translate("BIClass", "Ball size:", 0, QApplication::UnicodeUTF8));
        saveFramesButton->setText(QApplication::translate("BIClass", "Save settings", 0, QApplication::UnicodeUTF8));
        secondFrameLabel->setText(QApplication::translate("BIClass", "Second frame:", 0, QApplication::UnicodeUTF8));
        holeWidthLabel->setText(QApplication::translate("BIClass", "Hole width:", 0, QApplication::UnicodeUTF8));
        ballSizeLabel_2->setText(QApplication::translate("BIClass", "Recognize as:", 0, QApplication::UnicodeUTF8));
        interBallGapCheckBox->setText(QApplication::translate("BIClass", "Interball Gap", 0, QApplication::UnicodeUTF8));
        whiteBallButton->setText(QApplication::translate("BIClass", "White Ball", 0, QApplication::UnicodeUTF8));
        victimBallButton->setText(QApplication::translate("BIClass", "Victim Ball", 0, QApplication::UnicodeUTF8));
        obstructionBallButton->setText(QApplication::translate("BIClass", "Obstruction Ball", 0, QApplication::UnicodeUTF8));
        clearObstructionsButton->setText(QApplication::translate("BIClass", "CLEAR Obstructions", 0, QApplication::UnicodeUTF8));
        allBallsButton->setText(QApplication::translate("BIClass", "ALL Balls", 0, QApplication::UnicodeUTF8));
        steppingBallButton->setText(QApplication::translate("BIClass", "Stepping-stone Ball", 0, QApplication::UnicodeUTF8));
        discardButton->setText(QApplication::translate("BIClass", "DISCARD", 0, QApplication::UnicodeUTF8));
        sndSteppingBallButton->setText(QApplication::translate("BIClass", "2-nd Stepping-stone Ball", 0, QApplication::UnicodeUTF8));
        choosePocketButton->setText(QApplication::translate("BIClass", "Pocket", 0, QApplication::UnicodeUTF8));
        chooseClearPocketButton->setText(QApplication::translate("BIClass", "CLEAR Pocket", 0, QApplication::UnicodeUTF8));
        tipsButton->setText(QApplication::translate("BIClass", "Tips", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("BIClass", "Mode", 0, QApplication::UnicodeUTF8));
        manualModeRadioButton->setText(QApplication::translate("BIClass", "Manual", 0, QApplication::UnicodeUTF8));
        semimanualModeRadioButton->setText(QApplication::translate("BIClass", "Semimanual", 0, QApplication::UnicodeUTF8));
        semiautomaticModeRadioButton->setText(QApplication::translate("BIClass", "Semiautomatic", 0, QApplication::UnicodeUTF8));
        shotAngleLabel->setText(QApplication::translate("BIClass", "Shot angle:", 0, QApplication::UnicodeUTF8));
        gotoButton->setText(QApplication::translate("BIClass", "Go To", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("BIClass", "Common Settings", 0, QApplication::UnicodeUTF8));
        screenAnalysisButton->setText(QApplication::translate("BIClass", "Screen Analysis", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("BIClass", "Add-ins", 0, QApplication::UnicodeUTF8));
        cueButton->setText(QApplication::translate("BIClass", "Cue Shot", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BIClass: public Ui_BIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BI_H
