#include "videoenncoder.h"
#include "videoinfowidget.h"
#include <QApplication>
#include <QGraphicsDropShadowEffect>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	VideoEnncoder w;
	w.show();

	return a.exec();
}
