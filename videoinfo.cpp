#include "videoinfo.h"
#include<assert.h>
#include <iostream>
#include<QTextCodec>
#include<QFile>
#include<QString>
#include<QDataStream>
extern "C"{
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libswscale/swscale.h>
#include<libavutil/imgutils.h>
}
videoInfo::videoInfo(QString filename)
{
	QByteArray temp=filename.toUtf8().data();
	this->filename=new char[temp.size()];
	strcpy_s(this->filename,strlen(this->filename),temp.data());
	av_register_all();

}

videoInfo::videoInfo(videoInfo &src)
{

}

void videoInfo::operator=(const videoInfo &)
{

}
/*
videoInfo *videoInfo::getInstance()
{
	if(!videoInfoManager)
	{
		videoInfoManager=new videoInfo()
	}
}
*/
qint64 videoInfo::Duration() {

	AVFormatContext *pFormatCtx = NULL;
	int             videoStream;
	unsigned int	i;
	AVCodecContext  *pCodecCtx;
	AVCodec         *pCodec;
	AVFrame         *pFrame;
	AVFrame         *pFrameRGB;
	AVPacket        packet;
	int             frameFinished;
	struct SwsContext *img_convert_ctx = NULL;

	pFormatCtx=avformat_alloc_context();
	if(avformat_open_input(&pFormatCtx, filename, NULL, NULL)!=0)
	{
		printf("can not open");
		return -1;
	}

	if(avformat_find_stream_info(pFormatCtx, NULL) <0)
	{
		printf("can not find");
		return -1;
	}

	av_dump_format(pFormatCtx, -1, filename, 0);

	videoStream = -1;
	for (i=0;i<pFormatCtx->nb_streams;i++)
	{
		if( pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}

	if ( videoStream == -1 )
	{
		printf("not find videoStream");
		return -1;
	}

	pCodecCtx = pFormatCtx->streams[videoStream]->codec;

	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if ( pCodec == NULL )
	{
		return -1;
	}

	if( avcodec_open2(pCodecCtx, pCodec, NULL) <0 )
	{
		return -1;
	}

	pFrame = av_frame_alloc();
	if( pFrame == NULL )
	{
		return -1;
	}

	pFrameRGB = av_frame_alloc();
	if( pFrameRGB == NULL )
	{
		return -1;
	}

	//numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

	//buffer = (uint8_t*)av_malloc(numBytes);

	//avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);


	img_convert_ctx = sws_getCachedContext(img_convert_ctx, pCodecCtx->width, pCodecCtx->height,
										   pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUVJ420P, SWS_BILINEAR, NULL, NULL, NULL);

	while( av_read_frame(pFormatCtx, &packet) >= 0 )
	{
		if( packet.stream_index == videoStream )
		{
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
			if ( frameFinished )
			{

				if (!img_convert_ctx )
				{
					fprintf(stderr, "Cannot initialize sws conversion context\n");
					exit(1);
				}

				//sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
						 // pFrameRGB->data, pFrameRGB->linesize);
				break;
			}
		}
	}
	AVCodec *jpegCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
	if (!jpegCodec) {
		return -1;
	}
	AVCodecContext *jpegContext = avcodec_alloc_context3(jpegCodec);
	if (!jpegContext) {
		return -1;
	}

	jpegContext->pix_fmt = AV_PIX_FMT_YUVJ420P;
	jpegContext->height = pFrame->height;
	jpegContext->width = pFrame->width;
	jpegContext->time_base=pCodecCtx->time_base;	//this is necessary because avcodec_open2 does not fail.
	if (avcodec_open2(jpegContext, jpegCodec, NULL) < 0) {
		return -1;
	}

	packet.data=nullptr;
	packet.size=0;
	av_init_packet(&packet);
	int gotFrame;

	if (avcodec_encode_video2(jpegContext, &packet, pFrame, &gotFrame) < 0) {
		return -1;
	}

	QString Jpegfilename;
	int FrameNo=1;
	Jpegfilename=QString("dvr-%1.jpeg").arg(QString::number(FrameNo),6,'0');
	QFile JpegFile(Jpegfilename);
	JpegFile.open(QIODevice::ReadWrite);
	QDataStream data(&JpegFile);
	data.writeRawData((char*)packet.data,packet.size);
	JpegFile.close();

	av_free_packet(&packet);
	//av_free(buffer);
	av_free(pFrame);
	//av_free(pFrameRGB);
	avcodec_close(pCodecCtx);
	avcodec_close(jpegContext);
	avformat_close_input(&pFormatCtx);
	return 0;
}

int videoInfo::ExtractThumbnail()
{

	AVFormatContext *pFormatCtx = NULL;
	int             videoStream;
	unsigned int	i;
	AVCodecContext  *pCodecCtx;
	AVCodec         *pCodec;
	AVFrame         *pFrame;
	AVFrame         *pFrameRGB;
	int             numBytes;
	uint8_t         *buffer;
	AVPacket        packet;
	int             frameFinished;
	struct SwsContext *img_convert_ctx = NULL;

	if(avformat_open_input(&pFormatCtx, filename, NULL, NULL)!=0)
	{
		printf("can not open");
		return -1;
	}

	if(avformat_find_stream_info(pFormatCtx, NULL) <0)
	{
		printf("can not find");
		return -1;
	}

	av_dump_format(pFormatCtx, -1, filename, 0);

	videoStream = -1;
	for (i=0;i<pFormatCtx->nb_streams;i++)
	{
		if( pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}

	if ( videoStream == -1 )
	{
		printf("not find videoStream");
		return -1;
	}

	pCodecCtx = pFormatCtx->streams[videoStream]->codec;

	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if ( pCodec == NULL )
	{
		return -1;
	}

	if( avcodec_open2(pCodecCtx, pCodec, NULL) <0 )
	{
		return -1;
	}

	pFrame = av_frame_alloc();
	if( pFrame == NULL )
	{
		return -1;
	}

	pFrameRGB = av_frame_alloc();
	if( pFrameRGB == NULL )
	{
		return -1;
	}

	numBytes = avpicture_get_size(AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);

	buffer = (uint8_t*)av_malloc(numBytes);

	avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);


	img_convert_ctx = sws_getCachedContext(img_convert_ctx, pCodecCtx->width, pCodecCtx->height,
										   pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUVJ420P, SWS_BILINEAR, NULL, NULL, NULL);

	while( av_read_frame(pFormatCtx, &packet) >= 0 )
	{
		if( packet.stream_index == videoStream )
		{
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
			if ( frameFinished )
			{

				if (!img_convert_ctx )
				{
					fprintf(stderr, "Cannot initialize sws conversion context\n");
					exit(1);
				}

				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
						  pFrameRGB->data, pFrameRGB->linesize);
				break;
			}
		}
	}
	AVCodec *jpegCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
	if (!jpegCodec) {
		return -1;
	}
	AVCodecContext *jpegContext = avcodec_alloc_context3(jpegCodec);
	if (!jpegContext) {
		return -1;
	}

	jpegContext->pix_fmt = AV_PIX_FMT_YUVJ420P;
	jpegContext->height = pFrame->height;
	jpegContext->width = pFrame->width;
	jpegContext->time_base=pCodecCtx->time_base;
	if (avcodec_open2(jpegContext, jpegCodec, NULL) < 0) {
		return -1;
	}

	packet.data=nullptr;
	packet.size=0;
	av_init_packet(&packet);
	int gotFrame;

	if (avcodec_encode_video2(jpegContext, &packet, pFrame, &gotFrame) < 0) {
		return -1;
	}

	QString Jpegfilename;
	int FrameNo=1;
	Jpegfilename=QString("dvr-%1.jpeg").arg(QString::number(FrameNo),6,'0');
	QFile JpegFile(Jpegfilename);
	JpegFile.open(QIODevice::ReadWrite);
	QDataStream data(&JpegFile);
	data.writeRawData((char*)packet.data,packet.size);
	JpegFile.close();

	av_free_packet(&packet);
	av_free(buffer);
	av_free(pFrame);
	av_free(pFrameRGB);
	avcodec_close(pCodecCtx);
	avcodec_close(jpegContext);
	avformat_close_input(&pFormatCtx);
	return 0;
}


