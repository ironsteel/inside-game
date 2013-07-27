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
	GLint mvp = glGetUniformLocation(mShaderProgramObject, u_ModelViewProjection.c_str());
	if(mvp == -1) {
		printf("Cannot find %s", u_ModelViewProjection.c_str());
		return false;
	}
	
	uniforms[u_ModelViewProjection] = mvp;
	
	GLint sampler = glGetUniformLocation(mShaderProgramObject, u_Sampler.c_str());
	if(sampler == -1) {
		printf("Cannot find %s", u_Sampler.c_str());
		return false;
	}
	uniforms[u_Sampler] = sampler;
	
	
	
	GLint selected = glGetUniformLocation(mShaderProgramObject, u_Selected.c_str());
	if(selected == -1) {
		printf("Cannot find %s", u_Selected.c_str());
		return false;
	}
	uniforms[u_Selected] = selected;
	
	return true;
	
}

bool ShaderProgram::initAttributeLocations()
{
	
	GLint pos = glGetAttribLocation(mShaderProgramObject, a_Position.c_str());
	if(pos == -1) {
        printf("Cannot find %s", a_Position.c_str());
        return false;
    }
	attributes[a_Position] =  pos;

	GLint textureCoords = glGetAttribLocation(mShaderProgramObject, a_TexCoords.c_str());
	if(textureCoords == -1) {
        printf("Cannot find %s", a_TexCoords.c_str());
        return false;
    }
    attributes[a_TexCoords] = textureCoords;
    return true;
}


GLuint ShaderProgram::getProgramId()
{
    return mShaderProgramObject;
}
