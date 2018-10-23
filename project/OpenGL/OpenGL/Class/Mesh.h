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
	vector<Vertex> vertices;		//��������
	vector<unsigned int> indices;   //��������
	vector<Texture> textures;       //��������

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

