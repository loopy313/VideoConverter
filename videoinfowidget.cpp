#include "videoinfowidget.h"
#include "ui_videoinfowidget.h"

videoinfoWidget::videoinfoWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::videoinfoWidget)
{
	QRect rt,tri_rt;
	QPoint pt,tr_pt;
	QSize size;
	ui->setupUi(this);


}
videoinfoWidget::~videoinfoWidget()
{
	delete ui;
}
