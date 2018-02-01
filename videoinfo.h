#ifndef VIDEOINFO_H
#define VIDEOINFO_H
#include<QString>

class videoInfo
{
private:
	char* filename;
public:
	videoInfo(QString filename);
	qint64 getDuration();
	int ExtractThumbnail();
};

#endif // VIDEOINFO_H
