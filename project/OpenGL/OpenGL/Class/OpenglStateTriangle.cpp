#include "OpenglStateTriangle.h"


OpenglStateTriangle::OpenglStateTriangle()
{
	OpenglState::OpenglState();
}

bool OpenglStateTriangle::init(string vertFile, string fragFile)
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	memcpy(_vertices, vertices, sizeof(float) * 9);
	_vertFile = vertFile;
	_fragFile = fragFile;
	__super::initRendCommand();
	
	return true;
}
bool OpenglStateTriangle::isUseEBORender()
{
	return false;
}

void OpenglStateTriangle::rendeCommand()
{
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(_shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
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

int OpenglStateTriangle::getShaderIndex()
{
	return 2;
}
