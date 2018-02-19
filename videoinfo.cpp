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

videoInfo::videoInfo(QString filename):
	fileinfo(filename)
{
	img=QImage(width,height,QImage::Format_RGB888);
	QByteArray temp=filename.toUtf8();
	const char* str=temp.data();
	this->_filename=const_cast<char*>(str);

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
	setVideoCodecContext();
}

void videoInfo::setVideoCodecContext()
{
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
	pVCodecContext=pFormatCtx->streams[videoStreamIdx]->codec;
}

QPixmap& videoInfo::getThumbnail()
{
	return p=QPixmap::fromImage(img);
}

void videoInfo::ExtractThumbnail()
{
	AVFrame         *pFrame;
	AVCodec         *pCodec;
	int             numBytes;
	AVPacket        packet;
	int             frameFinished;
	int				videoStream=videoStreamIdx;
	SwsContext*		img_convert_ctx=nullptr;
	uint8_t *buffer=nullptr;

	pCodec = avcodec_find_decoder(pVCodecContext->codec_id);
	if ( pCodec == NULL )
	{
		qDebug()<<"decoder not found";
	}

	if( avcodec_open2(pVCodecContext, pCodec, NULL) <0 )
	{
		qDebug()<<"codec open error";
	}

	pFrame = av_frame_alloc();
	if( pFrame == NULL )
	{
		qDebug()<<"frame alloc error";
	}

	pFrameRGB = av_frame_alloc();
	if( pFrameRGB == NULL )
	{
		qDebug()<<"frame alloc error";
	}
	numBytes=av_image_get_buffer_size(AV_PIX_FMT_RGB24,width,height,1);
	buffer = (uint8_t*)av_malloc(numBytes);

	av_image_fill_arrays(pFrameRGB->data,pFrameRGB->linesize,buffer,AV_PIX_FMT_RGB24,width,height,1);

	img_convert_ctx = sws_getContext(pVCodecContext->width, pVCodecContext->height,
									 pVCodecContext->pix_fmt, width, height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
	if (!img_convert_ctx )
	{
		qDebug()<<"getCachedContext failed.";
	}
	while(av_read_frame(pFormatCtx, &packet) >= 0 )
	{
		if(packet.stream_index == videoStream )
		{
			avcodec_decode_video2(pVCodecContext, pFrame, &frameFinished, &packet);
			if ( frameFinished )
			{
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pVCodecContext->height,
						  pFrameRGB->data, pFrameRGB->linesize);

				for( int y = 0; y<height; ++y )
				{
					memcpy(img.scanLine(y), pFrameRGB->data[0]+(y*pFrameRGB->linesize[0]), pFrameRGB->linesize[0]);
				}
				break;
			}
		}
	}
}
void videoInfo::setDuration()
{
	int toSecond=pFormatCtx->duration/1000/1000;
	int hour=toSecond/3600;
	int minute=(toSecond-(hour*3600))/60;
	int second=toSecond%60;
	duration=QTime(hour,minute,second);
}

QString videoInfo::getDuration()
{

	return duration.toString("hh:mm:ss");
}

void videoInfo::setdisplaySize()
{
	screensize.setHeight(pVCodecContext->height);
	screensize.setWidth(pVCodecContext->width);

}

QString videoInfo::getdisplaySize()
{
	return QString("%1*%2").arg(QString::number(screensize.width()),QString::number(screensize.height()));
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


void videoInfo::setfileName()
{
	filename=fileinfo.fileName();
}

QString videoInfo::getfileName()
{
	return filename;
}
