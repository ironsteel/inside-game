/*
 *
 */

#include "ResourceManager.h"


void ResourceManager::setBaseDirectory(const char *path)
{
	mBaseDir = std::string(path);
}

void ResourceManager::setResourceProvider(ResourceProvider *provider)
{
	mResourceProvider = provider;
}

char * ResourceManager::getFileBuffer(const char *path)
{
	std::string fullPath = mBaseDir + std::string(path);
	return mResourceProvider->getFileBuffer(fullPath.c_str());
}