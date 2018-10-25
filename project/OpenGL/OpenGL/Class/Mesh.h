#pragma once
#include <iostream>
#include <vector>
#include "OpenglState.h"

using namespace std;
//��������
struct Vertex{
	glm::vec3 Position;  //����λ��
	glm::vec3 Normal;    //���㷨��
	glm::vec2 TexCoords; //ӳ����������

	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;  //����ID
	string type; //�������� ����������ͼ �߹���ͼ �Է�����ͼ...��
	string path;  // ���Ǵ��������·������������������бȽ�
};

class Mesh
{
public:

	/* �������� */
	vector<Vertex> _vertices;		//��������
	vector<unsigned int> _indices;   //��������
	vector<Texture> _textures;       //��������

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, OpenglState *glSate);
	void Draw();

public:
	OpenglState *_glSate;
	unsigned int _shaderProgram;
	OpenglUtils * _glUtils;

private:
	/*  ��Ⱦ����  */
	unsigned int VAO, VBO, EBO;
	/*  ����  */
	void setupMesh();
};

