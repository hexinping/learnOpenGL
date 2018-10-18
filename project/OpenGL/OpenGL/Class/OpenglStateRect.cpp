#include "OpenglStateRect.h"


OpenglStateRect::OpenglStateRect()
{
	OpenglState::OpenglState();
}

bool OpenglStateRect::init(string vertFile, string fragFile)
{
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	unsigned int indices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
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
