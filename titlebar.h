#ifndef TITLEBAR_H
#define TITLEBAR_H

#include<QMouseEvent>
#include<QWidget>

class titlebar : public QWidget
{
	Q_OBJECT
private:
	QWidget* m_parent;
	QPoint m_pos;
public:
	explicit titlebar(QWidget *parent = 0);
protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);
};


#endif
