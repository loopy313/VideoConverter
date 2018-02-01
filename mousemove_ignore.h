#ifndef MOUSEMOVE_IGNORE_H
#define MOUSEMOVE_IGNORE_H

#include <QWidget>

class mousemove_ignore : public QWidget
{
	Q_OBJECT
public:
	explicit mousemove_ignore(QWidget *parent = nullptr);

signals:

public slots:

protected:
	void mouseMoveEvent(QEvent* event);
};

#endif // MOUSEMOVE_IGNORE_H
