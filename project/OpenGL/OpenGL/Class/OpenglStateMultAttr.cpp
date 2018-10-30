#include "OpenglStateMultAttr.h"

bool OpenglStateMultAttr::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
	};

	int len = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len);
	_vertFile = vertFile;
	_fragFile = fragFile;
	__super::initRendCommand();
	
	return true;
}
bool OpenglStateMultAttr::isUseEBORender()
{
	return false;
}

void OpenglStateMultAttr::rendeCommand()
{
	//float timeValue = glfwGetTime();
	//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	//int vertexColorLocation = glGetUniformLocation(_shaderProgram, "ourColor");
	//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	__super::rendeCommand();
	

	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

int OpenglStateMultAttr::getShaderIndex()
{
	return 3;
}

void OpenglStateMultAttr::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(colorLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
}
