/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "RenderInterfaceOpenGLES.h"
#include "ShaderUtils.h"

#include <Rocket/Core.h>
#include <stdio.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <GLES2/gl2.h>


enum VertexAttributes 
{
	VERTEX_POSITION_INDEX = 0,
	VERTEX_TEX_COODRS_INDEX = 1,
	VERTEX_COLOR_INDEX = 2,
};


const char* vertShader = 
"#version 100 \n\
attribute vec2 position; \
attribute vec2 texcoord; \
attribute vec4 color; \
\
varying vec2 v_texCoord; \
varying vec4 vcolor; \
\
uniform vec2 translation; \
uniform mat4 projection; \
\
void main() { \
	v_texCoord = texcoord;\
	vcolor = color;\
	vec2 tranformedPos =  position + translation;\
	gl_Position = projection * vec4(tranformedPos, 0.0, 1.0); \
} \ 
";

//
const char* fragShader = 
"#version 100 \n\
precision mediump float;\
varying vec2 v_texCoord; \
varying vec4 vcolor; \
\
void main() { \
    gl_FragColor = vcolor;\ 
} \ 
    ";

const char* fragShaderTextured = 
"#version 100 \n\
precision mediump float;\
varying vec2 v_texCoord; \
varying vec4 vcolor; \
uniform sampler2D textureHandle;\
\
void main() { \
    gl_FragColor = texture2D(textureHandle, v_texCoord.st) * vcolor;\ 
} \ 
    ";





RenderInterfaceOpenGLES::RenderInterfaceOpenGLES()
{
    compileShaders();
}

void RenderInterfaceOpenGLES::SetViewport(int width, int height)
{
    mProjection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
    m_width = width;
    m_height = height;
}


// Called by Rocket when it wants to render geometry that it does not wish to optimise.
void RenderInterfaceOpenGLES::RenderGeometry(Rocket::Core::Vertex* vertices, int ROCKET_UNUSED(num_vertices), int* indices, int num_indices, const Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!texture)
    {
        glUseProgram(mColorShaderProgram);
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glUseProgram(mTextureShaderProgram);
        glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, (GLuint) texture);
        glUniform1i(textureLocation, 0);
        glVertexAttribPointer(VERTEX_TEX_COODRS_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), &vertices[0].tex_coord);
		glEnableVertexAttribArray(VERTEX_TEX_COODRS_INDEX);
    }

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(mProjection));
    glUniform2f(transLocation, translation.x, translation.y);

    glVertexAttribPointer(VERTEX_POSITION_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), &vertices[0].position);
    glVertexAttribPointer(VERTEX_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Rocket::Core::Vertex), &vertices[0].colour);


	glEnableVertexAttribArray(VERTEX_POSITION_INDEX);
	glEnableVertexAttribArray(VERTEX_COLOR_INDEX);

    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

    glDisable(GL_BLEND);

}

// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.		
Rocket::Core::CompiledGeometryHandle RenderInterfaceOpenGLES::CompileGeometry(Rocket::Core::Vertex* ROCKET_UNUSED(vertices), int ROCKET_UNUSED(num_vertices), int* ROCKET_UNUSED(indices), int ROCKET_UNUSED(num_indices), const Rocket::Core::TextureHandle ROCKET_UNUSED(texture))
{
	return (Rocket::Core::CompiledGeometryHandle) NULL;
}

// Called by Rocket when it wants to render application-compiled geometry.		
void RenderInterfaceOpenGLES::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry), const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{
}

// Called by Rocket when it wants to release application-compiled geometry.		
void RenderInterfaceOpenGLES::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry))
{
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.		
void RenderInterfaceOpenGLES::EnableScissorRegion(bool enable)
{
	if (enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

// Called by Rocket when it wants to change the scissor region.		
void RenderInterfaceOpenGLES::SetScissorRegion(int x, int y, int width, int height)
{
	glScissor(x, m_height - (y + height), width, height);
}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1) 
struct TGAHeader 
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()

// Called by Rocket when a texture is required by the library.		
bool RenderInterfaceOpenGLES::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
	Rocket::Core::FileHandle file_handle = file_interface->Open(source);
	if (!file_handle)
	{
		return false;
	}
	
	file_interface->Seek(file_handle, 0, SEEK_END);
	size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);
	
	char* buffer = new char[buffer_size];
	file_interface->Read(buffer, buffer_size, file_handle);
	file_interface->Close(file_handle);

	TGAHeader header;
	memcpy(&header, buffer, sizeof(TGAHeader));
	
	int color_mode = header.bitsPerPixel / 8;
	int image_size = header.width * header.height * 4; // We always make 32bit textures 
	
	if (header.dataType != 2)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}
	
	// Ensure we have at least 3 colors
	if (color_mode < 3)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
		return false;
	}
	
	const char* image_src = buffer + sizeof(TGAHeader);
	unsigned char* image_dest = new unsigned char[image_size];
	
	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; y++)
	{
		long read_index = y * header.width * color_mode;
		long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * color_mode;
		for (long x = 0; x < header.width; x++)
		{
			image_dest[write_index] = image_src[read_index+2];
			image_dest[write_index+1] = image_src[read_index+1];
			image_dest[write_index+2] = image_src[read_index];
			if (color_mode == 4)
				image_dest[write_index+3] = image_src[read_index+3];
			else
				image_dest[write_index+3] = 255;
			
			write_index += 4;
			read_index += color_mode;
		}
	}

	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;
	
	bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);
	
	delete [] image_dest;
	delete [] buffer;
	
	return success;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool RenderInterfaceOpenGLES::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	if (texture_id == 0)
	{
		printf("Failed to generate textures\n");
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	texture_handle = (Rocket::Core::TextureHandle) texture_id;

	return true;
}

// Called by Rocket when a loaded texture is no longer required.		
void RenderInterfaceOpenGLES::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	glDeleteTextures(1, (GLuint*) &texture_handle);
}

GLuint RenderInterfaceOpenGLES::compileShaders()
{
    GLuint vert = ShaderUtils::compileShader(vertShader, GL_VERTEX_SHADER); 
	GLuint fragColored = ShaderUtils::compileShader(fragShader, GL_FRAGMENT_SHADER); 
	GLuint fragTextured = ShaderUtils::compileShader(fragShaderTextured, GL_FRAGMENT_SHADER); 
    
	
	mColorShaderProgram = ShaderUtils::createProgramAndAttachShaders(vert, fragColored);
    bindVertexAttributes(mColorShaderProgram);
	ShaderUtils::linkProgram(mColorShaderProgram);
	
    
	mTextureShaderProgram = ShaderUtils::createProgramAndAttachShaders(vert, fragTextured);
    bindVertexAttributes(mTextureShaderProgram);
	ShaderUtils::linkProgram(mTextureShaderProgram);
	
    transLocation = 0;
    projectionLocation = 1;
    textureLocation = 2;
    
}
void RenderInterfaceOpenGLES::bindVertexAttributes(GLuint program) 
{
    glBindAttribLocation(program, VERTEX_POSITION_INDEX, "position");
    glBindAttribLocation(program, VERTEX_TEX_COODRS_INDEX, "texcoord");
    glBindAttribLocation(program, VERTEX_COLOR_INDEX, "color");
}
