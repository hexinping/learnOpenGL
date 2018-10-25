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
public:
	vector<OpenglState *> _openglStateArray;

	OpenglUtils * _glUtils;
	MathUtils   * _mathUtils;
	bool _isLight;
	unsigned int _lightVBO, _lightVAO, _lightEBO;
	unsigned int _lightShaderProgram;

	float _lightVertices[1024];
	unsigned int _lightIndices[1024];


};
