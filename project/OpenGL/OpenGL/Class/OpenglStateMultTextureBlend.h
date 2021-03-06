#pragma once
#include <iostream>
#include "openglstate.h"

#include <vector>

class OpenglStateMultTextureBlend :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	bool isLihgtAction();
	int getPointLights();
	bool isShowLight();
	bool  isUsePlane();

	unsigned int transparentVAO, transparentVBO;

	//提取亮度的shader
	unsigned int _shaderBrightProgram;

	//水平模糊的shader
	unsigned int _shaderBlurHProgram;

	//垂直模糊的shader
	unsigned int _shaderBlurVProgram;

};

