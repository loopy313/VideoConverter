#include "videoenncoder.h"
#include "ui_videoenncoder.h"
#include "ffmpegtest.h"
#include "videoinfowidget.h"
#include<QFileDialog>
#include<QDesktopWidget>
#include<Windows.h>

VideoEnncoder::VideoEnncoder(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::VideoEnncoder),
	signalMapper(new QSignalMapper(this))
{
	setWindowFlags(Qt::FramelessWindowHint);
	ui->setupUi(this);


	RestoreBtn=new QPushButton(ui->title);
	QIcon restoreIcon;
	restoreIcon.addFile(QStringLiteral(":/icons/restore.png"));
	RestoreBtn->setObjectName(tr("RestoreIcon"));
	RestoreBtn->setIcon(restoreIcon);
	RestoreBtn->setFlat(true);
	RestoreBtn->hide();
	windowsize=this->size();
	Screen = QApplication::desktop()->screenGeometry(this);

	for(int i=0;i<ui->baseIcons->children().count();++i)
	{
		QPushButton* mainbtn=qobject_cast<QPushButton*>(ui->horizontalLayout_3->itemAt(i)->widget());
		connect(mainbtn,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
		signalMapper->setMapping(mainbtn,i);
	}

	ui->title->installEventFilter(this);
	ui->baseIcons->installEventFilter(this);
	ui->titleIcons->installEventFilter(this);

	connect(RestoreBtn,SIGNAL(clicked(bool)),this,SLOT(showNormal()));
	connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(stackSelect(int)));
	connect(ui->AddFileBtn,&QPushButton::clicked,this,&VideoEnncoder::findFiles);
	connect(this,&VideoEnncoder::fileListChanged,&VideoEnncoder::showVideoInfo);
	connect(ui->ConvertingDilsplayBtn,&QPushButton::clicked,this,&VideoEnncoder::showVideoInfo);
}

VideoEnncoder::~VideoEnncoder()
{
	delete ui;
}

void VideoEnncoder::findFiles()
{
	fileList=QFileDialog::getOpenFileNames(this,tr("files explorer"),"d:/EdgeDownload",tr("Video Files (*.mp4 *.mkv *.avi)"));

	if(!fileList.isEmpty())
		emit fileListChanged();
}

void VideoEnncoder::showVideoInfo()
{
	//QThread thread(this);
	delete ui->verticalLayout_4;
	if(!vBoxlayout)
	{
		vBoxlayout=new QVBoxLayout(this);
		vBoxlayout->addSpacing(5);
	}
	for(auto& f : fileList)
	{
		widgetlists.push_back(new videoinfoWidget(f,this));
	}
	for(auto& l:widgetlists)
	{
		l->setUiInfo();
		vBoxlayout->addWidget(l);
	}
	ui->scrollAreaWidgetContents->setLayout(vBoxlayout);
}
void VideoEnncoder::extractThumbnail()
{
	ffmpegTest test(fileList);
	test.Thumbnail_to_Jpeg();
}

void VideoEnncoder::stackSelect(int num)
{
	ui->stackedWidget->setCurrentIndex(num);
	ui->stackedWidget->show();
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

bool VideoEnncoder::eventFilter(QObject *watched, QEvent *event)
{
	if(watched==ui->titleIcons || watched==ui->baseIcons)
	{
		if(event->type()==QEvent::MouseButtonPress || event->type()==QEvent::MouseMove || event->type()==QEvent::MouseButtonDblClick)
		{
			QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
			//It's not propagated to the parent.
			mouseEvent->accept();
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
				if(this->windowState()==Qt::WindowMaximized)
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
				if(this->windowState()==Qt::WindowMaximized)
				{
					int topleft_x;
					int topleft_y;

					this->setWindowState(this->windowState() ^ Qt::WindowMaximized);
					MoveTo=mouseEvent->globalPos();

					if(windowsize.width()/2+MoveTo.x()>=Screen.topRight().x())
					{
						topleft_x=Screen.topRight().x()-windowsize.width();
						bias=QPoint(-5,5);
					}
					else if(MoveFrom.x()-windowsize.width()/2>=Screen.topLeft().x())
					{
						topleft_x=MoveTo.x()-windowsize.width()/2;
						bias=QPoint(0,5);
					}
					else if(MoveFrom.x()<=windowsize.width()/2)
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




