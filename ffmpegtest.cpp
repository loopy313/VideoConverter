#include "ffmpegtest.h"

extern "C"{
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libswscale/swscale.h>
#include<libavutil/imgutils.h>
}

ffmpegTest::ffmpegTest(QStringList& filelist)
{
	QByteArray stringbyte;
	int i=0;
	for(auto &filename : filelist)
	{
		stringbyte.append(filename.toUtf8().data());
		files.append(new char[stringbyte.size()]);
		strcpy_s(files[i],strlen(files[i]),stringbyte.data());
		stringbyte.clear();
		i++;
	}
	filename=&files[0];
	av_register_all();
}

ffmpegTest::~ffmpegTest()
{
}

void ffmpegTest::Thumbnail_to_Jpeg()
{
	AVFormatContext *pFormatCtx = NULL;
	int             videoStream;
	unsigned int	i;
	AVCodecContext  *pCodecCtx;
	AVCodec         *pCodec;
	AVFrame         *pFrame;
	AVPacket        packet;
	int             frameFinished;

	if(avformat_open_input(&pFormatCtx, *filename, NULL, NULL)!=0)
	{
		printf("can not open");
		return;

	}

	if(avformat_find_stream_info(pFormatCtx, NULL) <0)
	{
		printf("can not find");
		return;
	}

	av_dump_format(pFormatCtx, -1, *filename, 0);

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
		return;
	}

	pCodecCtx = pFormatCtx->streams[videoStream]->codec;

	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if ( pCodec == NULL )
	{
		return;
	}

	if( avcodec_open2(pCodecCtx, pCodec, NULL) <0 )
	{
		return;
	}

	pFrame = av_frame_alloc();
	if( pFrame == NULL )
	{
		return;
	}

	while( av_read_frame(pFormatCtx, &packet) >= 0 )
	{
		if( packet.stream_index == videoStream )
		{
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
			if ( frameFinished )
			{
				break;
			}
		}
	}
	AVCodec *jpegCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
	if (!jpegCodec) {
		return;
	}
	AVCodecContext *jpegContext = avcodec_alloc_context3(jpegCodec);
	if (!jpegContext) {
		return;
	}

	jpegContext->pix_fmt = AV_PIX_FMT_YUVJ420P;
	jpegContext->height = pFrame->height;
	jpegContext->width = pFrame->width;
	jpegContext->time_base=pCodecCtx->time_base;	//this is necessary because avcodec_open2 does not fail.
	if (avcodec_open2(jpegContext, jpegCodec, NULL) < 0) {
		return;
	}

	packet.data=nullptr;
	packet.size=0;
	av_init_packet(&packet);
	int gotFrame;

	if (avcodec_encode_video2(jpegContext, &packet, pFrame, &gotFrame) < 0) {
		return;
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
	av_free(pFrame);
	avcodec_close(pCodecCtx);
	avcodec_close(jpegContext);
	avformat_close_input(&pFormatCtx);
}
