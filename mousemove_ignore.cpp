#include "mousemove_ignore.h"
#include<QEvent>
mousemove_ignore::mousemove_ignore(QWidget *parent) : QWidget(parent)
{

}

void mousemove_ignore::mouseMoveEvent(QEvent *event)
{
	event->ignore();
}
