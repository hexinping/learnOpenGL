#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTextureMaterialMapDirLight :
	public OpenglState
{
public:
	OpenglStateMultTextureMaterialMapDirLight();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	void enableLightVertexAttribArray();
};

