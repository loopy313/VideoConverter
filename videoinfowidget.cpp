#include "videoinfowidget.h"
#include "ui_videoinfowidget.h"

videoinfoWidget::videoinfoWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::videoinfoWidget)
{
	setWindowFlags(Qt::CustomizeWindowHint);

	ui->setupUi(this);
}
videoinfoWidget::~videoinfoWidget()
{
	delete ui;
}
