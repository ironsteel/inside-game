/*
 *
 */

#include <FreeImage.h>
#include <GLES2/gl2.h>

#ifndef TEXTUREUTILS_H
#define TEXTUREUTILS_H

class TextureUtils
{

public:
	static void loadTexture(const char* textName, GLuint *g_textureID);
	
};

#endif // TEXTUREUTILS_H
