/*
 *
 */

#include "TextureUtils.h"
#include <stdio.h>

void TextureUtils::loadTexture(const char* textName, GLuint *g_textureID)
{
	/////////////////////////////////////////////
	// NEW! - This function has been completely
	// rewritten to use FreeImage.
	
	
	// Get the image file type from FreeImage.
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(textName, 0);
	
	// Actually load the image file.
	FIBITMAP *dib = FreeImage_Load(fifmt, textName,0);
	// Now, there is no guarantee that the image file
	// loaded will be GL_RGB, so we force FreeImage to
	// convert the image to GL_RGB.
	dib = FreeImage_ConvertTo24Bits(dib);
	
	if( dib != NULL )
	{
		glGenTextures(1, g_textureID);
		glBindTexture( GL_TEXTURE_2D, *g_textureID );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
		// This is important to note, FreeImage loads textures in
		// BGR format. Now we could just use the GL_BGR extension
		// But, we will simply swap the B and R components ourselves.
		// Firstly, allocate the new bit data doe the image.
		BYTE *bits = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 3];
		// get a pointer to FreeImage's data.
		BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
		// Iterate through the pixels, copying the data
		// from 'pixels' to 'bits' except in RGB format.
		for(int pix=0; pix<FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pix++)
		{
			bits[pix*3+0]=pixels[pix*3+2];
			bits[pix*3+1]=pixels[pix*3+1];
			bits[pix*3+2]=pixels[pix*3+0];
		}
		// The new 'glTexImage2D' function, the prime difference
		// being that it gets the width, height and pixel information
		// from 'bits', which is the RGB pixel data..
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,
					  GL_RGB, GL_UNSIGNED_BYTE, bits);
		glGenerateMipmap(GL_TEXTURE_2D);
		// Unload the image.
		// and free the bit data.
		FreeImage_Unload(dib);
		delete bits;
	}
}