#pragma once
#include<GL/glew.h>
#include <glm/glm.hpp>
#include <cmath>



class Light
{

public:

	Light();
	Light(GLclampf red, GLclampf green, GLclampf blue, GLclampf aIntensity, GLfloat dIntensity);

	~Light();



protected:

	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;


};

