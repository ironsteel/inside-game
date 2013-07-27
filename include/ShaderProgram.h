#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <GLES2/gl2.h>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "ShaderConstants.h"


using namespace std;

class ShaderProgram 
{

public:
    ShaderProgram(GLuint vertexShader, GLuint fragmentShader);
    virtual ~ShaderProgram() { }
		
		virtual bool link();
		GLint getAttributeLocation(const string& attributeName);
		GLint getUniformLocation(const string& uniformName);
		virtual GLuint getProgramId();
    
protected:
    virtual bool initUniformLocations();
    virtual bool initAttributeLocations();
    
    

private:
    GLuint mVertexShader;
    GLuint mFragmentShader;    
    
    GLuint mShaderProgramObject;
    map<string, GLuint> attributes;
    map<string, GLint> uniforms;
    
};

#endif // SHADERPROGRAM_H
