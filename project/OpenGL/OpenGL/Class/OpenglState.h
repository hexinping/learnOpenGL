#pragma once
#include <iostream>
#include "OpenglUtils.h"
#include "MathUtils.h"
#include "OpenglCamera.h"

extern int _countId;

using namespace std;
class OpenglState
{
public:

	OpenglState();
	~OpenglState(){};
	virtual bool init(string vertFile, string fragFile);
	void initRendCommand();
	virtual bool isUseEBORender();
	virtual void rendeCommand();
	virtual void lightRendeCommand();
	virtual void enableVertexAttribArray();
	virtual void enableLightVertexAttribArray();
	virtual bool isRenderModel();
	virtual bool isLihgtAction();
	virtual bool isUseCustomLightPos(); //是否使用自定义的光源位置
	virtual int  getPointLights();
	virtual float  getExposure();
	virtual bool  isShowLight();
	virtual bool  isUsePlane();

	//是否让大世界创建帧缓冲
	virtual bool  isUseFrameBuffer();

	//是否使用大世界创建的帧缓冲
	virtual bool  isRenderFrameBuffer();
	virtual bool  isUseSkyBox();
	virtual bool  isUseReflect();
	virtual bool  isUseBloom();
	virtual bool  isDelayRenderLights(); //是否延迟光照渲染计算
	virtual bool  isUseSSAO(); //是否使用环境光遮罩
	virtual bool  isUseMSAA(); //是否使用多重采样  默认开启

	virtual bool afterModleRender(); //渲染模型之后

	virtual bool  isUseHDR();
	//帧缓冲是否使用多个纹理附件
	virtual bool  isUseMultipleTextureId();


	virtual string  getModelFile();
	GLint getUniformLocation(unsigned int program, const char *name);
	GLint getAttribLocation(unsigned int program, const char *name);
	void useProgram(unsigned int program);

	virtual int getShaderIndex();

	void deleteBuffers();



	void setBool(unsigned int program, const char *name, bool value) const;
	void setInt(unsigned int program, const char *name, int value) const;
	void setFloat(unsigned int program, const char *name, float value) const;
	/*void setMat4(unsigned int program, const char *name, glm::mat4 *mat);
	void setVec3(unsigned int program, const char *name, float x, float y, float z);
	void setVec4(unsigned int program, const char *name, float x, float y, float z, float w);*/
	

	void setMat2(unsigned int program, const char *name, glm::mat2 &mat);
	void setMat3(unsigned int program, const char *name, glm::mat3 &mat);
	void setMat4(unsigned int program, const char *name, glm::mat4 *mat);

	void setVec2(unsigned int program, const char *name, float x, float y);
	void setVec3(unsigned int program, const char *name, float x, float y, float z);
	void setVec4(unsigned int program, const char *name, float x, float y, float z, float w);

	void setVec2(unsigned int program, const char *name, glm::vec2 &value);
	void setVec3(unsigned int program, const char *name, glm::vec3 &value);
	void setVec4(unsigned int program, const char *name, glm::vec4 &value);

	unsigned int loadTexture(const char *file, int textureUnit = GL_TEXTURE0);
	unsigned int loadCubemap(vector<std::string> &faces, bool isRevert = false);

	void setModelMat4(glm::vec3 tranVec, glm::vec3 scaleVec, glm::vec3 rorateVec, float angle);

	void setLight(bool light);

	void bindTexture(unsigned int textureId, bool isCube = false);
	void activiteTexture(int textureUnit);
public:
	OpenglUtils * _glUtils;
	MathUtils   * _mathUtils;
	bool _isUseEBORender;
	unsigned int _VBO, _VAO,_EBO;
	unsigned int _vertexShader;
	unsigned int _fragmentShader;
	unsigned int _shaderProgram;
	unsigned int _lightShaderProgram;

	glm::mat4 _modelMat4;
	bool _useModelMat4;

	//单独为光源配置一个vao 与模型的vao分开
	bool _isLight;
	unsigned int _lightVBO, _lightVAO, _lightEBO;


	float _vertices[2048];
	unsigned int _indices[2048];

	float _lightVertices[1024];
	unsigned int _lightIndices[1024];

	string _vertFile;
	string _fragFile;

	float  _param1 = 0.0f;
	glm::vec3 _param2 = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 _param3 = glm::vec3(0.0f, 0.0f, 0.0f);
	float _param4 = 0.0f;

	glm::mat4 _viewMat4; //观察矩阵

	glm::vec3 _cameroPosOffset = glm::vec3(0.0); // 摄像机位置偏移

	unsigned int _texture0;
	unsigned int _texture1;
	unsigned int _texture2;
	unsigned int _texture3;
	unsigned int _texture4;
	unsigned int _texture5;
	unsigned int _texture6;
	unsigned int _texture7;


	unsigned int _cubemapTexture;

	//帧缓冲对象
	unsigned int _framebuffer;
	unsigned int _textureColorbuffer;
	unsigned int _multFrambuffer;

	//唯一ID
	int _ID;


	glm::mat4 *modelMatrices;
	unsigned int _amount = 1;

	bool _isUseHDR = false;
	bool _isUseBloom = false;

	//曝光色调映射的曝光值
	float _exposure = 0.0f;


	//光源模型信息
	std::vector<glm::vec3> _lightPositions;			//位置
	std::vector<glm::vec3> _lightScale;				//缩放
	std::vector<float> _lightRotateAngle;			//旋转角度
	std::vector<glm::vec3> _lightRotateAxis;       //旋转轴
	std::vector<glm::vec3> _lightColors;            //颜色


	unsigned int _brightTexture; //明亮区域的纹理
	unsigned int _blurTexture;


	//延迟光照
	unsigned int _delayeRenderLightPosTexture;             
	unsigned int _delayeRenderLightNormalTexture;
	unsigned int _delayeRenderLightAlbedoSpecTexture;

	//默认不使用延迟光照技术
	bool _isUseDelayeRenderLight = false;

	//SSAO 需要的缓冲数据
	unsigned int _ssaoPosTexture;
	unsigned int _ssaoNormalTexture;
	unsigned int _ssaoAlbedoSpecTexture;

	//camrea
	OpenglCamera *_camera;

	//window
	GLFWwindow * _window;

};
