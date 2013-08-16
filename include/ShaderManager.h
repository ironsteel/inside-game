/*
 *
 */

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <map>
#include "ShaderProgram.h"
#include "Singleton.h"

class ShaderManager : public Singleton<ShaderManager>
{

public:
	ShaderProgram* createShaderProgram(const std::string& name, const char* vertexShaderPath, const char* fragmentShaderPath);
	
	ShaderProgram* get(std::string& shaderProgramName);
private:
	std::map<std::string, ShaderProgram*> mShaderPrograms;
	
};

#endif // SHADERMANAGER_H
