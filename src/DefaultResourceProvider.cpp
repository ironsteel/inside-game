/*
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>

#include "DefaultResourceProvider.h"

char* DefaultResourceProvider::getFileBuffer(const char *filePath)
{
	FILE* file = fopen(filePath, "rb");
	if (file == 0)
		fprintf(stderr, "Cannot open file %s", filePath);
	
	fseek(file, 0, SEEK_END);
	const size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	
	char* buffer = (char*) malloc(size);
	
	const size_t size_read = fread((void *)buffer, sizeof(char), size, file);
	fclose(file);
	
	return buffer;
}
