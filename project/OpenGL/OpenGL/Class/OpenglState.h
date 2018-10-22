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
	virtual void lightRendeCommand();
	virtual void enableVertexAttribArray();
	virtual void enableLightVertexAttribArray();
	GLint getUniformLocation(unsigned int program, const char *name);
	GLint getAttribLocation(unsigned int program, const char *name);
	void useProgram(unsigned int program);

	virtual int getShaderIndex();


	void setBool(unsigned int program, const char *name, bool value) const;
	void setInt(unsigned int program, const char *name, int value) const;
	void setFloat(unsigned int program, const char *name, float value) const;
	/*void setMat4(unsigned int program, const char *name, glm::mat4 *mat);
	void setVec3(unsigned int program, const char *name, float x, float y, float z);
	void setVec4(unsigned int program, const char *name, float x, float y, float z, float w);*/
	

	void setMat2(unsigned int program, const char *name, glm::mat2 &mat);
	void setMat3(unsigned int program, const char *name, glm::mat3 &mat);
	void setMat4(unsigned int program, const char *name, glm::mat4 *mat);

	void setVec2(unsigned int program, const char *name, float x, float y);
	void setVec3(unsigned int program, const char *name, float x, float y, float z);
	void setVec4(unsigned int program, const char *name, float x, float y, float z, float w);

	void setVec2(unsigned int program, const char *name, glm::vec2 &value);
	void setVec3(unsigned int program, const char *name, glm::vec3 &value);
	void setVec4(unsigned int program, const char *name, glm::vec4 &value);

	void genTexImage2D(const char *file, int type = GL_RGB, int level = 0, int textureUnit = GL_TEXTURE0, int around = GL_REPEAT, int filter = GL_LINEAR);

	void setLight(bool light);
public:
	OpenglUtils * _glUtils;
	MathUtils   * _mathUtils;
	bool _isUseEBORender;
	unsigned int _VBO, _VAO,_EBO;
	unsigned int _vertexShader;
	unsigned int _fragmentShader;
	unsigned int _shaderProgram;
	unsigned int _lightShaderProgram;

	//单独为光源配置一个vao 与模型的vao分开
	bool _isLight;
	unsigned int _lightVBO, _lightVAO, _lightEBO;


	float _vertices[1024];
	unsigned int _indices[1024];

	float _lightVertices[1024];
	unsigned int _lightIndices[1024];

	string _vertFile;
	string _fragFile;

	float  _param1 = 0.0f;
	glm::vec3 _param2 = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 _param3 = glm::vec3(0.0f, 0.0f, 0.0f);
	float _param4 = 0.0f;

};
