#include "OpenglStateRect.h"


OpenglStateRect::OpenglStateRect()
{
	OpenglState::OpenglState();
}

bool OpenglStateRect::init(string vertFile, string fragFile)
{
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};

	unsigned int indices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	memcpy(_vertices, vertices, sizeof(float) * 12);
	memcpy(_indices, indices, sizeof(float) * 6);

	_vertFile = vertFile;
	_fragFile = fragFile;
	__super::initRendCommand();

	return true;
}
bool OpenglStateRect::isUseEBORender()
{
	return true;
}
