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

	unsigned int ssaoFBO;                //SSAO֡����==�������ڱ�����
	unsigned int ssaoBlurFBO;            //ģ��֡����==���Ż������ڱ�Ч��

	//��ɫ������
	unsigned int _shaderSSAO;
	unsigned int _shaderSSAOBlur;
	unsigned int _shaderLightingPass;

	unsigned int ssaoColorBuffer;
	unsigned int ssaoColorBufferBlur;

	//��Դ��Ϣ
	glm::vec3 lightPos;
	glm::vec3 lightColor;

	glm::mat4 _projection;


	//���������
	std::vector<glm::vec3> ssaoKernel;

	//�������
	unsigned int noiseTexture;

   
};



