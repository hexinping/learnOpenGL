#include "OpenglStateMultTextureMaterialMapMultLights.h"

OpenglStateMultTextureMaterialMapMultLights::OpenglStateMultTextureMaterialMapMultLights()
{
	OpenglState::OpenglState();
}

bool OpenglStateMultTextureMaterialMapMultLights::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	float lightVertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	int len1 = sizeof(vertices) / sizeof(float);
	int len2 = sizeof(lightVertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);
	memcpy(_lightVertices, lightVertices, sizeof(float) * len2);

	_vertFile = vertFile;
	_fragFile = fragFile;

	//this->genTexImage2D("resource/container.jpg", GL_RGB, 0, GL_TEXTURE0, GL_REPEAT, GL_LINEAR);
	genTexImage2D("resource/container2.png", GL_RGBA, 0, GL_TEXTURE0,GL_REPEAT, GL_LINEAR);
	genTexImage2D("resource/container2_specular.png", GL_RGBA, 0, GL_TEXTURE1, GL_REPEAT, GL_LINEAR);
	genTexImage2D("resource/matrix.jpg", GL_RGB, 0, GL_TEXTURE2, GL_REPEAT, GL_LINEAR);

	setLight(true);

	__super::initRendCommand();


	/*
		����uniform���������ַ���

		��һ�֣���initRendCommand��ʹ��һ��glUseProgram(_shaderProgram)�����ʱ��ֵ��uniform����
		�ڶ��֣�����ѭ��rendeCommand�ﲻ������
	
	*/
	//��������Ԫ
	_glUtils->useProgram(_shaderProgram);// ��ʹ�������ɫ����������������uniform����

	//ͨ��ʹ��glUniform1i���ò����������Ǳ�֤��ÿ��uniform��������Ӧ����ȷ������Ԫ
	setInt(_shaderProgram, "material.diffuse", 0); // �����0�Ͷ�Ӧ��ǰ���GL_TEXTURE0
	setInt(_shaderProgram, "material.specular", 1); // �����1�Ͷ�Ӧ��ǰ���GL_TEXTURE1
	setInt(_shaderProgram, "material.emission", 2);

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

	//���ù�Դ����ɫ
	//setVec3(_shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "objectColor", 1.0f, 1.0f, 1.0f);

	//ambient���ʺ�diffuse���ʶ����óɸ��������ɫһ��
	//setVec3(_shaderProgram, "material.ambient", 1.0f, 0.5f, 0.31f);
	//setVec3(_shaderProgram, "material.diffuse", 1.0f, 0.5f, 0.31f);  // ����������ͼ������
	//setVec3(_shaderProgram, "material.specular", 0.5f, 0.5f, 0.5f);  // �ø߹���ͼ������
	setFloat(_shaderProgram, "material.shininess", 32.0f);


	//setVec3(_shaderProgram, "light.ambient", 0.2f, 0.2f, 0.2f);
	//setVec3(_shaderProgram, "light.diffuse", 0.5f, 0.5f, 0.5f);
	//setVec3(_shaderProgram, "light.specular", 1.0f, 1.0f, 1.0f);

	//���ù�Դ����ɫ
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "lightColor", lightColor);
	setVec3(_shaderProgram, "objectColor", lightColor);
	

	glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // ����Ӱ��
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // �ܵ͵�Ӱ��

	//setVec3(_shaderProgram, "light.ambient", ambientColor);
	//setVec3(_shaderProgram, "light.diffuse", diffuseColor);
	//setVec3(_shaderProgram, "light.specular", 1.0f, 1.0f, 1.0f);
	//setVec3(_shaderProgram, "light.direction", -0.2f, -1.0f, -0.3f); //��0.2f, 1.0f, 0.3f) ��������Դ��λ�õ�

	////���õ��Դ��˥������
	//setFloat(_shaderProgram, "light.constant", 1.0f);
	//setFloat(_shaderProgram, "light.linear", 0.09f);
	//setFloat(_shaderProgram, "light.quadratic", 0.032f);

	//����ⷽ��
	setVec3(_shaderProgram, "dirLight.direction", -0.2f, -1.0f, -0.3f); //��0.2f, 1.0f, 0.3f) ��������Դ��λ�õ�
	setVec3(_shaderProgram, "dirLight.ambient", 0.05f, 0.05f, 0.05f);
	setVec3(_shaderProgram, "dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	setVec3(_shaderProgram, "dirLight.specular", 0.5f, 0.5f, 0.5f);

	//���Դ
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	std::string s = "pointLights";
	for (unsigned int i = 0; i < 4; i++)
	{
		std::string positon = s;
		positon = positon + "[" + to_string(i) + "].position";

		std::string ambient = s;
		ambient = ambient + "[" + to_string(i) + "].ambient";

		std::string diffuse = s;
		diffuse = diffuse + "[" + to_string(i) + "].diffuse";

		std::string specular = s;
		specular = specular + "[" + to_string(i) + "].specular";

		std::string constant = s;
		constant = constant + "[" + to_string(i) + "].constant";

		std::string linear = s;
		linear = linear + "[" + to_string(i) + "].linear";

		std::string quadratic = s;
		quadratic = quadratic + "[" + to_string(i) + "].quadratic";

		setVec3(_shaderProgram, positon.c_str(), pointLightPositions[i]);

		//���ù�Դ�����Ĳ�ͬǿ��
		setVec3(_shaderProgram, ambient.c_str(), 0.05f, 0.05f, 0.05f);
		setVec3(_shaderProgram, diffuse.c_str(), 0.8f, 0.8f, 0.8f);
		setVec3(_shaderProgram, specular.c_str(), 1.0f, 1.0f, 1.0f);

		//���õ��Դ��˥������
		setFloat(_shaderProgram, constant.c_str(), 1.0f);
		setFloat(_shaderProgram, linear.c_str(), 0.09);
		setFloat(_shaderProgram, quadratic.c_str(), 0.032);
	}
	

	//�۹�
	setVec3(_shaderProgram, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
	setVec3(_shaderProgram, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "spotLight.specular", 1.0f, 1.0f, 1.0f);
	setFloat(_shaderProgram, "spotLight.constant", 1.0f);
	setFloat(_shaderProgram, "spotLight.linear", 0.09);
	setFloat(_shaderProgram, "spotLight.quadratic", 0.032);
	setFloat(_shaderProgram, "spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	setFloat(_shaderProgram, "spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));



	return true;
}
bool OpenglStateMultTextureMaterialMapMultLights::isUseEBORender()
{
	return false;
}

void OpenglStateMultTextureMaterialMapMultLights::rendeCommand()
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

	
	//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
	//setVec3(_shaderProgram, "lightPos", lightPos.x, lightPos.y, lightPos.z);		// ��Դ��λ��
	//setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);		// �������λ�ã��۲�ռ��ԭ�㣩

	//setVec3(_shaderProgram, "light.position", lightPos);

	//�۹�����
	setVec3(_shaderProgram, "spotLight.position", cameraPos);
	setVec3(_shaderProgram, "spotLight.direction", cameraFront);



	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //ʹ����������
	}
	else
	{
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f, 3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f, 2.0f, -2.5f),
			glm::vec3(1.5f, 0.2f, -1.5f),
			glm::vec3(-1.3f, 1.0f, -1.5f)
		};
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i+1);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			setMat4(_shaderProgram, "model", &model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//���Դ��λ��
		if (_isLight)
		{	
			__super::lightRendeCommand();
			glm::vec3 pointLightPositions[] = {
				glm::vec3(0.7f, 0.2f, 2.0f),
				glm::vec3(2.3f, -3.3f, -4.0f),
				glm::vec3(-4.0f, 2.0f, -12.0f),
				glm::vec3(0.0f, 0.0f, -3.0f)
			};
			for (unsigned int i = 0; i < 4; i++)
			{
				glm::mat4 model;
				model = glm::translate(model, pointLightPositions[i]);
				//model = glm::scale(model, glm::vec3(0.05f));
				setMat4(_lightShaderProgram, "model", &model);

				setMat4(_lightShaderProgram, "view", &view);
				setMat4(_lightShaderProgram, "projection", &projection);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			
		}
	}
}

int OpenglStateMultTextureMaterialMapMultLights::getShaderIndex()
{
	return 16;
}

void OpenglStateMultTextureMaterialMapMultLights::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(posLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	//GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	//glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	//glEnableVertexAttribArray(colorLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	GLint normalLocation = _glUtils->getAttribLocation(_shaderProgram, "aNormal");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(normalLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoords");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(aTexCoordLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

}

void OpenglStateMultTextureMaterialMapMultLights::enableLightVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(posLocation); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ�
}


