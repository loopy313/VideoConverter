#ifndef VIDEOENNCODER_H
#define VIDEOENNCODER_H

#include <QWidget>
#include<QSignalMapper>
#include<QObjectList>
#include<QLayout>

#include "videoinfo.h"
#include "ffmpegtest.h"

class QPushButton;

namespace Ui {
class VideoEnncoder;
}

class VideoEnncoder : public QWidget
{
	Q_OBJECT
private:
	Ui::VideoEnncoder *ui;
	QSignalMapper* signalMapper;
	QRect baseSize;
	QPushButton* RestoreBtn;
	QSize windowsize;
	int idx;
	QRect Screen;
	QLayoutItem* replaceBtn;
private:
	QStringList fileList;
	QObjectList menuList;
	QPoint m_pos;
public:
	explicit VideoEnncoder(QWidget *parent = 0);
	~VideoEnncoder();	
signals:
	void fileListChanged();
public slots:
	void findFiles();
	void showVideoInfo();
	void extractThumbnail();
	void stackSelect(int num);
protected:
	void changeEvent(QEvent* event);
	bool eventFilter(QObject *watched, QEvent *event);
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
};

#endif // VIDEOENNCODER_H
