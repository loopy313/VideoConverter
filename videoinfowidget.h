#ifndef VIDEOINFOWIDGET_H
#define VIDEOINFOWIDGET_H

#include <QWidget>
#include<QLineEdit>
#include<QPushButton>
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
	QPushButton* PlayBtn;
	QPoint playPos;
	QSize playSize;
private:
	void setInfo();
	void setElidedText(QLineEdit* edit, const QString &text);
public:
	explicit videoinfoWidget(QString filename,QWidget *parent = 0);
	~videoinfoWidget();
public:
	void setUiInfo();
	void enterwindow();
	void leavewindow();
	int getHeight();
protected:
	bool eventFilter(QObject *watched, QEvent *event);
	bool event(QEvent *event);
};

#endif // VIDEOINFOWIDGET_H
