#include "OpenglStateTexture.h"
OpenglStateTexture::OpenglStateTexture()
{
	OpenglState::OpenglState();
}

bool OpenglStateTexture::init(string vertFile, string fragFile)
{
	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // ����
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // ����
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // ����
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

	this->genTexImage2D("resource/container.jpg", GL_RGB, 0);
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
	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //ʹ����������
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
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(posLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(colorLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�


	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoord");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(aTexCoordLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�
}

