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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(0); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�
}

void OpenglState::rendeCommand()
{
	glUseProgram(_shaderProgram); //������ɫ����������Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ��
	glBindVertexArray(_VAO);     // ʹ��VAO�����ÿһ����Ⱦ��ʱ��ֱ��ʹ��VAO�洢�õ�����ָ��

	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //ʹ����������
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}