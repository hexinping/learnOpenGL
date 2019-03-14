#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStatePBR_IBL_Irradiance :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	bool isUseFrameBuffer();
	int getPointLights();
	bool isShowLight();
	bool isUseCustomLightPos();
	bool isRenderFrameBuffer();

	bool isDelayRenderLights();
	void enableVertexAttribArray();



public:

	int cfgwidth = 1024;
	int cfgheight = 1024;

	unsigned int sphereVAO = 0;
	unsigned int indexCount = 0;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	//shader
	unsigned int _equirectangularToCubemapShader;
	unsigned int _backgroundShader;
	unsigned int _irradianceShader;

	//texture
	unsigned int hdrTexture;
	unsigned int envCubemap;
	unsigned int irradianceMap;


	glm::mat4 captureProjection;
	glm::mat4 captureViews[6];

	//÷°ª∫≥Â∂‘œÛ ‰÷»æª∫≥Â∂‘œÛ
	unsigned int captureFBO;
	unsigned int captureRBO;

};



