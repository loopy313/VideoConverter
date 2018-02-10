#include "videoinfowidget.h"
#include "ui_videoinfowidget.h"

videoinfoWidget::videoinfoWidget(QString filename,QWidget *parent) :
	QWidget(parent),v(filename),
	ui(new Ui::videoinfoWidget)
{
	setWindowFlags(Qt::CustomizeWindowHint);
	ui->setupUi(this);
}
videoinfoWidget::~videoinfoWidget()
{
	delete ui;
}

void videoinfoWidget::setUiInfo()
{
	QString str;
	setInfo();
	str=v.getDuration();
	str=v.getdisplaySize();
	ui->srcFilename->setText(v.getfileName());
	ui->destFilename->setText(v.getfileName());
	ui->srcFormat->setText(v.getvideoFormat());
	ui->srcScreensize->setText(v.getdisplaySize());
	ui->srcDuration->setText(v.getDuration());
	ui->srcFilesize->setText(v.getfileSize());
}

void videoinfoWidget::setInfo()
{
	v.setfileName();
	v.setvideoFormat();
	v.setdisplaySize();
	v.setDuration();
	v.setfileSize();
}

