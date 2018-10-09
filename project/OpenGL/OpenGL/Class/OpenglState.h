#pragma once
#include <iostream>
#include "OpenglUtils.h"
using namespace std;
class OpenglState
{
public:

	OpenglState();
	virtual bool init(string vertFile, string fragFile);
	void initRendCommand();
	virtual bool isUseEBORender();
	virtual void rendeCommand();
	virtual void enableVertexAttribArray();

public:
	OpenglUtils * _glUtils;
	bool _isUseEBORender;
	unsigned int _VBO, _VAO,_EBO;
	unsigned int _vertexShader;
	unsigned int _fragmentShader;
	unsigned int _shaderProgram;

	float _vertices[12];
	unsigned int _indices[6];

	string _vertFile;
	string _fragFile;

};
