#include "OpenglStatePBRTexture.h"
#include <random>

/*

PBR 带贴图模型


*/


bool OpenglStatePBRTexture::init(string vertFile, string fragFile)
{

	_vertFile = vertFile;
	_fragFile = fragFile;
	//_cameroPosOffset = glm::vec3(0.0, 4.5, 8.0);

	__super::initRendCommand();


	//光源位置
	_lightPositions.push_back(glm::vec3(-10.0f, 10.0f, 10.0f));
	_lightPositions.push_back(glm::vec3(10.0f, 10.0f, 10.0f));
	_lightPositions.push_back(glm::vec3(-10.0f, -10.0f, 10.0f));
	_lightPositions.push_back(glm::vec3(10.0f, -10.0f, 10.0f));

	//光的颜色
	_lightColors.push_back(glm::vec3(300.0f, 300.0f, 300.0f));
	_lightColors.push_back(glm::vec3(300.0f, 300.0f, 300.0f));
	_lightColors.push_back(glm::vec3(300.0f, 300.0f, 300.0f));
	_lightColors.push_back(glm::vec3(300.0f, 300.0f, 300.0f));


	_texture0 = loadTexture("resource/pbr/rusted_iron/albedo.png", GL_TEXTURE0);
	_texture1 = loadTexture("resource/pbr/rusted_iron/normal.png", GL_TEXTURE1); 
	_texture2 = loadTexture("resource/pbr/rusted_iron/metallic.png", GL_TEXTURE2);
	_texture3 = loadTexture("resource/pbr/rusted_iron/roughness.png", GL_TEXTURE3); 
	_texture4 = loadTexture("resource/pbr/rusted_iron/ao.png", GL_TEXTURE4);

	_glUtils->useProgram(_shaderProgram);

	setInt(_shaderProgram, "albedoMap", 0);
	setInt(_shaderProgram, "normalMap", 1);
	setInt(_shaderProgram, "metallicMap", 2);
	setInt(_shaderProgram, "roughnessMap", 3);
	setInt(_shaderProgram, "aoMap", 4);

	return true;
}
bool OpenglStatePBRTexture::isUseEBORender()
{
	return false;
}

bool OpenglStatePBRTexture::isUseFrameBuffer()
{
	return false;
}


int  OpenglStatePBRTexture::getPointLights()
{
	return 4;
}

bool OpenglStatePBRTexture::isShowLight()
{
	return false;
}

bool OpenglStatePBRTexture::isUseCustomLightPos()
{
	return true;
}

bool OpenglStatePBRTexture::isDelayRenderLights()
{
	return false;
}

bool OpenglStatePBRTexture::isRenderFrameBuffer()
{
	return false;
}


void OpenglStatePBRTexture::rendeCommand()
{
	__super::rendeCommand();
	//用贴图代替
	//setVec3(_shaderProgram,"albedo", 0.5f, 0.0f, 0.0f);
	//setFloat(_shaderProgram,"ao", 1.0f);


	activiteTexture(GL_TEXTURE0);
	bindTexture(_texture0);
	setInt(_shaderProgram, "albedoMap", 0); // 这里的0就对应了前面的GL_TEXTURE0

	activiteTexture(GL_TEXTURE1);
	bindTexture(_texture1);
	setInt(_shaderProgram, "normalMap", 1); // 这里的0就对应了前面的GL_TEXTURE0

	activiteTexture(GL_TEXTURE2);
	bindTexture(_texture2);
	setInt(_shaderProgram, "metallicMap", 2); // 这里的0就对应了前面的GL_TEXTURE0

	activiteTexture(GL_TEXTURE3);
	bindTexture(_texture3);
	setInt(_shaderProgram, "roughnessMap", 3); // 这里的0就对应了前面的GL_TEXTURE0

	activiteTexture(GL_TEXTURE4);
	bindTexture(_texture4);
	setInt(_shaderProgram, "aoMap", 4); // 这里的0就对应了前面的GL_TEXTURE0

	// initialize static shader uniforms before rendering
	// --------------------------------------------------

	glm::vec3 cameraPos = _param2;
	float cameraZoom = _param4;
	glm::mat4 projection = glm::perspective(glm::radians(cameraZoom), (float)800 / (float)600, 0.1f, 100.0f);
	setMat4(_shaderProgram,"projection", &projection);
	setMat4(_shaderProgram, "view", &_viewMat4);
	setVec3(_shaderProgram, "camPos", cameraPos);

	// render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;
	glm::mat4 model = glm::mat4(1.0f);
	for (int row = 0; row < nrRows; ++row)
	{
		//用贴图代替
		//setFloat(_shaderProgram,"metallic", (float)row / (float)nrRows);
		for (int col = 0; col < nrColumns; ++col)
		{
			// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			//用贴图代替
			//setFloat(_shaderProgram,"roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(col - (nrColumns / 2)) * spacing,
				(row - (nrRows / 2)) * spacing,
				0.0f
				));
			setMat4(_shaderProgram,"model", &model);

			//绘制球体
			glBindVertexArray(sphereVAO);
			glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
		}
	}

	// render light source (simply re-render sphere at light positions)
	// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
	// keeps the codeprint small.
	for (unsigned int i = 0; i < sizeof(_lightPositions) / sizeof(_lightPositions[0]); ++i)
	{
		glm::vec3 newPos = _lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = _lightPositions[i];
		std::string str = "lightPositions[" + std::to_string(i) + "]";
		setVec3(_shaderProgram, str.c_str(), newPos);
		str = "lightColors[" + std::to_string(i) + "]";
		setVec3(_shaderProgram, str.c_str(), _lightColors[i]);

		model = glm::mat4(1.0f);
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		setMat4(_shaderProgram,"model", &model);

		//绘制球体
		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
	}

}

int OpenglStatePBRTexture::getShaderIndex()
{
	return 44;
}


void OpenglStatePBRTexture::enableVertexAttribArray()
{

	glGenVertexArrays(1, &sphereVAO);

	unsigned int vbo, ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	indexCount = indices.size();

	std::vector<float> data;
	for (int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
	}

	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	float stride = (3 + 2 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
}



