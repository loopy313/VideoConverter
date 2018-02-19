#include "videoenncoder.h"
#include "ui_videoenncoder.h"
#include "ffmpegtest.h"
#include "videoinfowidget.h"
#include<QFileDialog>
#include<QDesktopWidget>
#include<Windows.h>
#include<QDebug>
#include<QTimer>
#include<QScrollBar>

VideoEnncoder::VideoEnncoder(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::VideoEnncoder),
	signalMapper(new QSignalMapper(this))
{
	setWindowFlags(Qt::FramelessWindowHint);
	ui->setupUi(this);
	Screen=QApplication::desktop()->screenGeometry();
	windowsize=this->size();
	verticalScroll=ui->listWidget->verticalScrollBar();
	RestoreBtn=new QPushButton(ui->title);
	QIcon restoreIcon;
	restoreIcon.addFile(QStringLiteral(":/icons/restore.png"));
	RestoreBtn->setObjectName(tr("RestoreIcon"));
	RestoreBtn->setIcon(restoreIcon);
	RestoreBtn->setFlat(true);
	RestoreBtn->hide();

	for(int i=0;i<ui->baseIcons->children().count();++i)
	{
		QPushButton* mainbtn=qobject_cast<QPushButton*>(ui->horizontalLayout_3->itemAt(i)->widget());
		connect(mainbtn,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
		signalMapper->setMapping(mainbtn,i);
	}
	ui->title->installEventFilter(this);
	ui->baseIcons->installEventFilter(this);
	ui->titleIcons->installEventFilter(this);
	ui->listWidget->installEventFilter(this);

	connect(RestoreBtn,SIGNAL(clicked(bool)),this,SLOT(showNormal()));
	connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(stackSelect(int)));
	connect(ui->AddFileBtn,&QPushButton::clicked,this,&VideoEnncoder::findFiles);
	connect(ui->ConvertingDilsplayBtn,&QPushButton::clicked,this,&VideoEnncoder::showVideoInfo);
	connect(ui->listWidget,SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(entereditem(QListWidgetItem*)));

	connect(this,&VideoEnncoder::fileListChanged,&VideoEnncoder::showVideoInfo);
	connect(ui->ConvertedDisplayBtn,&QPushButton::clicked,ui->listWidget,&QListWidget::clear);
}

VideoEnncoder::~VideoEnncoder()
{
	delete ui;
}

void VideoEnncoder::findFiles()
{
	QStringList files;
	files=QFileDialog::getOpenFileNames(this,tr("files explorer"),"d:/EdgeDownload",tr("Video Files (*.mp4 *.mkv *.avi)"));

	if(!files.isEmpty())
	{
		fileList+=files;
		emit fileListChanged();
	}
}

void VideoEnncoder::showVideoInfo()
{

	for(int i=this->lastIdx;i<fileList.count();++i)
	{
		listitems.push_back(new QListWidgetItem(ui->listWidget));
		widgetlists.push_back(new videoinfoWidget(fileList[i],this));

	}
	for(int i=this->lastIdx;i<widgetlists.count();++i)
	{
		widgetlists[i]->setUiInfo();
	}
	for(int i=this->lastIdx;i<fileList.count();++i)
	{
		listitems[i]->setSizeHint(widgetlists[i]->size());
		ui->listWidget->setItemWidget(listitems[i],widgetlists[i]);
	}
	this->lastIdx=fileList.count();
}

void VideoEnncoder::stackSelect(int num)
{
	ui->stackedWidget->setCurrentIndex(num);
	ui->stackedWidget->show();
}

void VideoEnncoder::entereditem(QListWidgetItem *item)
{
	if(temp)
	{
		temp->leavewindow();
		temp=nullptr;
	}
	static_cast<videoinfoWidget*>(ui->listWidget->itemWidget(item))->enterwindow();
	temp=static_cast<videoinfoWidget*>(ui->listWidget->itemWidget(item));
}

void VideoEnncoder::changeEvent(QEvent *event)
{
	QWindowStateChangeEvent* e = static_cast< QWindowStateChangeEvent* >( event );
	if(event->type()==QEvent::WindowStateChange)
	{
		if(isMaximized() )
		{
			ui->MaximizeBtn->hide();
			replaceBtn=ui->horizontalLayout_7->replaceWidget(ui->MaximizeBtn,this->RestoreBtn);
			RestoreBtn->show();

		}

		else if(e->oldState()==Qt::WindowMaximized && this->windowState()==Qt::WindowNoState)
		{
			RestoreBtn->hide();
			QPushButton* maxBtn=qobject_cast<QPushButton*>(replaceBtn->widget());
			ui->horizontalLayout_7->replaceWidget(this->RestoreBtn,maxBtn);
			ui->MaximizeBtn->show();
		}

	}
}

//title window's style sheet ignored,if return true;
bool VideoEnncoder::eventFilter(QObject *watched, QEvent *event)
{
	if(watched==ui->titleIcons || watched==ui->baseIcons)
	{
		if(event->type()==QEvent::MouseMove || event->type()==QEvent::MouseButtonDblClick)
		{
			QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
			//It's not propagated to the parent.
			mouseEvent->accept();
			return true;
		}
	}
	else if(watched==ui->title)
	{
		static QPoint MoveFrom;
		QPoint bias;
		if(event->type()==QEvent::MouseButtonPress)
		{
			QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
			if(mouseEvent->button() == Qt::LeftButton)
			{
				if(this->windowState()&Qt::WindowMaximized)
				{
					MoveFrom=mouseEvent->globalPos();
					mouseEvent->accept();
				}
				m_pos = mouseEvent->globalPos() - this->geometry().topLeft();
			}
			mouseEvent->accept();
		}
		else if(event->type()==QEvent::MouseMove)
		{
			QPoint MoveTo;
			QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);

			if(mouseEvent->buttons()&Qt::LeftButton)
			{
				//window move(max->normal). use only on single monitor.
				if(this->windowState()&Qt::WindowMaximized)
				{
					int topleft_x;
					int topleft_y;

					this->setWindowState(this->windowState() ^ Qt::WindowMaximized);
					MoveTo=mouseEvent->globalPos();

					if(windowsize.width()/2+MoveTo.x()>=Screen.topRight().x())//right
					{
						topleft_x=Screen.topRight().x()-windowsize.width();
						bias=QPoint(-5,5);
					}
					else if(MoveFrom.x()-windowsize.width()/2>=Screen.topLeft().x())//left
					{
						topleft_x=MoveTo.x()-windowsize.width()/2;
						bias=QPoint(0,5);
					}
					else if(MoveFrom.x()<=windowsize.width()/2)//center
					{
						topleft_x=Screen.topLeft().x();
						bias=QPoint(5,5);
					}
					topleft_y=MoveTo.y()-MoveFrom.y();
					this->setGeometry(topleft_x,topleft_y,windowsize.width(),windowsize.height());

					m_pos = mouseEvent->globalPos() - this->geometry().topLeft()-bias;
					ui->MaximizeBtn->show();
					this->RestoreBtn->hide();
				}
				//widgetPos+(MovePos-pressPos)
				this->move(mouseEvent->globalPos()-m_pos);
				mouseEvent->accept();
			}
		}
		else if(event->type()==QEvent::MouseButtonDblClick)
		{
			QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
			if(mouseEvent->button()==Qt::LeftButton)
			{
				if(this->isMaximized())
					this->showNormal();
				else
					this->showMaximized();
			}
			mouseEvent->accept();
		}
	}
	else if(watched==ui->listWidget)
	{
		if(event->type()==QEvent::Resize)
		{
			verticalScroll->setValue(0);
		}
	}
	return false;
}


bool VideoEnncoder::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	UNREFERENCED_PARAMETER(eventType);
	MSG* msg=static_cast<MSG*>(message);
	LRESULT lresult;

	if(msg->message==WM_NCHITTEST)
	{
		lresult=DefWindowProc(msg->hwnd,msg->message,msg->wParam,msg->lParam);

		int x=msg->lParam&0x0000ffff;
		int y=(msg->lParam>>16)&0x0000ffff;
		if(lresult==HTCLIENT)
		{
			x=x-this->pos().x();
			y=y-this->pos().y();
			QRect rect=this->rect();
			if(x>this->rect().topRight().x()-5 && y<this->rect().topRight().y()+5)
			{
				*result=HTTOPRIGHT;
				return true;
			}
			else if(x<this->rect().topLeft().x()+5 && y<this->rect().topLeft().y()+5)
			{
				*result=HTTOPLEFT;
				return true;
			}
			else if(x>this->rect().bottomRight().x()-5 && y>this->rect().bottomRight().y()-5)
			{
				*result=HTBOTTOMRIGHT;
				return true;
			}
			else if(x<this->rect().bottomLeft().x()+5 && y>this->rect().bottomLeft().y()-5)
			{
				*result=HTBOTTOMLEFT;
				return true;
			}
			else if(x>this->rect().topRight().x()-5 && x<this->rect().topRight().x()+5)
			{
				*result=HTRIGHT;
				return true;
			}
			else if(x>this->rect().topLeft().x()-5 && x<this->rect().topLeft().x()+5)
			{
				*result=HTLEFT;
				return true;
			}
			else if(y>this->rect().top()-5 && y<this->rect().top()+5)
			{
				*result=HTTOP;
				return true;
			}
			else if(y>this->rect().bottom()-5 && y<this->rect().bottom()+5)
			{
				*result=HTBOTTOM;
				return true;
			}
		}
	}
	return false;
}







