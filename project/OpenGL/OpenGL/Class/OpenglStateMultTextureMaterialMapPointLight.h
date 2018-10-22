#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTextureMaterialMapPointLight :
	public OpenglState
{
public:
	OpenglStateMultTextureMaterialMapPointLight();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	void enableLightVertexAttribArray();
};

