#include "videoprocess.h"
#include <QByteArray>
#include<QFile>
extern "C"{
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libswscale/swscale.h>
#include<libavutil/imgutils.h>
}

videoProcess::videoProcess()
{
	av_register_all();
}

int videoProcess::extractThumbnail(const char* _filename)
{

	int numBytes;

	numBytes = avpicture_get_size(AV_PIX_FMT_BGR24, ->width, pCodecCtx->height);
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
