#include "OpenglStateMultTexture3D.h"

OpenglStateMultTexture3D::OpenglStateMultTexture3D()
{
	OpenglState::OpenglState();
}

bool OpenglStateMultTexture3D::init(string vertFile, string fragFile)
{
	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // ����
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // ����
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // ����
	};

	//float vertices[] = {
	//	// positions          // colors           // texture coords (note that we changed them to 'zoom in' on our texture image)
	//	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.55f, 0.55f, // top right
	//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.55f, 0.45f, // bottom right
	//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.45f, 0.45f, // bottom left
	//	-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.45f, 0.55f  // top left 
	//};


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

	glm::mat4 model;
	glm::vec3 asix(1.0f, 0.0f, 0.0f); //��X����ת
	//90.0f ��ʱ��
	_mathUtils->setRotateMat4(&model, -55.0f, &asix);

	glm::mat4 view;
	// ע�⣬���ǽ�����������Ҫ�����ƶ������ķ������ƶ���

	glm::vec3 trans(0.0f, 0.0f, -3.0f);
	_mathUtils->setTranslateMat4(&view, &trans);

	float width = 800.0f;
	float height = 600.0f;
	glm::mat4 projection;
	_mathUtils->setPerspective(&projection, 45.0f, width, height, 0.1f, 100.0f);

	setMat4(_shaderProgram, "model", &model);
	setMat4(_shaderProgram, "view", &view);
	setMat4(_shaderProgram, "projection", &projection);


	return true;
}
bool OpenglStateMultTexture3D::isUseEBORender()
{
	return true;
}

void OpenglStateMultTexture3D::rendeCommand()
{
	//glUseProgram����֮ǰ���ñ��ָ���
	/*
	���Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram)����Ϊ�����ڵ�ǰ�������ɫ������������uniform�ġ�
	*/
	__super::rendeCommand();
	setFloat(_shaderProgram, "textureAlpha", _param1);


	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //ʹ����������
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

int OpenglStateMultTexture3D::getShaderIndex()
{
	return 8;
}

void OpenglStateMultTexture3D::enableVertexAttribArray()
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
