#include "OpenglState.h"


OpenglState::OpenglState()
: _glUtils(nullptr)
, _mathUtils(nullptr)
, _isUseEBORender(true)
, _isLight(false)
, _VBO(0)
, _VAO(0)
, _EBO(0)
, _lightVBO(0)
, _lightVAO(0)
, _lightEBO(0)
, _vertexShader(0)
, _fragmentShader(0)
, _shaderProgram(0)
, _lightShaderProgram(0)
{
	_glUtils = OpenglUtils::getInstance();
	_mathUtils = MathUtils::getInstance();

}

bool OpenglState::init(string vertFile, string fragFile)
{
	this->initRendCommand();
	return true;
}

void OpenglState::initRendCommand()
{
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, _vertFile);
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, _fragFile);
	_glUtils->linkShader(&_shaderProgram, _vertexShader, _fragmentShader);

	_isUseEBORender = this->isUseEBORender();
	_glUtils->bindVBOAndVAO(&_VBO, &_VAO, _vertices, sizeof(_vertices), _isUseEBORender, &_EBO, _indices, sizeof(_indices));
	this->enableVertexAttribArray();
	if (_isLight)
	{
		glEnableVertexAttribArray(0);
		unsigned int _lightVertexShader;
		unsigned int _lightFragmentShader;
		_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_lightVertexShader, "shader/lamp.vert");
		_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_lightFragmentShader, "shader/lamp.frag");
		_glUtils->linkShader(&_lightShaderProgram, _lightVertexShader, _lightFragmentShader);

		
		_glUtils->bindVBOAndVAO(&_lightVBO, &_lightVAO, _lightVertices, sizeof(_lightVertices), false, &_lightEBO, _lightIndices, sizeof(_lightIndices));
		this->enableLightVertexAttribArray();
		glEnableVertexAttribArray(0);
	}

}

void OpenglState::setLight(bool light)
{
	_isLight = light;
}

bool OpenglState::isUseEBORender()
{
	return true;
}

bool OpenglState::isShowLight()
{
	return false;
}

void  OpenglState::setModelMat4(glm::vec3 tranVec, glm::vec3 scaleVec, glm::vec3 rorateVec, float angle)
{
	//先进行缩放操作，然后是旋转，最后才是位移
	_modelMat4 = glm::translate(_modelMat4, tranVec);
	_modelMat4 = glm::rotate(_modelMat4, glm::radians(angle), rorateVec);
	_modelMat4 = glm::scale(_modelMat4, scaleVec);
}

void OpenglState::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
}

void OpenglState::enableLightVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
}

void OpenglState::rendeCommand()
{
	glUseProgram(_shaderProgram); //激活着色器程序对象：已激活着色器程序的着色器将在我们发送渲染调用的时候被使用
	glBindVertexArray(_VAO);     // 使用VAO后就是每一次渲染的时候直接使用VAO存储好的属性指针
}

void OpenglState::lightRendeCommand()
{
	glUseProgram(_lightShaderProgram);
	glBindVertexArray(_lightVAO);
}

bool OpenglState::isLihgtAction()
{
	return false;
}

int  OpenglState::getPointLights()
{
	return 0;
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

bool OpenglState::isRenderModel()
{
	return false;
}

bool  OpenglState::isUsePlane()
{
	return false;
}

bool OpenglState::isUseFrameBuffer()
{
	return false;
}

unsigned int OpenglState::genTexImage2D(const char *file, int type, int level, int textureUnit, int around, int filter)
{
	return _glUtils->genTexImage2D(file, type, level, textureUnit, around, filter);
}

void OpenglState::bindTexture(unsigned int textureId)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void OpenglState::activiteTexture(int textureUnit)
{
	glActiveTexture(textureUnit);
}

void OpenglState::useProgram(unsigned int program)
{
	_glUtils->useProgram(program);
}

void OpenglState::setMat2(unsigned int program, const char *name, glm::mat2 &mat)
{
	_glUtils->setMat2(program, name, mat);
}

void OpenglState::setMat3(unsigned int program, const char *name, glm::mat3 &mat)
{
	_glUtils->setMat3(program, name, mat);
}

void OpenglState::setMat4(unsigned int program, const char *name, glm::mat4 *mat)
{
	_glUtils->setMat4(program, name, mat);
}


void OpenglState::setVec2(unsigned int program, const char *name, float x, float y)
{
	_glUtils->setVec2(program, name, x, y);
}

void OpenglState::setVec3(unsigned int program, const char *name, float x, float y, float z)
{
	_glUtils->setVec3(program, name, x, y, z);
}

void OpenglState::setVec4(unsigned int program, const char *name, float x, float y, float z, float w)
{
	_glUtils->setVec4(program, name, x, y, z, w);
}

void OpenglState::setVec2(unsigned int program, const char *name, glm::vec2 &value)
{
	_glUtils->setVec2(program, name, value);
}

void OpenglState::setVec3(unsigned int program, const char *name, glm::vec3 &value)
{
	_glUtils->setVec3(program, name, value);
}

void OpenglState::setVec4(unsigned int program, const char *name, glm::vec4 &value)
{
	_glUtils->setVec4(program, name, value);
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

