#include "stdafx.h"
#include "regular.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Regular w;
	w.show();
	return a.exec();
}
