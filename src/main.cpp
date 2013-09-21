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
#define GLFW_INCLUDE_ES2

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <sys/time.h>

#include "InsideApplication.h"


InsideApplication app;


static double getCurrentTime()
{
	static struct timeval start;
	
	gettimeofday(&start, NULL);
	return ((start.tv_sec * 1000) + (start.tv_usec / 1000.0));
}

static void draw(void)
{
	app.drawOneFrame();
}


static void animate(double timeSinceLastFrame)
{
	app.update(timeSinceLastFrame);
}


void key( GLFWwindow* window, int k, int action, int a, int b )
{
	app.onKeyPressed(k);
}


/* new window size */
void reshape( GLFWwindow* window, int width, int height )
{
	app.reshape(width, height);
}


/* program & OpenGL initialization */
static void init(int argc, char *argv[])
{
	app.init();
}


static void mouse_down(GLFWwindow *window, int button, int pressed, int flags)
{
	double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	
	MouseButton btn;
	if(button == GLFW_MOUSE_BUTTON_LEFT)
		btn = LEFT;
	else if(button == GLFW_MOUSE_BUTTON_RIGHT)
		btn = RIGHT;
	
	if(pressed == GLFW_PRESS) {
		app.onPointerDown(btn, cursorX, cursorY);
	} else if(pressed == GLFW_RELEASE) {
		app.onPointerUp(btn, cursorX, cursorY);
	}
	
}

static void cursor_moved(GLFWwindow *window, double x, double y) 
{
	app.onPointerMoved(x, y);
}

void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}


/* program entry */
int main(int argc, char *argv[])
{
	GLFWwindow* window;
	int width, height;
	
	glfwSetErrorCallback(error_callback);
	
    
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit( EXIT_FAILURE );
	}
	
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	
	window = glfwCreateWindow( 800, 600, "Inside", NULL, NULL );
	if (!window)
	{
		fprintf( stderr, "Failed to open GLFW window\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	
	// Set callback functions
	glfwSetWindowSizeCallback(window, reshape);
	glfwSetKeyCallback(window, key);
	glfwSetMouseButtonCallback(window, mouse_down);
	glfwSetCursorPosCallback(window, cursor_moved);
	
	
	glfwMakeContextCurrent(window);
	
	glfwGetWindowSize(window, &width, &height);
	reshape(window, width, height);
	// Parse command-line options
	init(argc, argv);
	
	double startTime = glfwGetTime();
	double timeSinceLastFrame = 0;
	
	// Main loop
	while(!glfwWindowShouldClose(window) )
	{
		
		startTime = getCurrentTime();
		// Draw gears
		draw();
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		// Update animation
		animate(timeSinceLastFrame);
		timeSinceLastFrame = getCurrentTime() - startTime;
	}
	
	// Terminate GLFW
	glfwTerminate();
	
	// Exit program
	exit( EXIT_SUCCESS );
}
