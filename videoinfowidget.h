#ifndef VIDEOINFOWIDGET_H
#define VIDEOINFOWIDGET_H

#include <QWidget>
#include "videoinfo.h"

namespace Ui {
class videoinfoWidget;
}

class videoinfoWidget : public QWidget
{
	Q_OBJECT
private:
	Ui::videoinfoWidget *ui;
	char** pixmap=nullptr;
	videoInfo v;
private:
	void setInfo();
public:
	explicit videoinfoWidget(QString filename,QWidget *parent = 0);
	~videoinfoWidget();
public:
	void setUiInfo();
};

#endif // VIDEOINFOWIDGET_H
