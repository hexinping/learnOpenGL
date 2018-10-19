#pragma once
#include <iostream>
#include "OpenglUtils.h"
#include "MathUtils.h"
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
	void useProgram(unsigned int program);

	virtual int getShaderIndex();


	void setBool(unsigned int program, const char *name, bool value) const;
	void setInt(unsigned int program, const char *name, int value) const;
	void setFloat(unsigned int program, const char *name, float value) const;
	void setMat4(unsigned int program, const char *name, glm::mat4 *mat);

	void genTexImage2D(const char *file, int type = GL_RGB, int level = 0, int textureUnit = GL_TEXTURE0, int around = GL_REPEAT, int filter = GL_LINEAR);

public:
	OpenglUtils * _glUtils;
	MathUtils   * _mathUtils;
	bool _isUseEBORender;
	unsigned int _VBO, _VAO,_EBO;
	unsigned int _vertexShader;
	unsigned int _fragmentShader;
	unsigned int _shaderProgram;

	float _vertices[1024];
	unsigned int _indices[1024];

	string _vertFile;
	string _fragFile;

	float  _param1 = 0.0f;

};
