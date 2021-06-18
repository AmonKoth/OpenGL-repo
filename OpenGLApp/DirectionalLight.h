#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{

public:

    DirectionalLight();


    DirectionalLight(GLclampf red, GLclampf green, GLclampf blue,
        GLclampf aIntensity, GLfloat dIntensity,
        GLfloat xDir, GLfloat yDir, GLfloat zDir);

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint DiffuseIntensityLocation, GLfloat directionLocation);


    ~DirectionalLight();


private:

    glm::vec3 direction;

};

