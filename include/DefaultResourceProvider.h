/*
 *
 */

#ifndef DEFAULTRESOURCEPROVIDER_H
#define DEFAULTRESOURCEPROVIDER_H
#include "ResourceProvider.h"

class DefaultResourceProvider : public ResourceProvider 
{
	virtual char* getFileBuffer(const char *filePath);
};

#endif // DEFAULTRESOURCEPROVIDER_H
