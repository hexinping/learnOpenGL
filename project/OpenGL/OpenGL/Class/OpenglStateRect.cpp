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

	int len1 = sizeof(vertices) / sizeof(float);
	int len2 = sizeof(indices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);
	memcpy(_indices, indices, sizeof(float) * len2);

	_vertFile = vertFile;
	_fragFile = fragFile;
	this->initRendCommand();

	return true;
}

void OpenglStateRect::rendeCommand()
{
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

int OpenglStateRect::getShaderIndex()
{
	return 0;
}