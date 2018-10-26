#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTextureMaterial :
	public OpenglState
{
public:
	OpenglStateMultTextureMaterial();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	void enableLightVertexAttribArray();
	bool isLihgtAction();
	int getPointLights();
	bool isShowLight();
};

