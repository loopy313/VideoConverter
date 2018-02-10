#ifndef VIDEOINFO_H
#define VIDEOINFO_H
#include<QString>
#include<QSize>
#include<QFileInfo>
#include<QTime>

extern "C"{
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libswscale/swscale.h>
#include<libavutil/imgutils.h>
}
class videoInfo
{
private:
	void setVideoStream();
private:
	char* _filename=nullptr;
	AVFormatContext *pFormatCtx = nullptr;
	AVStream* videostream;
	int videoStreamIdx;
	int audioStream;
	QString filename;
	QString videoformat;
	QSize screensize;
	QTime duration;
	quint64 filesize;
	QString subtitle;
	QFileInfo fileinfo;
public:
	videoInfo(QString filename);
	//~videoInfo();
	//static videoInfo& getInstance();
	//videoInfo(videoInfo const& src)=delete;
	//videoInfo(videoInfo&& src)=delete;
	//void operator=(videoInfo const& src)=delete;
	//void operator=(videoInfo&& src)=delete;
	//void initialize(QString filename);
	void ExtractThumbnail();
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

};

#endif // VIDEOINFO_H
