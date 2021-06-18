#include "Mesh.h"


Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}


void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*numOfVertices, vertices, GL_STATIC_DRAW);
	//update the multiplication values depending the size(how much do we want to jump)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices)*8, 0);
	glEnableVertexAttribArray(0);
	//Can enable this whenever we use a mesh since i will have one type of mesh i'll just enable it and keep it enabled
	//Don't use this if more than one mesh
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	//for normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;

	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;

	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;

	}
	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}