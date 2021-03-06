#include "DirectionalLight.h"
DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);

}

DirectionalLight::DirectionalLight(GLclampf red, GLclampf green, GLclampf blue,
	GLclampf aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) :Light(red, green, blue, aIntensity, dIntensity)

{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	GLuint DiffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(DiffuseIntensityLocation, diffuseIntensity);
}


DirectionalLight::~DirectionalLight()
{

}