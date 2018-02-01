#ifndef VIDEOINFOWIDGET_H
#define VIDEOINFOWIDGET_H

#include <QWidget>

namespace Ui {
class videoinfoWidget;
}

class videoinfoWidget : public QWidget
{
	Q_OBJECT

public:
	explicit videoinfoWidget(QWidget *parent = 0);
	~videoinfoWidget();

private:
	Ui::videoinfoWidget *ui;
};

#endif // VIDEOINFOWIDGET_H
