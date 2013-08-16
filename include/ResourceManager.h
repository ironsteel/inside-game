/*
 *
 */

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "Singleton.h"
#include "ResourceProvider.h"
#include <string>

class ResourceManager : public Singleton<ResourceManager>
{
public:
	~ResourceManager() { delete mResourceProvider; } 
	void setBaseDirectory(const char *path);
	void setResourceProvider(ResourceProvider *provider);
	char *getFileBuffer(const char *path);

private:
	std::string mBaseDir;
	ResourceProvider *mResourceProvider;
	
	
};

#endif // RESOURCEMANAGER_H
