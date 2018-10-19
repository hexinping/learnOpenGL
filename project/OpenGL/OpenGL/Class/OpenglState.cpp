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
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(posLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�
}

void OpenglState::rendeCommand()
{
	glUseProgram(_shaderProgram); //������ɫ����������Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ��
	glBindVertexArray(_VAO);     // ʹ��VAO�����ÿһ����Ⱦ��ʱ��ֱ��ʹ��VAO�洢�õ�����ָ��
}

GLint OpenglState::getUniformLocation(unsigned int program, const char *name)
{
	return _glUtils->getUniformLocation(program, name);
}

GLint OpenglState::getAttribLocation(unsigned int program, const char *name)
{
	return _glUtils->getAttribLocation(program, name);
}

int OpenglState::getShaderIndex()
{
	return 0;
}

void OpenglState::setBool(unsigned int program, const char *name, bool value) const
{
	_glUtils->setBool(program, name, value);
}
void OpenglState::setInt(unsigned int program, const char *name, int value) const
{
	_glUtils->setInt(program, name, value);
}
void OpenglState::setFloat(unsigned int program, const char *name, float value) const
{
	_glUtils->setFloat(program, name, value);
}


void OpenglState::genTexImage2D(const char *file, int type, int level, int textureUnit, int around, int filter)
{
	_glUtils->genTexImage2D(file, type, level, textureUnit, around, filter);
}

void OpenglState::useProgram(unsigned int program)
{
	_glUtils->useProgram(program);
}