#include "Grapher.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	try
	{
		QApplication a(argc, argv);
		
		QTextCodec *codec = QTextCodec::codecForName("CP1251");	// кодировка в Windows
		QTextCodec::setCodecForCStrings(codec);

		Grapher w;
		w.show();
	
		return a.exec();
	}
	catch (...)
	{
	
	}
	return 0;
}
