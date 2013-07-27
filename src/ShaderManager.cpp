/*
 *
 */

#include "ShaderManager.h"
#include "ShaderUtills.h"
#include <stdexcept>

ShaderProgram* ShaderManager::createShaderProgram(const std::string& name, const char* vertexShaderPath, const char* fragmentShaderPath)
{
	GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
	if(!vertexShader || !fragmentShader) {
		throw std::runtime_error("Cannot compile shaders");
	}
	
	ShaderProgram* shaderProgram = new ShaderProgram(vertexShader, fragmentShader);
	if(!shaderProgram->link()) {
		throw new std::runtime_error("Cannot link shader program");
	}
	
	mShaderPrograms[name] = shaderProgram;
	return shaderProgram;
}


ShaderProgram* ShaderManager::get(std::string& shaderProgramName)
{
	return mShaderPrograms[shaderProgramName];
}


ShaderManager& ShaderManager::getInstance()
{
	static ShaderManager shaderManager;
	return shaderManager;
}