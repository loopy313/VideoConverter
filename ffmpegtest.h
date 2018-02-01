#ifndef FFMPEGTEST_H
#define FFMPEGTEST_H

#include<QStringList>
#include<QList>
#include<QByteArray>

class ffmpegTest
{
private:
	QList<char*> files;
	char** filename;
public:
	explicit ffmpegTest(QStringList& files);
	~ffmpegTest();
	void Thumbnail_to_Jpeg();
};

#endif // FFMPEGTEST_H
