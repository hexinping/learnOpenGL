
#include "main.h"
#include <iostream>
#include <map>
#include <ctime>
#include <cstdlib>
#include "OpenglUtils.h"
#include "OpenglCamera.h"
#include "OpenglState.h"
#include "OpenglStateTriangle.h"
#include "OpenglStateRect.h"
#include "OpenglStateMultAttr.h"
#include "OpenglStateTriangleMove.h"
#include "OpenglStateTexture.h"
#include "OpenglStateMultTexture.h"
#include "OpenglStateMultTextureMat4.h"
#include "OpenglStateMultTexture3D.h"
#include "OpenglStateMultTextureCube.h"
#include "OpenglStateMultTextureCamera.h"
#include "OpenglStateMultTextureCameraManuel.h"
#include "OpenglStateMultTextureLight.h"
#include "OpenglStateMultTextureMaterial.h"
#include "OpenglStateMultTextureMaterialMap.h"
#include "OpenglStateMultTextureMaterialMapPointLight.h"
#include "OpenglStateMultTextureMaterialMapDirLight.h"
#include "OpenglStateMultTextureMaterialMapSpotLight.h"
#include "OpenglStateMultTextureMaterialMapMultLights.h"
#include "OpenglStateModel3D.h"
#include "OpenglStateMultTextureDepthCube.h"
#include "OpenglStatePlane.h"
#include "OpenglStateMultTextureBlend.h"
#include "OpenglStateMultTextureTransparent.h"
#include "OpenglStateMultTextureFrameBuffer.h"
#include "OpenglStateMultTextureFrameBuffer.h"
#include "OpenglStateCubeMap.h"
#include "OpenglStateReflect.h"
#include "OpenglStateInStance.h"
#include "OpenglStateInstance2.h"
#include "OpenglStateModel3DPlanet.h"
#include "OpenglStateModel3DRock.h"
#include "OpenglStateMultTextureMultSample.h"
#include "OpenglStateMultTextureMaterialMapMultLightsAdvance.h"
#include "OpenglStateMultTextureMaterialMapPointLightAdvance.h"
#include "OpenglStateNormalMap.h"
#include "OpenglStateNormalMap1.h"
#include "OpenglStateModel3DNormalMap.h"
#include "OpenglStateHDR.h"
#include "OpenglStateHDRBloom.h"
#include "OpenglStateDelayRenderLights.h"
#include "OpenglStateDelayRenderLightsFrameBuffer.h"
#include "OpenglStateShadowDirLight.h"
#include "OpenglStateShadowPointLight.h"
#include "OpenglStateLabel.h"
#include "OpenglStateMultTextureWave.h"
#include "OpenglStateSSAO.h"
#include "OpenglStatePBRSimple.h"
#include "OpenglStatePBRTexture.h"
#include "OpenglStatePBR_IBL_Irradiance_Conversion.h"
#include "OpenglStatePBR_IBL_Irradiance.h"


#define random(a,b) (rand()%(b-a+1)+a)

#define MAX_SHADERCOUNT 46

#include "Model.h"
#include "OpenglWorld.h"

#include "practice/practice_2_1.h"


int width = 800;
int height = 600;

float mixValue = 0.2f;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

bool firstMouse = true;
float lastX = 400, lastY = 300; //记录鼠标的位置
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;

float fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

using namespace std;

GLFWwindow* window = nullptr;
OpenglCamera *camera = nullptr;
OpenglWorld  *world = nullptr;
OpenglState *glStateFrameBuffer = nullptr;
OpenglState *glStateDelayRenderLightsFrameBuffer = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.0001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.0001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

	float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		//cameraPos += cameraSpeed * cameraFront;
		camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		//cameraPos -= cameraSpeed * cameraFront;
		camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//计算当前帧和上一帧鼠标位置的偏移量
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;
	camera->ProcessMouseMovement(xoffset, yoffset);

	//float sensitivity = 0.05f;
	//xoffset *= sensitivity;
	//yoffset *= sensitivity;

	//yaw += xoffset;
	//pitch += yoffset;


	/*
	对于俯仰角，要让用户不能看向高于89度的地方（在90度时视角会发生逆转，所以我们把89度作为极限），
	同样也不允许小于-89度。这样能够保证用户只能看到天空或脚下，但是不能超越这个限制
	*/
	//if (pitch > 89.0f)
	//	pitch = 89.0f;
	//if (pitch < -89.0f)
	//	pitch = -89.0f;

	////通过俯仰角和偏航角来计算以得到真正的方向向量
	//glm::vec3 front;
	//front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	//front.y = sin(glm::radians(pitch));
	//front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	//cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//if (fov >= 1.0f && fov <= 45.0f)
	//	fov -= yoffset;
	//if (fov <= 1.0f)
	//	fov = 1.0f;
	//if (fov >= 45.0f)
	//	fov = 45.0f;

	camera->ProcessMouseScroll(yoffset);
}

int createWindow(GLFWwindow** pWindow)
{

	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, width, height); //必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)==>设置视口

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	*pWindow = window;
	return 0;
}

map<int, string> OpenglStatesMap;
void initShaders()
{
	int count = MAX_SHADERCOUNT;
	for (int i = 0; i <= count;i++)
	{
		string shaderName = "shader" + to_string(i);
		OpenglStatesMap.insert(pair<int, string >(i, shaderName));
	}
}


void createTestObjects()
{
	//------------------------------------------------------------------------------------------
	//底板
	OpenglState *glStatePlane = new OpenglStatePlane();
	int index = glStatePlane->getShaderIndex();
	string shaderName = OpenglStatesMap[index];
	string vertFile = "shader/" + shaderName + ".vert";
	string fragFile = "shader/" + shaderName + ".frag";
	glStatePlane->init(vertFile, fragFile);


	//OpenglStateMultTextureBlend 无光照模板
	//OpenglStateReflect 带光照模板
	OpenglState *glState = new OpenglStatePBR_IBL_Irradiance();
	index = glState->getShaderIndex();
	shaderName = OpenglStatesMap[index];
	//float s = i * random(1, 2);
	//float v = i* random(1, 2);
	//glState->setModelMat4(glm::vec3(s, v, s * v), glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0);
	vertFile = "shader/" + shaderName + ".vert";
	fragFile = "shader/" + shaderName + ".frag";
	//存储下摄像机
	glState->_camera = camera;
	glState->_window = window;
	glState->init(vertFile, fragFile);
	world->add(glState);

	//OpenglState *glState1 = new OpenglStateModel3DRock();
	//index = glState1->getShaderIndex();
	//shaderName = OpenglStatesMap[index];
	////float s = i * random(1, 2);
	////float v = i* random(1, 2);
	////glState->setModelMat4(glm::vec3(s, v, s * v), glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0);
	//vertFile = "shader/" + shaderName + ".vert";
	//fragFile = "shader/" + shaderName + ".frag";
	//glState1->init(vertFile, fragFile);
	//world->add(glState1);

	//OpenglState *glState1 = new OpenglStateMultTextureBlend();
	//index = glState1->getShaderIndex();
	//shaderName = OpenglStatesMap[index];
	////float s = i * random(1, 2);
	////float v = i* random(1, 2);
	////glState->setModelMat4(glm::vec3(s, v, s * v), glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0);
	//vertFile = "shader/" + shaderName + ".vert";
	//fragFile = "shader/" + shaderName + ".frag";
	//glState1->init(vertFile, fragFile);
	//world->add(glState1);


	// 使用混合渲染半透明物体
	//OpenglState *glState2 = new OpenglStateMultTextureTransparent();
	//index = glState2->getShaderIndex();
	//shaderName = OpenglStatesMap[index];
	//vertFile = "shader/" + shaderName + ".vert";
	//fragFile = "shader/" + shaderName + ".frag";
	//glState2->init(vertFile, fragFile);
	//world->add(glState2);


	//使用自定义帧缓冲对象绘制
	glStateFrameBuffer = new OpenglStateMultTextureFrameBuffer();
	index = glStateFrameBuffer->getShaderIndex();
	shaderName = OpenglStatesMap[index];
	vertFile = "shader/" + shaderName + ".vert";
	fragFile = "shader/" + shaderName + ".frag";
	glStateFrameBuffer->init(vertFile, fragFile);


	//延迟光照渲染的帧缓冲
	glStateDelayRenderLightsFrameBuffer = new OpenglStateDelayRenderLightsFrameBuffer();
	index = glStateDelayRenderLightsFrameBuffer->getShaderIndex();
	shaderName = OpenglStatesMap[index];
	vertFile = "shader/" + shaderName + ".vert";
	fragFile = "shader/" + shaderName + ".frag";
	glStateDelayRenderLightsFrameBuffer->init(vertFile, fragFile);



	//天空盒子
	OpenglState *glStateSkyBox = new OpenglStateCubeMap();
	index = glStateSkyBox->getShaderIndex();
	shaderName = OpenglStatesMap[index];
	vertFile = "shader/" + shaderName + ".vert";
	fragFile = "shader/" + shaderName + ".frag";
	glStateSkyBox->init(vertFile, fragFile);
	//world->add(glStateSkyBox);

	//------------------------------------------------------------------------------------------

	auto _openglStateArray = world->_openglStateArray;
	int size = _openglStateArray.size();
	bool isPlane = false;
	bool isUseSkyBox = false;
	for (int i = 0; i < size; i++)
	{
		OpenglState *glState = _openglStateArray[i];

		//是否渲染光源模型
		if (!world->_isLight)
		{
			world->setLight(glState->isShowLight());
			world->setLightNum(glState->getPointLights());
			world->setLightAction(glState->isLihgtAction());
			world->_isUseCustomLightPos = glState->isUseCustomLightPos();
			if (world->_isUseCustomLightPos)
			{
				world->_lightPositions = glState->_lightPositions;
				world->_lightColors = glState->_lightColors;
				world->_lightScale = glState->_lightScale;
			}
		}

		//是否渲染3d模型
		if (glState->isRenderModel())
		{
			world->addModelOpenglState(glState, glState->_ID);
		}

		//是否需要添加底板
		if (!isPlane && glState->isUsePlane())
		{
			isPlane = true;
		}

		//是否使用自定义帧缓冲对象
		if (!world->_isUseFrameBuffer)
		{
			world->setUseFameBuffer(glState->isUseFrameBuffer());
		}

		//是否使用天空盒子
		if (!isUseSkyBox && glState->isUseSkyBox())
		{
			isUseSkyBox = true;
			world->add(glStateSkyBox);
			world->setCubemapTexture(glStateSkyBox->_cubemapTexture);
		}

		//是否使用高动态范围
		if (!world->_isUseHDR)
		{
			world->setUseHDR(glState->isUseHDR());
			world->setExposure(glState->getExposure());
		}

		//是否使用bloom效果
		if (!world->_isUseBloom)
		{
			world->_isUseBloom = glState->isUseBloom();
		}

		//是否使用延迟光照渲染
		if (!world->_isDelayRenderLights)
		{
			world->_isDelayRenderLights = glState->isDelayRenderLights();
		}

		//是否使用多重采样
		if (!world->_isUseMSAA)
		{
			world->_isUseMSAA = glState->isUseMSAA();
		}

		//是否使用环境光遮罩
		if (!world->_isUseSSAO)
		{
			world->_isUseSSAO = glState->isUseSSAO();
		}

		//是否渲染默认帧缓冲
		if (!world->_isRenderFrameBuffer)
		{
			world->_isRenderFrameBuffer = glState->isRenderFrameBuffer();
		}

	}


	if (isPlane && !isUseSkyBox)
	{
		world->add(glStatePlane, true);
	}

	//
	if (world->_isRenderFrameBuffer)
	{
		if (!world->_isDelayRenderLights)
		{
			//正向渲染光照
			world->add(glStateFrameBuffer);

			//标记下是否使用hdr
			glStateFrameBuffer->_isUseHDR = world->_isUseHDR;
			glStateFrameBuffer->_exposure = world->_exposure;
			if (world->_isUseBloom)
			{
				glStateFrameBuffer->_isUseBloom = true;
			}
		}
		else
		{
			//延迟渲染光照
			world->add(glStateDelayRenderLightsFrameBuffer);
			//标记下是否使用hdr
			glStateDelayRenderLightsFrameBuffer->_isUseHDR = world->_isUseHDR;
			glStateDelayRenderLightsFrameBuffer->_exposure = world->_exposure;
			if (world->_isUseBloom)
			{
				glStateDelayRenderLightsFrameBuffer->_isUseBloom = true;
			}
		}
	}

}

int main(int argc, char* argv[])
{
	glfwInit();

	// 设置opegl版本号为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//同样明确告诉GLFW我们使用的是核心模式(Core-profile) 明确告诉GLFW我们需要使用核心模式意味着我们只能使用OpenGL功能的一个子集
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//glfwWindowHint(GLFW_SAMPLES, 4); //每个像素点使用4个采样点 ，为了多级采样用
	
	createWindow(&window);

	camera = new OpenglCamera();

	// ------------------------------------------------------------------


	//---------------------------------------------------------------------------------------------------
	// OpenglUtils * glUtils = OpenglUtils::getInstance();
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.0f, 0.5f, 0.0f
	//};

	//float vertices[] = {
	//	0.5f, 0.5f, 0.0f,   // 右上角
	//	0.5f, -0.5f, 0.0f,  // 右下角
	//	-0.5f, -0.5f, 0.0f, // 左下角
	//	-0.5f, 0.5f, 0.0f   // 左上角
	//};

	//unsigned int indices[] = { // 注意索引从0开始! 
	//	0, 1, 3, // 第一个三角形
	//	1, 2, 3  // 第二个三角形
	//};

	////1 顶点数据传入
	//unsigned int VBO,VAO,EBO;
	//glUtils->bindVBOAndVAO(&VBO, &VAO, vertices, sizeof(vertices), true, &EBO, indices, sizeof(indices));

	////2 顶点着色器
	//unsigned int vertexShader;
	////createShaderWithSource(GL_VERTEX_SHADER, &vertexShader, vertexShaderSource);
	//glUtils->createShaderWithFile(GL_VERTEX_SHADER, &vertexShader, "shader/base.vert");

	////3片段着色器
	//unsigned int fragmentShader;
	////createShaderWithSource(GL_FRAGMENT_SHADER, &fragmentShader, fragmentShaderSource);
	//glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &fragmentShader, "shader/base.frag");


	////4链接着色器，创建着色器程序并且激活
	//unsigned int shaderProgram;
	//glUtils->linkShader(&shaderProgram, vertexShader, fragmentShader);

	////5 链接顶点属性
	////第一个参数就是顶点的location值
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	//glEnableVertexAttribArray(0); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//---------------------------------------------------------------------------------------------------


	/*
	GLM库从0.9.9版本起，默认会将矩阵类型初始化为一个零矩阵（所有元素均为0），而不是单位矩阵（对角元素为1，其它元素为0）。
	如果你使用的是0.9.9或0.9.9以上的版本，你需要将所有的矩阵初始化改为 glm::mat4 mat = glm::mat4(1.0f)。
	如果你想与本教程的代码保持一致，请使用低于0.9.9版本的GLM，或者改用上述代码初始化所有的矩阵。
	*/

	initShaders();

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	srand((unsigned)time(NULL));

	world = new OpenglWorld();

	//创建测试对象
	createTestObjects();

	/*
	glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
	glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
	glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
	*/

	auto creaetModelMap = world->_creaetModelMap;
	map<int, OpenglState *>::iterator iter;
	for (iter = creaetModelMap.begin(); iter != creaetModelMap.end(); iter++)
	{
		int id = iter->first;
		OpenglState *state = iter->second;
		string file = state->getModelFile();
		Model *model = new Model(file.c_str(), state);

		//加入渲染map里
		world->addRenderModel(model, id);
	}

	//渲染光源模型
	if (world->_isLight)
	{
		if (world->_isUseHDR)
		{
			world->_vertFile = "shader/lamp1.vert";
			world->_fragFile = "shader/lamp1.frag";
		}
		world->init();
		
	}


	// 渲染循环
	glEnable(GL_DEPTH_TEST); // 开启深度测试

	if (world->_isUseFrameBuffer)
	{
		glfwWindowHint(GLFW_SAMPLES, 4); //每个像素点使用4个采样点 ，为了多级采样用
		glEnable(GL_MULTISAMPLE); //开启多级采样
	}

	/*

	渲染到多重采样帧缓冲对象的过程都是自动的。只要我们在帧缓冲绑定时绘制任何东西，光栅器就会负责所有的多重采样运算。
	我们最终会得到一个多重采样颜色缓冲以及/或深度和模板缓冲。因为多重采样缓冲有一点特别，我们不能直接将它们的缓冲图像用于其他运算，比如在着色器中对它们进行采样。

	一个多重采样的图像包含比普通图像更多的信息，我们所要做的是缩小或者还原(Resolve)图像。
	多重采样帧缓冲的还原通常是通过glBlitFramebuffer来完成，它能够将一个帧缓冲中的某个区域复制到另一个帧缓冲中，并且将多重采样缓冲还原。
	
	*/

	unsigned int framebuffer = 0; //使用多重采样的帧缓冲
	unsigned int texColorBuffer; //使用多重采样的纹理附件

	unsigned int intermediateFBO; //多重采样帧缓冲的还原的正常帧缓冲
	unsigned int screenTexture = 0;  //正常帧缓冲的纹理附件
	unsigned int screenBrightTexture = 0;  //屏幕明亮区域纹理

	//延迟光照计算保存的信息
	unsigned int postionTexture = 0;		//顶点位置信息
	unsigned int normalTexture = 0;			//法线贴图信息
	unsigned int albedoSpecTexture = 0;		//漫反射贴图信息和高光贴图信息

	bool isUseFrameBuffer = world->_isUseFrameBuffer;
	if (isUseFrameBuffer)
	{
		
		if (!world->_isDelayRenderLights)
		{
			if (world->_isUseMSAA)
			{
				world->createFrameBufferByMultSample(width, height, &framebuffer);
			}
			
			//多重采样的帧缓冲不能进行采样，需要创建一个临时的正常帧缓冲然后把数据复制到正常帧缓冲中
			if (world->_isUseBloom)
			{
				//使用bloom 需要多个纹理
				world->createFrameBufferByColorBuffers(width, height, &intermediateFBO, &screenTexture, &screenBrightTexture, GL_RGBA16F, GL_RGBA);
			}
			else if (world->_isUseHDR)
			{
				//使用HDR
				world->createFrameBuffer(width, height, &intermediateFBO, &screenTexture, GL_RGBA16F, GL_RGBA);
			}
			else if (world->_isUseSSAO)
			{
				//使用SSAO 环境光遮罩
				world->createFrameBufferBySSAO(width, height, &intermediateFBO, &postionTexture, &normalTexture, &albedoSpecTexture);
			}
			else
			{	
				//默认使用帧缓冲的默认格式
				world->createFrameBuffer(width, height, &intermediateFBO, &screenTexture);
			}
		}
		else
		{
			//使用延迟光照 不支持多重采样帧缓冲区
			world->createFrameBufferByDelayRenderLights(width, height, &intermediateFBO, &postionTexture, &normalTexture, &albedoSpecTexture);
		}
	}

	auto _openglStateArray = world->_openglStateArray;
	int size = _openglStateArray.size();
	auto renderModelMap = world->_renderModelMap;

	while (!glfwWindowShouldClose(window))
	{
		//记录上一帧所使用的时间
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 输入
		processInput(window);

		// ------------------------渲染指令---------------------------------------

		//绑定创建的帧缓冲对象，下面的绘制命令都会填充到帧缓冲里面
		if (isUseFrameBuffer)
		{
			if (!world->_isDelayRenderLights)
			{
				
				if (world->_isUseMSAA)
				{
					//非延迟光照默认用多重采样帧缓冲
					world->useFrameBuffer(framebuffer);
				}
				else
				{
					world->useFrameBuffer(intermediateFBO);
				}
			}
			else
			{
				//延迟光照用普通采样帧缓冲
				world->useFrameBuffer(intermediateFBO);
			}
			
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 清除颜色缓冲和深度缓冲


		//glUseProgram(shaderProgram); //激活着色器程序对象：已激活着色器程序的着色器将在我们发送渲染调用的时候被使用
		//glBindVertexArray(VAO);     // 使用VAO后就是每一次渲染的时候直接使用VAO存储好的属性指针

		//渲染大世界里
		if (world->_isLight)
		{
			world->_param1 = mixValue;
			world->_param2 = camera->Position; //更新观察矩阵
			world->_param3 = camera->Front; //更新观察矩阵
			world->_param4 = camera->Zoom;   //更新矩阵投影
			world->rendeCommand();
		}


		for (int i = 0; i < size; i++)
		{
			OpenglState *state = _openglStateArray[i];
			state->_param1 = mixValue;
			state->_param2 = camera->Position; //更新观察矩阵
			state->_param3 = camera->Front; //更新观察矩阵
			state->_param4 = camera->Zoom;   //更新矩阵投影
			state->_viewMat4 = camera->GetViewMatrix(state->_cameroPosOffset);  //观察矩阵
			if (state->isUseFrameBuffer())
			{
			
				state->_multFrambuffer = framebuffer; //创建的多重采样帧缓冲
				state->_framebuffer = intermediateFBO; //要拷贝到正常的帧缓冲
				state->_textureColorbuffer = screenTexture;
				state->_isUseDelayeRenderLight = world->_isDelayRenderLights;
			}

			//非天空盒的对象保存下
			if (state->isUseReflect())
			{
				state->_cubemapTexture = world->_cubemapTexture;
			}

			if (state->isUseBloom())
			{
				state->_brightTexture = screenBrightTexture;
			}
			state->rendeCommand();

			if (state->isRenderModel())
			{
				int id = state->_ID;
				int amount = state->_amount;
				Model *model = renderModelMap[id];
				if (amount > 1)
				{
					glm::mat4 *modelMatrices = state->modelMatrices;

					for (unsigned int i = 0; i < state->_amount; i++)
					{
						glm::mat4 modelMat4 = modelMatrices[i];
						state->setMat4(state->_shaderProgram, "model", &modelMat4);
						model->Draw();
					}

				}
				else
				{
					if (world->_isUseSSAO)
					{
						glm::mat4 *modelMatrices = state->modelMatrices;
						glm::mat4 modelMat4 = modelMatrices[0];
						state->setMat4(state->_shaderProgram, "model", &modelMat4);
					}
					model->Draw();
					if (world->_isUseSSAO)
					{
						state->_ssaoPosTexture = postionTexture;
						state->_ssaoNormalTexture = normalTexture;
						state->_ssaoAlbedoSpecTexture = albedoSpecTexture;
						state->afterModleRender();
					}
				}


			}


			if (!glStateFrameBuffer->_blurTexture && state->isUseBloom())
			{
				//把模糊过的亮度区域纹理保存到帧缓冲渲染里下
				glStateFrameBuffer->_blurTexture = state->_blurTexture;
			}

			if (!glStateDelayRenderLightsFrameBuffer->_delayeRenderLightPosTexture && state->isDelayRenderLights())
			{
				//延迟光照渲染
				glStateDelayRenderLightsFrameBuffer->_delayeRenderLightPosTexture = postionTexture;
				glStateDelayRenderLightsFrameBuffer->_delayeRenderLightNormalTexture = normalTexture;
				glStateDelayRenderLightsFrameBuffer->_delayeRenderLightAlbedoSpecTexture = albedoSpecTexture;
			}
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//6 解除绑定VAO和VBO：直接绑定为0也是解绑glBindVertexArray(0）
	for (int i = 0; i < size; i++)
	{
		OpenglState *state = _openglStateArray[i];
		state->deleteBuffers();
	}

	world->deleteBuffers(framebuffer);

	delete world;
	delete camera;

	glfwTerminate(); //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	return 0;
}


