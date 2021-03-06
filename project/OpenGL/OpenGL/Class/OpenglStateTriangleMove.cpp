#include "OpenglStateTriangleMove.h"

bool OpenglStateTriangleMove::init(string vertFile, string fragFile)
{
	float vertices[] = {
		-2.0f, -0.5f, 0.0f,
		-1.0f, -0.5f, 0.0f,
		-1.5f, 0.5f, 0.0f
	};
	int len = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len);
	_vertFile = vertFile;
	_fragFile = fragFile;
	__super::initRendCommand();
	
	return true;
}
bool OpenglStateTriangleMove::isUseEBORender()
{
	return false;
}

void OpenglStateTriangleMove::rendeCommand()
{


	__super::rendeCommand();
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(_shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


	offset = offset + _deltaTime * 0.5;
	if (offset > 3.0f)
	{
		offset = 0.0f;
	}
	vertexColorLocation = glGetUniformLocation(_shaderProgram, "xOffset");
	glUniform1f(vertexColorLocation, offset);

	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

int OpenglStateTriangleMove::getShaderIndex()
{
	return 4;
}
