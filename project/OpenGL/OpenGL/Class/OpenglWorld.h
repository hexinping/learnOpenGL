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


	void createFrameBuffer(int screenWidth, int screenHeight, unsigned int *framebuffer, unsigned int *texColorBuffer, GLenum format1 = GL_RGB, GLenum format2 = GL_RGB);
	void createFrameBufferByMultSample(int screenWidth, int screenHeight, unsigned int *framebuffer, GLenum format = GL_RGB);

	//添加多个纹理附件的帧缓冲
	void createFrameBufferByColorBuffers(int screenWidth, int screenHeight, unsigned int *framebuffer, unsigned int *texColorBuffer, unsigned int *texBrightColorBuffer, GLenum format1 = GL_RGB, GLenum format2 = GL_RGB);


	//延迟光照渲染计算 保存G缓冲
	void createFrameBufferByDelayRenderLights(int screenWidth, int screenHeight, unsigned int *framebuffer, unsigned int *pos, unsigned int *normal, unsigned int *albedoSpec);

	//使用环境遮罩 （与延迟光照一样，这里只是为了区分单独再加一个方法）
	void createFrameBufferBySSAO(int screenWidth, int screenHeight, unsigned int *framebuffer, unsigned int *pos, unsigned int *normal, unsigned int *albedoSpec);


	void setCubemapTexture(unsigned int cubemapTexture);

	void setUseHDR(bool useHDR);
	void setExposure(float exposure);

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

	//是否使用高动态范围
	bool _isUseHDR = false;

	//是否使用泛光效果
	bool _isUseBloom = false;
	unsigned int _blurTexture;

	//曝光色调映射的曝光值
	float _exposure = 0.0f;

	//默认渲染关照模型的shader
	string _vertFile = "shader/lamp.vert";
	string _fragFile = "shader/lamp.frag";

	bool _isUseCustomLightPos;//是否使用自定义的光源位置


	std::vector<glm::vec3> _lightPositions;			//位置
	std::vector<glm::vec3> _lightScale;				//缩放
	std::vector<float> _lightRotateAngle;			//旋转角度
	std::vector<glm::vec3> _lightRotateAxis;       //旋转轴
	std::vector<glm::vec3> _lightColors;            //颜色


	bool _isDelayRenderLights = false;


};
