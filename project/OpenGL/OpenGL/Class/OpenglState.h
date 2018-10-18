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
	GLint getUniformLocation(unsigned int program, const char *name);
	GLint getAttribLocation(unsigned int program, const char *name);

	virtual int getShaderIndex();

	void setBool(unsigned int program, const char *name, bool value) const;
	void setInt(unsigned int program, const char *name, int value) const;
	void setFloat(unsigned int program, const char *name, float value) const;

public:
	OpenglUtils * _glUtils;
	bool _isUseEBORender;
	unsigned int _VBO, _VAO,_EBO;
	unsigned int _vertexShader;
	unsigned int _fragmentShader;
	unsigned int _shaderProgram;

	float _vertices[1024];
	unsigned int _indices[1024];

	string _vertFile;
	string _fragFile;

};