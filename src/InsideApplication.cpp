#include "InsideApplication.h"
#include "ShaderUtills.h"
#include "ShaderProgram.h"
#include "Cube.h"
#include "TextureUtils.h"

InsideApplication::InsideApplication()
{
	cube = new Cube();
}

InsideApplication::~InsideApplication()
{
	delete cube;
	delete mShaderProgram;
}

void InsideApplication::init()
{
	GLuint vertexShader = compileShader("../resources/shaders/simple.vsh", GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader("../resources/shaders/simple.fsh", GL_FRAGMENT_SHADER);
	mShaderProgram = new ShaderProgram(vertexShader, fragmentShader);
	if(!mShaderProgram->link()) {
		printf("Cannot link program");
	}
	TextureUtils::loadTexture("../resources/textures/wood.jpg", &texture);
	
	model = glm::mat4(1.0f);
	cube->initGeometry();
 	glEnable(GL_DEPTH_TEST);
}

void InsideApplication::drawOneFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(mShaderProgram->getProgramId());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glm::mat4 mvp = projection * view * model;
	
	GLint mvpId = mShaderProgram->getUniformLocation(u_ModelViewProjection);
	glUniformMatrix4fv(mvpId, 1, GL_FALSE, glm::value_ptr(mvp));
	
	GLint textureLocation = mShaderProgram->getUniformLocation(u_Sampler);
	glUniform1i(textureLocation, 0);
	
	cube->draw(mShaderProgram);
}

void InsideApplication::update(double timeSinceLastFrame)
{
	float angle =  timeSinceLastFrame / 1000.0 * 75.0;
	model = glm::rotate(model,angle, glm::vec3(0, 1, 0));
}

void InsideApplication::reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	projection = glm::perspective(45.0, (double) width / height, 0.1, 100.0);
    view = glm::lookAt(glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void InsideApplication::onKeyPressed()
{
	
}

void InsideApplication::onPointerDown(int x, int y)
{
	
}

void InsideApplication::onPointerMoved(double x, double y)
{
	
}



