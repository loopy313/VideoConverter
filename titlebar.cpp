#include "titlebar.h"
#include "ui_videoenncoder.h"

titlebar::titlebar(QWidget *parent) :
	QWidget(parent),m_parent(parent)
{

}

void titlebar::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons()&Qt::LeftButton)
	{
		m_parent->move(event->globalPos()-m_pos);
		event->accept();
	}
}

void titlebar::mousePressEvent(QMouseEvent *event)
{

	if(event->button() == Qt::LeftButton)
	{
		m_pos = event->globalPos() - m_parent->geometry().topLeft();
		event->accept();
	}
}

void titlebar::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(m_parent->isMaximized())
		m_parent->showNormal();
	else
		m_parent->showMaximized();
}

