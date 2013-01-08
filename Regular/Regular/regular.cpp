#include "stdafx.h"
#include "regular.h"

Regular::Regular(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	ui.listView->setModel(&filesModel);
}

Regular::~Regular()
{
}

QString newName (const QString& fullName, const QString& suffix)
{
	int ind = fullName.lastIndexOf(".");
	return fullName.mid(0,ind) + suffix + fullName.mid(ind);
}

bool Regular::slotProcess() 
{
	// delete from textEditFrom to textEditTo

	QString strFrom = ui.textEditFrom->toPlainText();
	QString strTo = ui.textEditTo->toPlainText();

	QStringListIterator it(filesModel.stringList());
	while(it.hasNext())
	{
		QString name = it.next();
		
		QFile file(name);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return false;
		QTextStream in(&file);
		
		QString str = in.readAll();
		QString strNew;
		int j = 0; 
		int jp = 0;
		int n = 0;
		while (((j = str.indexOf(strFrom, j)) != -1)&&(n!=-1)) {
			if ((n = str.indexOf(strTo, j+strFrom.count()))!=-1)
			{
				strNew.append(str.mid(jp,j-jp));
				j+=n-j+strTo.count();
				jp = j;
			}
		}
		strNew.append(str.mid(jp,str.count()-jp));

		file.close();
		QFile newfile(newName(name,"_new"));
		if (!newfile.open(QIODevice::ReadWrite | QIODevice::Text))
			return false;
		QTextStream out(&newfile);

		out<<strNew;
	}

	return true;
}

bool Regular::slotPath()
{
	QStringList fileList = QFileDialog::getOpenFileNames(this, tr("Choose Files"), "E:/MyDisc/HomeInfo/Jokes/");
	if (fileList.empty()) return false;
	
	filesModel.setStringList(fileList);
		
	return true;
}