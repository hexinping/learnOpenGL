#include "practice_2_1.h"

practice_2_1::practice_2_1()
{
	OpenglState::OpenglState();
}

bool practice_2_1::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		// second triangle
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	memcpy(_vertices, vertices, sizeof(float) * 18);

	_vertFile = vertFile;
	_fragFile = fragFile;
	this->initRendCommand();

	return true;
}
bool practice_2_1::isUseEBORender()
{
	return false;
}

void practice_2_1::rendeCommand()
{
	glUseProgram(_shaderProgram); //激活着色器程序对象：已激活着色器程序的着色器将在我们发送渲染调用的时候被使用
	glBindVertexArray(_VAO);     // 使用VAO后就是每一次渲染的时候直接使用VAO存储好的属性指针
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
