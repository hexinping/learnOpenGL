#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "OpenglState.h"
#include "Model.h"

using namespace std;
class OpenglWorld
{
public:

	OpenglWorld();
	~OpenglWorld();
	void add(OpenglState *glState, bool isFront=false);
	void setLight(bool light);
	void setUseFameBuffer(bool isUse);
	void init();
	void rendeCommand();
	void setLightAction(bool isAction);
	void setLightNum(int num);
	void addModelOpenglState(OpenglState *glState, int id);
	void addRenderModel(Model *model, int id);
	void useFrameBuffer(unsigned int framebuffer);

	void deleteBuffers(unsigned int framebuffer);


	void createFrameBuffer(int screenWidth,int screenHeight,unsigned int *framebuffer, unsigned int *texColorBuffer);
	void createFrameBufferByMultSample(int screenWidth, int screenHeight, unsigned int *framebuffer);

	void setCubemapTexture(unsigned int cubemapTexture);

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

	//是否使用自定义frameBuffer
	bool _isUseFrameBuffer;

	//天空盒纹理
	unsigned int _cubemapTexture;

	//模型存储容器
	map<int, OpenglState *> _creaetModelMap;
	map<int, Model *> _renderModelMap;


};
