#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H
#include "videoenncoder.h"

class videoProcess
{
public:

	videoProcess();
	int extractThumbnail(const char* _filename);
};

#endif // VIDEOPROCESS_H
