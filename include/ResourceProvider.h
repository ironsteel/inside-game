/*
 *
 */

#ifndef RESOURCEPROVIDER_H
#define RESOURCEPROVIDER_H

class ResourceProvider
{

public:
	virtual char* getFileBuffer(const char *filePath)  = 0;

};

#endif // RESOURCEPROVIDER_H
