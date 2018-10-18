#include "OpenglState.h"

OpenglState::OpenglState()
: _glUtils(nullptr)
, _isUseEBORender(true)
, _VBO(0)
, _VAO(0)
, _EBO(0)
, _vertexShader(0)
, _fragmentShader(0)
, _shaderProgram(0)
{
	_glUtils = OpenglUtils::getInstance();

}

bool OpenglState::init(string vertFile, string fragFile)
{
	this->initRendCommand();
	return true;
}

void OpenglState::initRendCommand()
{
	_isUseEBORender = this->isUseEBORender();
	_glUtils->bindVBOAndVAO(&_VBO, &_VAO, _vertices, sizeof(_vertices), _isUseEBORender, &_EBO, _indices, sizeof(_indices));
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, _vertFile);
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, _fragFile);
	_glUtils->linkShader(&_shaderProgram, _vertexShader, _fragmentShader);
	this->enableVertexAttribArray();
}

bool OpenglState::isUseEBORender()
{
	return true;
}

void OpenglState::enableVertexAttribArray()
{
	GLint posLocation = glGetAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
}

void OpenglState::rendeCommand()
{
	glUseProgram(_shaderProgram); //激活着色器程序对象：已激活着色器程序的着色器将在我们发送渲染调用的时候被使用
	glBindVertexArray(_VAO);     // 使用VAO后就是每一次渲染的时候直接使用VAO存储好的属性指针
}