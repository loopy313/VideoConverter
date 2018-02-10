#include "videoinfo.h"
#include<assert.h>
#include <iostream>
#include<QTextCodec>
#include<QFile>
#include<QDebug>
#include<QString>
#include<QStringList>
#include<QDataStream>
#include<QTime>
#include<QFileInfo>
#include "videoenncoder.h"

/*
//c++11 6.7 [stmt.dcl] p4
videoInfo &videoInfo::getInstance()
{
	static videoInfo v;
	return v;
}
*/

videoInfo::videoInfo(QString filename):fileinfo(filename)
{
	QByteArray temp=filename.toUtf8().data();
	this->_filename=new char[temp.size()];
	strcpy_s(this->_filename,strlen(this->_filename),temp.data());
	av_register_all();
	pFormatCtx=avformat_alloc_context();
	if(avformat_open_input(&pFormatCtx, this->_filename, NULL, NULL)!=0)
	{
		qDebug()<<"file not open";
	}

	if(avformat_find_stream_info(pFormatCtx, NULL) <0)
	{
		qDebug()<<"can't find stream";
	}
	av_dump_format(pFormatCtx, -1, this->_filename, 0);
	setVideoStream();
}

void videoInfo::setVideoStream()
{
	AVStream* pAvStream;
	videoStreamIdx = -1;
	for (unsigned int i=0;i<pFormatCtx->nb_streams;i++)
	{
		if( pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStreamIdx = i;
			break;
		}
	}

	if ( videoStreamIdx == -1 )
	{
		qDebug()<<"not find videoStream";
	}

	pAvStream = pFormatCtx->streams[videoStreamIdx];
	videostream=pAvStream;
}

/*
void videoInfo::initialize(QString filename)
{

}
*/
void videoInfo::ExtractThumbnail()
{

}

void videoInfo::setDuration()
{
	int toSecond=pFormatCtx->duration/1000/1000;
	int hour=toSecond/3600;
	int minute=(toSecond-hour)/60;
	int second=toSecond%60;
	duration=QTime(hour,minute,second);
}

QString videoInfo::getDuration()
{

	return duration.toString("hh:mm:ss");
}

void videoInfo::setvideoFormat()
{
	videoformat=fileinfo.suffix();
	if(videoformat.isEmpty())
		videoformat=QString("UnKnown");
}

QString videoInfo::getvideoFormat()
{
	return videoformat;
}

void videoInfo::setfileSize()
{
	filesize=fileinfo.size();
}

QString videoInfo::getfileSize()
{
	float_t gbyte,mbyte,kbyte=0.0;
	if((gbyte=static_cast<float_t>(filesize/1024.0/1024.0/1024.0))>=1)
	{
		return QString::number(gbyte,'f',2)+QString("GB");
	}
	else if((mbyte=static_cast<float_t>(filesize/1024.0/1024.0))>=1)
	{
		return QString::number(mbyte,'f',2)+QString("MB");
	}
	else if((kbyte=static_cast<float_t>(filesize/1024.0))>=1)
	{
		return QString::number(kbyte,'f',2)+QString("KB");
	}
	else
	{
		return QString("%1+B").arg(filesize);
	}
}

void videoInfo::setdisplaySize()
{
	screensize.setHeight(videostream->codec->height);
	screensize.setWidth(videostream->codec->width);
}

QString videoInfo::getdisplaySize()
{
	return QString("%1*%2").arg(QString::number(screensize.width()),QString::number(screensize.height()));
}

void videoInfo::setfileName()
{
	filename=fileinfo.fileName();
}

QString videoInfo::getfileName()
{
	return filename;
}


