#pragma once
#include <iostream>
#include <vector>
#include "OpenglState.h"

using namespace std;
//顶点数据
struct Vertex{
	glm::vec3 Position;  //顶点位置
	glm::vec3 Normal;    //顶点法线
	glm::vec2 TexCoords; //映射纹理坐标

	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;  //纹理ID
	string type; //纹理类型 （漫反射贴图 高光贴图 自发光贴图...）
	string path;  // 我们储存纹理的路径用于与其它纹理进行比较
};

class Mesh
{
public:

	/* 网格数据 */
	vector<Vertex> _vertices;		//顶点数据
	vector<unsigned int> _indices;   //索引数据
	vector<Texture> _textures;       //纹理数据

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, OpenglState *glSate);
	void Draw();

public:
	OpenglState *_glSate;
	unsigned int _shaderProgram;
	OpenglUtils * _glUtils;

private:
	/*  渲染数据  */
	unsigned int VAO, VBO, EBO;
	/*  函数  */
	void setupMesh();
};

