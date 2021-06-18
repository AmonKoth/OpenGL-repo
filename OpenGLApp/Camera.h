#pragma once

#include <GL/glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<GLFW/glfw3.h>


class Camera
{

public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys,GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 calculateViewMatrix();
	//to pass this value to the shader for reflection ops
	glm::vec3 getCameraPostion()
	{return position;}

	~Camera();


private:

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw, pitch;

	GLfloat moveSpeed, turnSpeed ,deltaTime;

	void update();


};

