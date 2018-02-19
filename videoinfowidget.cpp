#include "videoinfowidget.h"
#include "ui_videoinfowidget.h"
#include<QFontMetrics>
#include<QSizePolicy>
#include<QMouseEvent>
#include<QDebug>
#include<QWheelEvent>

videoinfoWidget::videoinfoWidget(QString filename,QWidget *parent) :
	QWidget(parent),v(filename),
	ui(new Ui::videoinfoWidget)
{
	ui->setupUi(this);
	setMouseTracking(true);
	QSizePolicy sp_retain = ui->closeBtn->sizePolicy();
	sp_retain.setRetainSizeWhenHidden(true);
	ui->closeBtn->setSizePolicy(sp_retain);

	PlayBtn = new QPushButton(this);
	PlayBtn->setObjectName(QStringLiteral("Play"));
	QIcon icon;
	icon.addFile(QStringLiteral(":/icons/play-button (1).png"), QSize(), QIcon::Normal, QIcon::Off);
	PlayBtn->setIcon(icon);
	PlayBtn->setIconSize(QSize(32, 32));
	PlayBtn->setFlat(true);
	PlayBtn->setStyleSheet(QStringLiteral("background:transparent"));
	ui->gridLayout_4->addWidget(PlayBtn, 1, 0, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
	PlayBtn->installEventFilter(this);
	PlayBtn->hide();
	ui->closeBtn->hide();

	connect(ui->editBtn,&QToolButton::clicked,[=](){ui->destFilename->setReadOnly(false);});
	connect(ui->destFilename,&QLineEdit::editingFinished,[=](){ui->destFilename->setReadOnly(true);});
}
videoinfoWidget::~videoinfoWidget()
{
	delete ui;
}

void videoinfoWidget::setInfo()
{
	v.setfileName();
	v.setvideoFormat();
	v.setdisplaySize();
	v.setDuration();
	v.setfileSize();
	v.ExtractThumbnail();
}

void videoinfoWidget::setUiInfo()
{
	setInfo();
	ui->srcFormat->setText(v.getvideoFormat());
	ui->srcScreensize->setText(v.getdisplaySize());
	ui->srcDuration->setText(v.getDuration());
	ui->srcFilesize->setText(v.getfileSize());
	setElidedText(ui->srcFilename,v.getfileName());
	setElidedText(ui->destFilename,v.getfileName());
	ui->label_2->setPixmap(v.getThumbnail());
}

void videoinfoWidget::enterwindow()
{
	ui->closeBtn->show();
	PlayBtn->show();
}

void videoinfoWidget::leavewindow()
{
	ui->closeBtn->hide();
	PlayBtn->hide();
}

int videoinfoWidget::getHeight()
{
	return this->size().height();
}

bool videoinfoWidget::eventFilter(QObject *watched, QEvent *event)
{
	if(watched==PlayBtn)
	{
		if(playPos.isNull())
		{
			playPos=PlayBtn->geometry().topLeft();
			playSize=PlayBtn->size();
		}
		if(event->type()==QEvent::MouseButtonPress)
		{
			QMouseEvent* mouseevent=static_cast<QMouseEvent*>(event);
			PlayBtn->setGeometry(playPos.x()+2,playPos.y()+2,playSize.width(),playSize.height());
			mouseevent->accept();
			return true;
		}
		else if(event->type()==QEvent::MouseButtonRelease)
		{
			QMouseEvent* mouseevent=static_cast<QMouseEvent*>(event);
			PlayBtn->setGeometry(playPos.x(),playPos.y(),playSize.width(),playSize.height());
			mouseevent->accept();
			return true;
		}
	}

	return QWidget::eventFilter(watched,event);
}

bool videoinfoWidget::event(QEvent *event)
{
	if(event->type()==QEvent::Leave)
	{
		leavewindow();
		return true;
	}
	return false;
}

void videoinfoWidget::setElidedText(QLineEdit* edit, const QString &text){
	QFontMetrics metrix(edit->font());
	int width = edit->width() - 2;
	QString clippedText = metrix.elidedText(text, Qt::ElideLeft, width);
	edit->setText(clippedText);
}
