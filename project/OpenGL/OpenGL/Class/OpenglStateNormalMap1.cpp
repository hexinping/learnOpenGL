#include "OpenglStateNormalMap1.h"

bool OpenglStateNormalMap1::init(string vertFile, string fragFile)
{

	// positions
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// triangle 1
	// ----------
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);


	float vertices[] = {
		// positions            // normal         // texcoords  // tangent                          // bitangent
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};
	//float vertices[] = {
	//	// positions            // normals         // texcoords
	//	10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
	//	-10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	//	-10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,

	//	10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
	//	-10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
	//	10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f
	//};

	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);

	_vertFile = vertFile;
	_fragFile = fragFile;

	_texture0 = _glUtils->loadTexture("resource/brickwall.jpg");
	_texture1 = _glUtils->loadTexture("resource/brickwall_normal.jpg");


	__super::initRendCommand();


	/*
		设置uniform变量有两种方法

		第一种：在initRendCommand后使用一次glUseProgram(_shaderProgram)，这个时候赋值给uniform变量
		第二种：在主循环rendeCommand里不断设置
	
	*/
	//设置纹理单元
	_glUtils->useProgram(_shaderProgram);// 先使用这个着色器程序对象才能设置uniform变量

	//通过使用glUniform1i设置采样器，我们保证了每个uniform采样器对应着正确的纹理单元
	setInt(_shaderProgram, "material.diffuse", 0); // 这里的0就对应了前面的GL_TEXTURE0
	//setInt(_shaderProgram, "material.specular", 1); // 这里的1就对应了前面的GL_TEXTURE1
	//setInt(_shaderProgram, "material.emission", 2);
	setInt(_shaderProgram, "material.normal", 1);

	//glm::mat4 trans;
	//float angle = 90.0f;
	//glm::vec3 axis(0.0, 0.0, 1.0);
	//glm::vec3 s(0.5, 0.5, 0.5);
	//
	//_mathUtils->setRoateMat4(&trans, angle, &axis);
	//_mathUtils->setSclaeMat4(&trans, &s);

	////先缩放 再旋转 ==》与设置顺序相反
	//setMat4(_shaderProgram, "transform", &trans);

	//glm::mat4 model;
	//glm::vec3 asix(1.0f, 0.0f, 0.0f); //绕X轴旋转
	////90.0f 逆时针
	//_mathUtils->setRotateMat4(&model, -55.0f, &asix);

	

	//float width = 800.0f;
	//float height = 600.0f;
	//glm::mat4 projection;
	//_mathUtils->setPerspective(&projection, 45.0f, width, height, 0.1f, 100.0f);

	//setMat4(_shaderProgram, "model", &model);
	//setMat4(_shaderProgram, "view", &view);
	//setMat4(_shaderProgram, "projection", &projection);

	//设置光源的颜色
	//setVec3(_shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "objectColor", 1.0f, 1.0f, 1.0f);

	//ambient材质和diffuse材质都设置成跟物体的颜色一样
	//setVec3(_shaderProgram, "material.ambient", 1.0f, 0.5f, 0.31f);
	//setVec3(_shaderProgram, "material.diffuse", 1.0f, 0.5f, 0.31f);  // 用漫反射贴图代替了
	//setVec3(_shaderProgram, "material.specular", 0.5f, 0.5f, 0.5f);  // 用高光贴图代替了
	setFloat(_shaderProgram, "material.shininess", 32.0f);


	//setVec3(_shaderProgram, "light.ambient", 0.2f, 0.2f, 0.2f);
	//setVec3(_shaderProgram, "light.diffuse", 0.5f, 0.5f, 0.5f);
	//setVec3(_shaderProgram, "light.specular", 1.0f, 1.0f, 1.0f);

	//设置光源的颜色
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "lightColor", lightColor);
	

	glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

	setVec3(_shaderProgram, "light.ambient", ambientColor);
	setVec3(_shaderProgram, "light.diffuse", diffuseColor);
	setVec3(_shaderProgram, "light.specular", 1.0f, 1.0f, 1.0f);
	//setVec3(_shaderProgram, "light.direction", -0.2f, -1.0f, -0.3f); //（0.2f, 1.0f, 0.3f) 可以理解光源的位置点

	//设置点光源的衰减变量
	setFloat(_shaderProgram, "light.constant", 1.0f);
	setFloat(_shaderProgram, "light.linear", 0.09f);
	setFloat(_shaderProgram, "light.quadratic", 0.032f);


	return true;
}
bool OpenglStateNormalMap1::isUseEBORender()
{
	return false;
}

bool OpenglStateNormalMap1::isLihgtAction()
{
	return false;
}

int  OpenglStateNormalMap1::getPointLights()
{
	return 1;
}

bool OpenglStateNormalMap1::isShowLight()
{
	return true;
}


void OpenglStateNormalMap1::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/
	__super::rendeCommand();

	activiteTexture(GL_TEXTURE0);
	bindTexture(_texture0);
	setInt(_shaderProgram, "material.diffuse", 0); // 这里的0就对应了前面的GL_TEXTURE0

	activiteTexture(GL_TEXTURE1);
	bindTexture(_texture1);
	setInt(_shaderProgram, "material.normal", 1); // 这里的1就对应了前面的GL_TEXTURE1

	//activiteTexture(GL_TEXTURE2);
	//bindTexture(_texture2);
	//setInt(_shaderProgram, "material.emission", 2); // 这里的1就对应了前面的GL_TEXTURE2

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
	setMat4(_shaderProgram, "projection", &projection);

	glm::mat4 view;
	glm::vec3 cameraPos = _param2;
	glm::vec3 cameraFront = _param3;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	setMat4(_shaderProgram, "view", &view);	

	
	glm::vec3 lightPos(0.5f, 1.0f, 3.3f);
	//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
	setVec3(_shaderProgram, "lightPos", lightPos.x, lightPos.y, lightPos.z);		// 光源的位置
	setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);		// 摄像机的位置（观察空间的原点）

	setVec3(_shaderProgram, "light.position", lightPos);

	glm::mat4 model;
	//model = glm::rotate(model, glm::radians(70.0f), glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
	setMat4(_shaderProgram, "model", &model);
	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

int OpenglStateNormalMap1::getShaderIndex()
{
	return 32;
}

void OpenglStateNormalMap1::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	//glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	//glEnableVertexAttribArray(colorLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint normalLocation = _glUtils->getAttribLocation(_shaderProgram, "aNormal");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(normalLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoords");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(aTexCoordLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint aTangentLocation = _glUtils->getAttribLocation(_shaderProgram, "aTangent");
	glVertexAttribPointer(aTangentLocation, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(aTangentLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint aBitangentLocation = _glUtils->getAttribLocation(_shaderProgram, "aBitangent");
	glVertexAttribPointer(aBitangentLocation, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(aBitangentLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

}



