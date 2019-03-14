#include "OpenglStatePBR_IBL_Irradiance_Conversion.h"
#include <random>
#include "stb_image.h"
/*

PBR IBL(基于图像光照)  漫反射辐照   

irradiance 辐照度

*/

//窗口大小为800 * 600 最好大于这个比例 也要是2的次幂方
int cfgwidth  = 1024;
int cfgheight = 1024;

bool OpenglStatePBR_IBL_Irradiance_Conversion::init(string vertFile, string fragFile)
{

	_vertFile = vertFile;
	_fragFile = fragFile;
	//_cameroPosOffset = glm::vec3(0.0, 4.5, 8.0);

	__super::initRendCommand();


	//初始化 shaders
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/cubemap.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/equirectangular_to_cubemap.frag");
	_glUtils->linkShader(&_equirectangularToCubemapShader, _vertexShader, _fragmentShader);

	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/PBR_IBL_background.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/PBR_IBL_background.frag");
	_glUtils->linkShader(&_backgroundShader, _vertexShader, _fragmentShader);

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

	
	// set depth function to less than AND equal for skybox depth trick. ==> 需要立方体贴图
	glDepthFunc(GL_LEQUAL);

	//********* pbr: setup framebuffer
	// ---------------------------------------------------------

	glGenFramebuffers(1, &captureFBO);  //生成帧缓冲对象  ==》暂时只创建rbo附件，纹理附件后面用立方体贴图创建
	glGenRenderbuffers(1, &captureRBO); //生成渲染缓冲对象

	//把深度信息存到帧缓冲的渲染对象里
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);									//绑定帧缓冲对象
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);								//绑定渲染缓冲对象
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cfgwidth, cfgheight);			//内部格式设置为GL_DEPTH24_STENCIL8

	////将渲染缓冲对象附加到帧缓冲的深度附件上
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	//解绑帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// *********pbr: load the HDR environment map
	// ---------------------------------

	//_texture0 = _texture0 = loadTexture("resource/hdr/newport_loft.hdr", GL_TEXTURE0);
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float *data = stbi_loadf("resource/hdr/newport_loft.hdr", &width, &height, &nrComponents, 0);
	//unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);

		//GL_RGB16F 浮点格式
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	// *********pbr: setup cubemap to render to and attach to framebuffer
	// ---------------------------------------------------------
	//unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cfgwidth, cfgheight, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//********* pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------
	captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

	captureViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	captureViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	captureViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	captureViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	captureViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	captureViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	////********* pbr: convert HDR equirectangular environment map to cubemap equivalent
	//// ----------------------------------------------------------------------

	//glUseProgram(_equirectangularToCubemapShader);
	//activiteTexture(GL_TEXTURE0);
	//bindTexture(hdrTexture);
	//setInt(_equirectangularToCubemapShader, "equirectangularMap", 0);
	//setMat4(_equirectangularToCubemapShader, "projection", &captureProjection);
	//

	//glViewport(0, 0, cfgwidth, cfgheight); // don't forget to configure the viewport to the capture dimensions.

	////绑定帧缓冲，绘制到帧缓冲区，这步之后envCubemap就有数据了
	//glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	//for (unsigned int i = 0; i < 6; ++i)
	//{
	//	setMat4(_equirectangularToCubemapShader, "view", &captureViews[i]);
	//	//把立方体贴图作为帧缓冲的纹理附件
	//	//生成颜色立方体贴图
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//	//绘制立方体
	//	glBindVertexArray(cubeVAO);
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//	glBindVertexArray(0);
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// initialize static shader uniforms before rendering
	// --------------------------------------------------


	// then before rendering, configure the viewport to the original framebuffer's screen dimensions
	//int scrWidth, scrHeight;
	//glfwGetFramebufferSize(_window, &scrWidth, &scrHeight);
	//glViewport(0, 0, scrWidth, scrHeight);

	return true;
}
bool OpenglStatePBR_IBL_Irradiance_Conversion::isUseEBORender()
{
	return false;
}

bool OpenglStatePBR_IBL_Irradiance_Conversion::isUseFrameBuffer()
{
	return false;
}


int  OpenglStatePBR_IBL_Irradiance_Conversion::getPointLights()
{
	return 4;
}

bool OpenglStatePBR_IBL_Irradiance_Conversion::isShowLight()
{
	return false;
}

bool OpenglStatePBR_IBL_Irradiance_Conversion::isUseCustomLightPos()
{
	return true;
}

bool OpenglStatePBR_IBL_Irradiance_Conversion::isDelayRenderLights()
{
	return false;
}

bool OpenglStatePBR_IBL_Irradiance_Conversion::isRenderFrameBuffer()
{
	return false;
}


void OpenglStatePBR_IBL_Irradiance_Conversion::rendeCommand()
{
	//生成帧缓冲里的纹理附件数据
	//********* pbr: convert HDR equirectangular environment map to cubemap equivalent
	glUseProgram(_equirectangularToCubemapShader);
	activiteTexture(GL_TEXTURE0);
	bindTexture(hdrTexture);
	setInt(_equirectangularToCubemapShader, "equirectangularMap", 0);
	setMat4(_equirectangularToCubemapShader, "projection", &captureProjection);

	glViewport(0, 0, cfgwidth, cfgheight); // don't forget to configure the viewport to the capture dimensions.

	//绑定帧缓冲，绘制到帧缓冲区，这步之后envCubemap就有数据了
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		setMat4(_equirectangularToCubemapShader, "view", &captureViews[i]);
		//把立方体贴图作为帧缓冲的纹理附件
		//生成颜色立方体贴图
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//绘制立方体
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	//恢复到默认帧缓冲以及重置映射窗口大小
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	__super::rendeCommand();
	setVec3(_shaderProgram,"albedo", 0.5f, 0.0f, 0.0f);
	setFloat(_shaderProgram,"ao", 1.0f);

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
		setFloat(_shaderProgram,"metallic", (float)row / (float)nrRows);
		for (int col = 0; col < nrColumns; ++col)
		{
			// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			setFloat(_shaderProgram,"roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

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


	// render skybox (render as last to prevent overdraw)
	glUseProgram(_backgroundShader);
	setMat4(_backgroundShader, "projection", &projection);
	setMat4(_backgroundShader, "view", &_viewMat4);

	activiteTexture(GL_TEXTURE0);
	bindTexture(envCubemap, true);
	setInt(_backgroundShader, "environmentMap", 0);
	//绘制立方体
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}

int OpenglStatePBR_IBL_Irradiance_Conversion::getShaderIndex()
{
	return 45;
}


void OpenglStatePBR_IBL_Irradiance_Conversion::enableVertexAttribArray()
{
	//立方体数据
	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
		-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f  // bottom-left        
	};

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	// fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// link vertex attributes
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//球体数据
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



