#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"
#include "PointLight.h"

using namespace std;

const float toRadiants = 3.14159265f / 180.0f;

Window mainWindow;
vector<Mesh*> meshList;
vector<Shader*> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];

Material shinyMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;



//Vertex Shader
static const char* vShader ="Shaders/shader.vert";

//Fragment Shader
static const char* fshader ="Shaders/shader.frag";


void calcAvarageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vertexLength,
		unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vertexLength;
		unsigned int in1 = indices[i + 1] * vertexLength;
		unsigned int in2 = indices[i + 2] * vertexLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vertexLength; i++)
	{
		unsigned int nOffset = i * vertexLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.x; vertices[nOffset + 2] = vec.x;
	}
}



void CreateObjects()
{
	unsigned int indices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	GLfloat vertices[] = {
		//    x      y      z      u        v		nx   ny   nz
			-1.0f, -1.0f, -0.6f,   0.0f,    0.0f,	0.0f,0.0f,0.0f,
			0.0f,-1.0f,1.0f,      0.5f,    0.0f,	0.0f,0.0f,0.0f,
			1.0f, -1.0f, -0.6f,    1.0f,    0.0f,	0.0f,0.0f,0.0f,
			0.0f, 1.0f, 0.0f,     0.5f,    1.0f,	0.0f,0.0f,0.0f
	};

	unsigned int floorIndices[] = {
		0,2,1,
		1,2,3

	};


	GLfloat floorVertices[] = {

		-10.0f, 0.0f,-10.0f,	 0.0f, 0.0f,		0.0f,-1.0f,0.0f,
		10.0f,0.0f,-10.0f,		10.0f,0.0f,			0.0f, -1.0f,0.0f,
		-10.0f,0.0f,10.0f,		0.0f, 10.0f,		0.0f,-1.0f,0.0f,
		10.0f,0.0f,10.0f,		10.0f,10.0f,		0.0f,-1.0f,0.0f

	};

	calcAvarageNormals(indices, 12, vertices, 32, 8, 5);
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fshader);
	shaderList.push_back(shader1);
}

int main()
{
	mainWindow = Window(1280, 1024);
	mainWindow.initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture((char*)("Textures/brick.png"));
	brickTexture.LoadTexture();

	dirtTexture = Texture((char*)("Textures/dirt.png"));
	dirtTexture.LoadTexture();

	shinyMaterial = Material(1.0f, 64.0f);
	dullMaterial = Material(0.3f, 4.0f);

	mainLight = DirectionalLight(1.0f,1.0f,1.0f,
		0.1f, 0.5f,
		2.0f,1.0f,-1.0f);
	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
								0.5f, 0.5f,
								-4.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
								0.5f, 0.4,
								4.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformCamPosition = 0,
		 uniformSpecularInt = 0, uniformShininess = 0;;
	
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeigth(), 0.1f, 100.0f);

	//GameLoop

	while (!mainWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime();//to make speed framerate dependant
		deltaTime = now - lastTime;
		lastTime = now;

		//Get and handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(),deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		//clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformCamPosition = shaderList[0]->GetCameraPos();


		uniformSpecularInt = shaderList[0]->GetSpecularIntLoc();
		uniformShininess = shaderList[0]->GetShineLoc();

		shaderList[0]->SetDirectionalLight(&mainLight);
		shaderList[0]->SetPointLights(pointLights, pointLightCount);

		//mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor,uniformDiffuseIntensity,uniformDirection);
		
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformCamPosition, camera.getCameraPostion().x, camera.getCameraPostion().y, camera.getCameraPostion().z);

		glm::mat4 model = glm::mat4(1.0f);
		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadiants, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularInt,uniformShininess);
		meshList[0]->RenderMesh();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularInt, uniformShininess);
		meshList[1]->RenderMesh();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularInt, uniformShininess);
		meshList[2]->RenderMesh();


		glUseProgram(0);
		mainWindow.SwapBuffers();
	}

	return 0;
}