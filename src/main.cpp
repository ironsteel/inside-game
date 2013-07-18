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

/* OpenGL draw function & timing */
static void draw(void)
{
	app.drawOneFrame();
}


/* update animation parameters */
static void animate(double timeSinceLastFrame)
{
	app.update(timeSinceLastFrame);
}


/* change view angle, exit upon ESC */
void key( GLFWwindow* window, int k, int action, int a, int b )
{
	if( action != GLFW_PRESS ) return;
	
    
	switch (k) {
		case GLFW_KEY_Z:
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_UP:
			break;
		case GLFW_KEY_DOWN:
			break;
		case GLFW_KEY_LEFT:
			break;
		case GLFW_KEY_RIGHT:
			break;
		default:
			return;
	}
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


static void mouse_down(GLFWwindow *window, int x, int y, int i)
{
	double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	
	app.onPointerDown(x, y, cursorX, cursorY);
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
