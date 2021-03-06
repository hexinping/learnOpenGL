#include "OpenglStateTexture.h"

bool OpenglStateTexture::init(string vertFile, string fragFile)
{
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // 左上
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	int len1 = sizeof(vertices) / sizeof(float);
	int len2 = sizeof(indices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);
	memcpy(_indices, indices, sizeof(float) * len2);

	_vertFile = vertFile;
	_fragFile = fragFile;

	_texture0 = loadTexture("resource/mode2.png", GL_TEXTURE0);
	__super::initRendCommand();
	
	return true;
}
bool OpenglStateTexture::isUseEBORender()
{
	return true;
}

void OpenglStateTexture::rendeCommand()
{
	__super::rendeCommand();


	/*
	
		颜色矩阵：
		1 5 9 13 ==》决定R通道
		2 6 10 14 ==》决定G通道
		3 7 11 15 ==》决定B通道
		4 8 12 16 ==》决定A通道
	
	*/

	//R通道
	glm::mat4 colorMultiplier = glm::mat4(
		0.5, 0, 0, 0, 
		0.5, 0, 0, 0,
		0.5, 0, 0.0, 0,
		0.5, 0, 0, 0);

	//G通道
	//glm::mat4 colorMultiplier = glm::mat4(
	//	0, 0.5, 0, 0,
	//	0, 0.5, 0, 0,
	//	0, 0.5, 0.0, 0,
	//	0, 0.5, 0, 0);

	//B通道
	//glm::mat4 colorMultiplier = glm::mat4(
	//	0, 0, 0.5, 0,
	//	0, 0, 0.5, 0,
	//	0, 0, 0.5, 0,
	//	0, 0, 0.5, 0);

	//A通道： 感觉没效果
	//glm::mat4 colorMultiplier = glm::mat4(
	//	1.0, 0, 0, -2.0,
	//	0, 1.0, 0, -2.0,
	//	0, 0, 1.0, -2.0,
	//	0, 0, 0, -2.0);

	

	setMat4(_shaderProgram, "u_colorMultiplier", &colorMultiplier);

	//颜色偏移量
	setVec4(_shaderProgram,"u_colorOffset",0,0,0,0);


	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

int OpenglStateTexture::getShaderIndex()
{
	return 5;
}

void OpenglStateTexture::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(colorLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的


	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoord");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(aTexCoordLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
}

