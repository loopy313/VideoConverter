#ifndef VIDEOENNCODER_H
#define VIDEOENNCODER_H

#include <QWidget>
#include<QSignalMapper>
#include<QObjectList>
#include<QLayout>
#include<QStringList>
#include<QSharedPointer>
#include<QListWidget>
#include<QListWidgetItem>
#include<QVector>
#include "videoinfowidget.h"

class QPushButton;
class videoinfoWidget;
class QDesktopWidget;
namespace Ui {
class VideoEnncoder;
}

class VideoEnncoder : public QWidget
{
	Q_OBJECT
private:
	int lastIdx=0;
	Ui::VideoEnncoder *ui;
	QSignalMapper* signalMapper;
	QPushButton* RestoreBtn;
	QSize windowsize;
	int idx;
	QRect Screen;
	QLayoutItem* replaceBtn;
	QPoint m_pos;
	QVBoxLayout* vBoxlayout=nullptr;
	videoinfoWidget* temp=nullptr;
	QVector<videoinfoWidget*> widgetlists;
	QVector<QListWidgetItem*> listitems;
	QScrollBar* verticalScroll;
	int vmax;
public:
	QStringList fileList;
	explicit VideoEnncoder(QWidget *parent = 0);
	~VideoEnncoder();	
signals:
	void fileListChanged();
public slots:
	void findFiles();
	void showVideoInfo();
	void stackSelect(int num);
	void entereditem(QListWidgetItem* item);
protected:
	void changeEvent(QEvent* event);
	bool eventFilter(QObject *watched, QEvent *event);
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
};

#endif // VIDEOENNCODER_H
