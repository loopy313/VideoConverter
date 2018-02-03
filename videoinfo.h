#ifndef VIDEOINFO_H
#define VIDEOINFO_H
#include<QString>

class videoInfo
{
private:
	char* filename;
	//QString filename;
	QString mediaFormt;
	videoInfo(QString filename);
	videoInfo(videoInfo& src);
	void operator=(videoInfo const&);
public:
	static videoInfo* videoInfoManager;
	//static videoInfo *getInstance();
	qint64 Duration();
	int ExtractThumbnail();
};

#endif // VIDEOINFO_H
