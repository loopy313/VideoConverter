#ifndef VIDEOINFO_H
#define VIDEOINFO_H
#include<QString>
#include<QSize>
#include<QFileInfo>
#include<QTime>
#include<QImage>
#include<QPixmap>
extern "C"{
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libswscale/swscale.h>
#include<libavutil/imgutils.h>
}
class videoInfo
{
private:
	void setVideoCodecContext();
private:
	char* _filename=nullptr;
	AVFormatContext *pFormatCtx = nullptr;
	AVCodecContext* pVCodecContext=nullptr;
	int videoStreamIdx;
	QString filename;
	QString videoformat;
	QSize screensize;
	QTime duration;
	quint64 filesize;
	QString subtitle;
	QFileInfo fileinfo;
	AVFrame *pFrameRGB;
	QPixmap p;
	QImage img;
	const int width=180,height=100;
public:
	videoInfo(QString filename);
	void ExtractThumbnail();
	QPixmap& getThumbnail();
	void setDuration();
	QString getDuration();
	void setvideoFormat();
	QString getvideoFormat();
	void setfileSize();
	QString getfileSize();
	void setdisplaySize();
	QString getdisplaySize();
	void setfileName();
	QString getfileName();
	//~videoInfo();
};

#endif // VIDEOINFO_H
