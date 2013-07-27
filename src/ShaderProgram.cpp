#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(GLuint vertexShader, GLuint fragmentShader)
:mVertexShader(vertexShader), mFragmentShader(fragmentShader), mShaderProgramObject(0)
{
}
bool ShaderProgram::link()
{
	bool linkStatus = true;
	mShaderProgramObject = glCreateProgram();
    GLint linked;
    
    glAttachShader(mShaderProgramObject, mVertexShader);
    glAttachShader(mShaderProgramObject, mFragmentShader);
    
    glBindAttribLocation(mShaderProgramObject, 0, a_Position.c_str());
    glBindAttribLocation(mShaderProgramObject, 1, a_TexCoords.c_str());
	glBindAttribLocation(mShaderProgramObject, 2, a_Normal.c_str());
    
    glLinkProgram(mShaderProgramObject);

    glGetProgramiv(mShaderProgramObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(mShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLen);
        char *infoLog = (char*) malloc(sizeof(char) * infoLen);
        glGetProgramInfoLog(mShaderProgramObject, infoLen, NULL, infoLog);
        printf("Error linking shader program\n%s\n", infoLog);
        glDeleteProgram(mShaderProgramObject);
        free(infoLog);
        linkStatus = false;
    }
	
    
    
    
    if(!initUniformLocations())
		linkStatus = false;
    
    
    if(!initAttributeLocations())
		linkStatus = false;
            
    if(!linkStatus)
		throw std::exception();
	
    return true;
}

GLint ShaderProgram::getAttributeLocation(const string& attributeName)
{
    return attributes[attributeName];
}

GLint ShaderProgram::getUniformLocation(const string& uniformName)
{
    return uniforms[uniformName];
}

bool ShaderProgram::initUniformLocations()
{
	
	initUniform(u_ModelViewProjection);
	initUniform(u_lightPosition);
	initUniform(u_NormalMatrix);
	initUniform(u_Sampler);
	initUniform(u_modelMatrix);
	
	return true;
	
}
bool ShaderProgram::initUniform(std::string name)
{
	GLint uniformLocation = glGetUniformLocation(mShaderProgramObject, name.c_str());
	if(uniformLocation == -1) {
		printf("Cannot find %s", name.c_str());
		return false;
	}
	uniforms[name] = uniformLocation;
	return true;
}

bool ShaderProgram::initAttributeLocations()
{
	
	initAttribute(a_Position);
	initAttribute(a_TexCoords);
	initAttribute(a_Normal);
	
    return true;
}

bool ShaderProgram::initAttribute(std::string name)
{
	GLint pos = glGetAttribLocation(mShaderProgramObject, name.c_str());
	if(pos == -1) {
		printf("Cannot find %s", name.c_str());
		return false;
	}
	attributes[name] =  pos;
	return true;
}


GLuint ShaderProgram::getProgramId()
{
    return mShaderProgramObject;
}
