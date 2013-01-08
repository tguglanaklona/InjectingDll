/********************************************************************************
** Form generated from reading UI file 'regular.ui'
**
** Created: Mon 1. Oct 18:40:32 2012
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGULAR_H
#define UI_REGULAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegularClass
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QTextEdit *textEditFrom;
    QGroupBox *groupBox_3;
    QTextEdit *textEditTo;
    QPushButton *pushButton;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer;
    QListView *listView;

    void setupUi(QDialog *RegularClass)
    {
        if (RegularClass->objectName().isEmpty())
            RegularClass->setObjectName(QString::fromUtf8("RegularClass"));
        RegularClass->resize(611, 596);
        horizontalLayoutWidget = new QWidget(RegularClass);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 80, 601, 111));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(horizontalLayoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        textEditFrom = new QTextEdit(groupBox);
        textEditFrom->setObjectName(QString::fromUtf8("textEditFrom"));
        textEditFrom->setGeometry(QRect(13, 20, 231, 71));

        horizontalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(horizontalLayoutWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        textEditTo = new QTextEdit(groupBox_3);
        textEditTo->setObjectName(QString::fromUtf8("textEditTo"));
        textEditTo->setGeometry(QRect(13, 20, 231, 71));

        horizontalLayout->addWidget(groupBox_3);

        pushButton = new QPushButton(RegularClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 230, 75, 23));
        horizontalLayoutWidget_2 = new QWidget(RegularClass);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 360, 591, 221));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton_2 = new QPushButton(horizontalLayoutWidget_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);

        listView = new QListView(horizontalLayoutWidget_2);
        listView->setObjectName(QString::fromUtf8("listView"));

        horizontalLayout_2->addWidget(listView);


        retranslateUi(RegularClass);
        QObject::connect(pushButton, SIGNAL(clicked()), RegularClass, SLOT(slotProcess()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), RegularClass, SLOT(slotPath()));

        QMetaObject::connectSlotsByName(RegularClass);
    } // setupUi

    void retranslateUi(QDialog *RegularClass)
    {
        RegularClass->setWindowTitle(QApplication::translate("RegularClass", "Regular", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("RegularClass", "Delete from", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("RegularClass", "Delete to", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("RegularClass", "Process", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("RegularClass", "Add Files", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RegularClass: public Ui_RegularClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGULAR_H
