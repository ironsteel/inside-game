/* Inside game

   Copyright (C) 2013 Rangel Ivanov

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
 *
 */

#include "TextureUtils.h"
#include <stdio.h>
#include <png.h>
#include <string.h>
#include <stdlib.h>

bool TextureUtils::loadPngImage(const char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, interlace_type;
	FILE *fp;
	
	if ((fp = fopen(name, "rb")) == NULL)
		return false;
	
	/* Create and initialize the png_struct
	 * with the desired error handler
	 * functions.  If you want to use the
	 * default stderr and longjump method,
	 * you can supply NULL for the last
	 * three parameters.  We also supply the
	 * the compiler header file version, so
	 * that we know if the application
	 * was compiled with a compatible version
	 * of the library.  REQUIRED
	 */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
									 NULL, NULL, NULL);
	
	if (png_ptr == NULL) {
		fclose(fp);
		return false;
	}
	
	/* Allocate/initialize the memory
	 * for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}
	
	/* Set error handling if you are
	 * using the setjmp/longjmp method
	 * (this is the normal method of
	 * doing things with libpng).
	 * REQUIRED unless you  set up
	 * your own error handlers in
	 * the png_create_read_struct()
	 * earlier.
	 */
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* Free all of the memory associated
		 * with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		/* If we get here, we had a
		 * problem reading the file */
		return false;
	}
	
	/* Set up the output control if
	 * you are using standard C streams */
	png_init_io(png_ptr, fp);
	
	
	/* If we have already
	 * read some of the signature */
	png_set_sig_bytes(png_ptr, sig_read);
	
	/*
	 * If you have enough memory to read
	 * in the entire image at once, and
	 * you need to specify only
	 * transforms that can be controlled
	 * with one of the PNG_TRANSFORM_*
	 * bits (this presently excludes
	 * dithering, filling, setting
	 * background, and doing gamma
	 * adjustment), then you can read the
	 * entire image (including pixels)
	 * into the info structure with this
	 * call
	 *
	 * PNG_TRANSFORM_STRIP_16 |
	 * PNG_TRANSFORM_PACKING  forces 8 bit
	 * PNG_TRANSFORM_EXPAND forces to
	 *  expand a palette into RGB
	 */
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
	
	png_uint_32 width, height;
	int bit_depth;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
				 &interlace_type, NULL, NULL);
	outWidth = width;
	outHeight = height;
	
	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	*outData = (unsigned char*) malloc(row_bytes * outHeight);
	
	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	
	for (int i = 0; i < outHeight; i++) {
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
	}
	
	outHasAlpha = (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGB);
	
	
	/* Clean up after the read,
	 * and free any memory allocated */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	
	/* Close the file */
	fclose(fp);
	
	/* That's it */
	return true;
}

void TextureUtils::loadTexture(const char* textName, GLuint *g_textureID)
{
	GLubyte *imageData;
	
	int width;
	int height;
	
	bool hasAlpha;
	
	TextureUtils::loadPngImage(textName, width, height, hasAlpha, &imageData);
	
	glGenTextures(1, g_textureID);
	glBindTexture( GL_TEXTURE_2D, *g_textureID );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, width, height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	free(imageData);

}