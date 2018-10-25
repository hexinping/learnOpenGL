#include "Mesh.h"
#include "OpenglUtils.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, OpenglState *glSate)
:_glSate(nullptr)
, _glUtils(nullptr)
, _shaderProgram(0)
{
	_vertices = vertices;
	_indices = indices;
	_textures = textures;
	_glSate = glSate;
	_shaderProgram = glSate->_shaderProgram;
	_glUtils = glSate->_glUtils;

	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int),
		&_indices[0], GL_STATIC_DRAW);

	
	// ����λ��
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// ���㷨��
	GLint normalLocation = _glUtils->getAttribLocation(_shaderProgram, "aNormal");
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// ������������
	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoords");
	glEnableVertexAttribArray(aTexCoordLocation);
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	// vertex tangent
	GLint aTangentLocation = _glUtils->getAttribLocation(_shaderProgram, "aTangent");
	glEnableVertexAttribArray(aTangentLocation);
	glVertexAttribPointer(aTangentLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	// vertex bitangent
	GLint aBitangentLocation = _glUtils->getAttribLocation(_shaderProgram, "aBitangent");
	glEnableVertexAttribArray(aBitangentLocation);
	glVertexAttribPointer(aBitangentLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));


	glBindVertexArray(0);
}

void Mesh::Draw()
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
		// ��ȡ������ţ�diffuse_textureN �е� N��
		string number;
		string name = _textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		_glSate->setInt(_shaderProgram, ("material."+ name).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, _textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// ��������
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
