#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>

#include<GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include"CommonValues.h"

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();

	
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	string ReadFile(const char* fileLocation);
	//general getters
	GLuint GetProjectionLocation() {return uniformProjection;}
	GLuint GetModelLocation() {return uniformModel;}
	GLuint GetViewLocation() { return uniformView; }
	GLuint GetCameraPos() { return uniformCamPosition; }
	//lighting getters
	GLuint GetAmbIntensityLoc() { return uniformDirectionalLight.uniformAmbientIntensity; }
	GLuint GetAmbColorLoc() { return uniformDirectionalLight.uniformColor; }
	GLuint GetDifIntensityLoc() { return uniformDirectionalLight.uniformDiffuseIntensity; }
	GLuint GetDirectionLoc() { return uniformDirectionalLight.uniformDirection; }
	GLuint GetSpecularIntLoc() { return uniformSpecularInt; }
	GLuint GetShineLoc() { return uniformShine; }

	void SetDirectionalLight(DirectionalLight *dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);

	void UseShader();
	void ClearShader();


private:

	//to avoid crashing while trying to create pointlights
	int pointLightCount;

		//general Values
	GLuint shaderID, uniformProjection, uniformModel,uniformView, uniformCamPosition,
		//lighting values
		uniformSpecularInt,uniformShine,uniformPointLightCount;

	struct {
		GLuint uniformColor, 
			uniformAmbientIntensity,
			uniformDiffuseIntensity,
			uniformDirection;
		
	} uniformDirectionalLight;


	struct {
		GLuint uniformColor,
			uniformAmbientIntensity,
			uniformDiffuseIntensity,
			uniformPosition,
			uniformConstant,
			uniformLinear,
			uniformExponent;

	} uniformPointLight[MAX_POINT_LIGHTS];



	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

};

