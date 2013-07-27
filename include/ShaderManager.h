/*
 *
 */

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <map>
#include "ShaderProgram.h"

class ShaderManager
{

public:

	
	ShaderProgram* createShaderProgram(const std::string& name, const char* vertexShaderPath, const char* fragmentShaderPath);
	
	ShaderProgram* get(std::string& shaderProgramName);
	
	~ShaderManager() 
	{
		mShaderPrograms.clear();
	}
	
public:
	static ShaderManager& getInstance();
	
	
private:
	ShaderManager() {};                   
	ShaderManager(ShaderManager const&);              
	void operator=(ShaderManager const&); 
private:
	std::map<std::string, ShaderProgram*> mShaderPrograms;
	
};

#endif // SHADERMANAGER_H
