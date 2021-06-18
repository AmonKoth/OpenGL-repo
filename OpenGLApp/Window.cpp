#include "Window.h"

Window::Window()
{
    width = 800;
    height = 600;

	xChange = 0.0f;
	yChange = 0.0f;
	for (int i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;

	xChange = 0.0f;
	yChange = 0.0f;
	//Initialize keys as not pressed
	for (int i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::initialise()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}


	//Set up GLFW window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core profile = no backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "TEST WINDOW", NULL, NULL);

	if (!mainWindow)
	{
		printf("Window Failed to be CREATED");
		glfwTerminate();
		return 1;
	}


	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Handle key and mouse Input
	CreateCallBacks();

	//To disable the mouse
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Allow Modern Extensions features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Glew initialisation failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
}
void Window::CreateCallBacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMovement);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}
void Window::handleMovement(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY-yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
