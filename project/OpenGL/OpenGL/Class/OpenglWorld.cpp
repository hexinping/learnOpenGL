#include "OpenglWorld.h"

unsigned int _lightVBO, _lightVAO, _lightEBO;
unsigned int _lightShaderProgram;
OpenglWorld::OpenglWorld()
:_isLight(false)
,_glUtils(nullptr)
,_mathUtils(nullptr)
, _lightVBO(0)
, _lightVAO(0)
, _lightEBO(0)
, _lightShaderProgram(0)
{
	_isLight = false;
	_glUtils = OpenglUtils::getInstance();
	_mathUtils = MathUtils::getInstance();
}

void  OpenglWorld::init()
{
	if (_isLight)
	{
		unsigned int _lightVertexShader;
		unsigned int _lightFragmentShader;
		_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_lightVertexShader, "shader/lamp.vert");
		_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_lightFragmentShader, "shader/lamp.frag");
		_glUtils->linkShader(&_lightShaderProgram, _lightVertexShader, _lightFragmentShader);

		_glUtils->bindVBOAndVAO(&_lightVBO, &_lightVAO, _lightVertices, sizeof(_lightVertices), false, &_lightEBO, _lightIndices, sizeof(_lightIndices));
		GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
		glEnableVertexAttribArray(posLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�
	}
}

void OpenglWorld::add(OpenglState *glState)
{
	_openglStateArray.push_back(glState);
}

void OpenglWorld::setLight(bool light)
{
	_isLight = light;
}

void OpenglWorld::rendeCommand()
{
	//glUseProgram����֮ǰ���ñ��ָ���
	/*
	���Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram)����Ϊ�����ڵ�ǰ�������ɫ������������uniform�ġ�
	*/
	//__super::rendeCommand();
	//setFloat(_shaderProgram, "textureAlpha", _param1);

	//glm::mat4 projection;
	//projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
	//setMat4(_shaderProgram, "projection", &projection);

	//glm::mat4 view;
	//glm::vec3 cameraPos = _param2;
	//glm::vec3 cameraFront = _param3;
	//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	//setMat4(_shaderProgram, "view", &view);


	//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
	//setVec3(_shaderProgram, "lightPos", lightPos.x, lightPos.y, lightPos.z);		// ��Դ��λ��
	//setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);		// �������λ�ã��۲�ռ��ԭ�㣩

	//setVec3(_shaderProgram, "light.position", lightPos);

	//if (_isUseEBORender)
	//{
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //ʹ����������
	//}
	//else
	//{
	//	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//	glm::vec3 cubePositions[] = {
	//		glm::vec3(0.0f, 0.0f, 0.0f),
	//		glm::vec3(2.0f, 5.0f, -15.0f),
	//		glm::vec3(-1.5f, -2.2f, -2.5f),
	//		glm::vec3(-3.8f, -2.0f, -12.3f),
	//		glm::vec3(2.4f, -0.4f, -3.5f),
	//		glm::vec3(-1.7f, 3.0f, -7.5f),
	//		glm::vec3(1.3f, -2.0f, -2.5f),
	//		glm::vec3(1.5f, 2.0f, -2.5f),
	//		glm::vec3(1.5f, 0.2f, -1.5f),
	//		glm::vec3(-1.3f, 1.0f, -1.5f)
	//	};
	//	for (unsigned int i = 0; i < 10; i++)
	//	{
	//		glm::mat4 model;
	//		model = glm::translate(model, cubePositions[i]);
	//		float angle = 20.0f * (i + 1);
	//		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	//		setMat4(_shaderProgram, "model", &model);
	//		glDrawArrays(GL_TRIANGLES, 0, 36);
	//	}

	//	//��Դ��λ��
	//	if (_isLight)
	//	{
	//		__super::lightRendeCommand();


	//		glm::mat4 model;
	//		model = glm::translate(model, lightPos);
	//		model = glm::scale(model, glm::vec3(0.05f));
	//		setMat4(_lightShaderProgram, "model", &model);

	//		setMat4(_lightShaderProgram, "view", &view);
	//		setMat4(_lightShaderProgram, "projection", &projection);

	//		glDrawArrays(GL_TRIANGLES, 0, 36);
	//	}
	//}
}

OpenglWorld::~OpenglWorld()
{
	int size = _openglStateArray.size();
	for (int i = 0; i < size; i++)
	{
		OpenglState * glState = _openglStateArray[i];
		delete glState;
	}
	_openglStateArray.clear();
}



