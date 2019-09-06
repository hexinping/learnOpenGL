#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateSSAO :
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
	string getModelFile();
	bool isRenderModel();

	bool isUseSSAO();
	bool isUseMSAA();
	bool isRenderFrameBuffer();

	bool isDelayRenderLights();
	void enableVertexAttribArray();

	bool afterGBufferRender();

public:

	unsigned int cubeVAO;
	unsigned int cubeVBO;

	unsigned int quadVAO;
	unsigned int quadVBO;

	unsigned int ssaoFBO;                //SSAO帧缓冲==》计算遮蔽因子
	unsigned int ssaoBlurFBO;            //模糊帧缓冲==》优化环境遮蔽效果

	//着色器程序
	unsigned int _shaderSSAO;
	unsigned int _shaderSSAOBlur;
	unsigned int _shaderLightingPass;

	unsigned int ssaoColorBuffer;
	unsigned int ssaoColorBufferBlur;

	//光源信息
	glm::vec3 lightPos;
	glm::vec3 lightColor;

	glm::mat4 _projection;


	//随机采样核
	std::vector<glm::vec3> ssaoKernel;

	//噪点纹理
	unsigned int noiseTexture;

   
};



