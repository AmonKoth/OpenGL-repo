#pragma once
#include "Light.h"
class PointLight :
    public Light
{

public:
    PointLight();
    PointLight(GLclampf red, GLclampf green, GLclampf blue, GLclampf aIntensity, GLfloat dIntensity,
        GLfloat xPost, GLfloat yPos, GLfloat zPos,
        GLfloat con,GLfloat lin ,GLfloat exp);


    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint DiffuseIntensityLocation, GLfloat positionLocation,
        GLfloat constantLocation,GLfloat linearLocation,GLfloat exponentLocation );


    ~PointLight();


private:

    glm::vec3 position;

    //controls the attuanetion of the light source these will be set according to testing
    //our calculation  will be as L/ax^2+bx+c so a is exponent b is linear c is constant so be carefull when calculating this
    GLfloat constant, linear, exponent;



};

