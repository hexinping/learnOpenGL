#pragma once
#include <iostream>
#include <vector>
#include "OpenglState.h"


using namespace std;
class OpenglWorld
{
public:

	OpenglWorld();
	~OpenglWorld();
	void add(OpenglState *glState);
	void setLight(bool light);
	void init();
	void rendeCommand();
	void setLightAction(bool isAction);
	void setLightNum(int num);

public:
	vector<OpenglState *> _openglStateArray;

	OpenglUtils * _glUtils;
	MathUtils   * _mathUtils;
	bool _isLight;
	bool _islightAction;
	int _lightNums;
	unsigned int _lightVBO, _lightVAO, _lightEBO;
	unsigned int _lightShaderProgram;

	float _lightVertices[1024];
	unsigned int _lightIndices[1024];

	float  _param1 = 0.0f;
	glm::vec3 _param2 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _param3 = glm::vec3(0.0f, 0.0f, 0.0f);
	float _param4 = 0.0f;


};
