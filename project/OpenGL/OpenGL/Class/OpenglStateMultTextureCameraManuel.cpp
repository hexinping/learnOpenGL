#include "OpenglStateMultTextureCameraManuel.h"

OpenglStateMultTextureCameraManuel::OpenglStateMultTextureCameraManuel()
{
	OpenglState::OpenglState();
}

bool OpenglStateMultTextureCameraManuel::init(string vertFile, string fragFile)
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);

	_vertFile = vertFile;
	_fragFile = fragFile;

	this->genTexImage2D("resource/container.jpg", GL_RGB, 0, GL_TEXTURE0, GL_REPEAT, GL_LINEAR);
	this->genTexImage2D("resource/awesomeface.png", GL_RGBA, 0, GL_TEXTURE1,GL_REPEAT, GL_LINEAR);
	__super::initRendCommand();


	/*
		����uniform���������ַ���

		��һ�֣���initRendCommand��ʹ��һ��glUseProgram(_shaderProgram)�����ʱ��ֵ��uniform����
		�ڶ��֣�����ѭ��rendeCommand�ﲻ������
	
	*/
	//����������Ԫ
	_glUtils->useProgram(_shaderProgram);// ��ʹ�������ɫ����������������uniform����

	//ͨ��ʹ��glUniform1i���ò����������Ǳ�֤��ÿ��uniform��������Ӧ����ȷ��������Ԫ
	setInt(_shaderProgram, "texture1", 0); // �����0�Ͷ�Ӧ��ǰ���GL_TEXTURE0
	setInt(_shaderProgram,"texture2", 1); // �����1�Ͷ�Ӧ��ǰ���GL_TEXTURE1

	//glm::mat4 trans;
	//float angle = 90.0f;
	//glm::vec3 axis(0.0, 0.0, 1.0);
	//glm::vec3 s(0.5, 0.5, 0.5);
	//
	//_mathUtils->setRoateMat4(&trans, angle, &axis);
	//_mathUtils->setSclaeMat4(&trans, &s);

	////������ ����ת ==��������˳���෴
	//setMat4(_shaderProgram, "transform", &trans);

	//glm::mat4 model;
	//glm::vec3 asix(1.0f, 0.0f, 0.0f); //��X����ת
	////90.0f ��ʱ��
	//_mathUtils->setRotateMat4(&model, -55.0f, &asix);

	

	//float width = 800.0f;
	//float height = 600.0f;
	//glm::mat4 projection;
	//_mathUtils->setPerspective(&projection, 45.0f, width, height, 0.1f, 100.0f);

	//setMat4(_shaderProgram, "model", &model);
	//setMat4(_shaderProgram, "view", &view);
	//setMat4(_shaderProgram, "projection", &projection);



	return true;
}
bool OpenglStateMultTextureCameraManuel::isUseEBORender()
{
	return false;
}

void OpenglStateMultTextureCameraManuel::rendeCommand()
{
	//glUseProgram����֮ǰ���ñ��ָ���
	/*
	���Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram)����Ϊ�����ڵ�ǰ�������ɫ������������uniform�ġ�
	*/
	__super::rendeCommand();
	setFloat(_shaderProgram, "textureAlpha", _param1);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
	setMat4(_shaderProgram, "projection", &projection);

	glm::mat4 view;
	glm::vec3 cameraPos = _param2;
	glm::vec3 cameraFront = _param3;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	setMat4(_shaderProgram, "view", &view);
	
	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //ʹ����������
	}
	else
	{
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			setMat4(_shaderProgram, "model", &model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

int OpenglStateMultTextureCameraManuel::getShaderIndex()
{
	return 9;
}

void OpenglStateMultTextureCameraManuel::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(posLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	//GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	//glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	//glEnableVertexAttribArray(colorLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�


	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoord");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(aTexCoordLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�
}
