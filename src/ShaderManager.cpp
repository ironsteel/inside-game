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

#include "ShaderManager.h"
#include "ShaderUtils.h"
#include <stdexcept>

ShaderProgram* ShaderManager::createShaderProgram(const std::string& name, const char* vertexShaderPath, const char* fragmentShaderPath)
{
	GLuint vertexShader = ShaderUtils::compileShaderFromFile(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = ShaderUtils::compileShaderFromFile(fragmentShaderPath, GL_FRAGMENT_SHADER);
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


