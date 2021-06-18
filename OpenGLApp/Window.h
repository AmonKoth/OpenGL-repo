#pragma once
#include <stdio.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

class Window
{

public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	
	int initialise();

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetBufferHeigth() { return bufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	//To handle the keys but don't to it like this there is a better way which includes creating a command class 
	bool* getKeys() { return keys; }

	GLfloat getXChange();
	GLfloat getYChange();

	~Window();

private:

	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];


	//Mouse Movement
	GLfloat lastX, lastY, xChange, yChange;
	//To stop mouse from its initial movement;
	bool mouseFirstMoved;

	//GLFW needs to make callbacks normal member functions don't work in this case
	//Values has to be like this
	void CreateCallBacks();
	static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	static void handleMovement(GLFWwindow* window, double xPos, double yPos);
};

