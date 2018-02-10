#include "videoenncoder.h"
#include "videoinfowidget.h"
#include "videoprocess.h"
#include <QApplication>
#include <QGraphicsDropShadowEffect>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//videoProcess v;
	//v.extractThumbnail("D:\\EdgeDownload\\1.mkv");
	VideoEnncoder w;
	w.show();

	return a.exec();
}
