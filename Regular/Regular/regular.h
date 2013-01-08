#ifndef REGULAR_H
#define REGULAR_H

#include <QtGui/QDialog>
#include "ui_regular.h"

class Regular : public QDialog
{
	Q_OBJECT

public:
	Regular(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Regular();

private:
	Ui::RegularClass ui;
	QStringListModel filesModel;

private slots:
	bool slotProcess();	
	bool slotPath();
};

#endif // REGULAR_H
