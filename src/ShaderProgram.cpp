#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
    this->vertexShader = vertexShader;
    this->fragmentShader = fragmentShader;
    this->shaderProgramObject = glCreateProgram();
}

bool ShaderProgram::link()
{
    GLint linked;
    
    glAttachShader(shaderProgramObject, vertexShader);
    glAttachShader(shaderProgramObject, fragmentShader);
    
    glBindAttribLocation(shaderProgramObject, 0, a_Position.c_str());
    glBindAttribLocation(shaderProgramObject, 1, a_TexCoords.c_str());
    
    glLinkProgram(shaderProgramObject);

    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLen);
        char *infoLog = (char*) malloc(sizeof(char) * infoLen);
        glGetProgramInfoLog(shaderProgramObject, infoLen, NULL, infoLog);
        printf("Error linking shader program\n%s\n", infoLog);
        glDeleteProgram(shaderProgramObject);
        free(infoLog);
        return false; 
    }
	
    
    
    
    if(!initUniformLocations()) {
        return false;
    }    
    
    if(!initAttributeLocations()) {
        return false;
    }
            
    
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
	GLint mvp = glGetUniformLocation(shaderProgramObject, u_ModelViewProjection.c_str());
	if(mvp == -1) {
		printf("Cannot find %s", u_ModelViewProjection.c_str());
		return false;
	}
	
	uniforms[u_ModelViewProjection] = mvp;
	
	GLint sampler = glGetUniformLocation(shaderProgramObject, u_Sampler.c_str());
	if(sampler == -1) {
		printf("Cannot find %s", u_Sampler.c_str());
		return false;
	}
	uniforms[u_Sampler] = sampler;
	
	return true;
	
}

bool ShaderProgram::initAttributeLocations()
{
	
	GLint pos = glGetAttribLocation(shaderProgramObject, a_Position.c_str());
	if(pos == -1) {
        printf("Cannot find %s", a_Position.c_str());
        return false;
    }
	attributes[a_Position] =  pos;

	GLint textureCoords = glGetAttribLocation(shaderProgramObject, a_TexCoords.c_str());
	if(textureCoords == -1) {
        printf("Cannot find %s", a_TexCoords.c_str());
        return false;
    }
    attributes[a_TexCoords] = textureCoords;
    return true;
}


GLuint ShaderProgram::getProgramId()
{
    return shaderProgramObject;
}
