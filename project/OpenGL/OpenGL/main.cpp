
#include "main.h"
#include <iostream>
#include <map>
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

#include "practice/practice_2_1.h"

float mixValue = 0.2f;

float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

bool firstMouse = true;
float lastX = 400, lastY = 300; //��¼����λ��
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;

float fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

using namespace std;

OpenglCamera *camera = nullptr;


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
	//���㵱ǰ֡����һ֡���λ�õ�ƫ����
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
	lastX = xpos;
	lastY = ypos;
	camera->ProcessMouseMovement(xoffset, yoffset);

	//float sensitivity = 0.05f;
	//xoffset *= sensitivity;
	//yoffset *= sensitivity;

	//yaw += xoffset;
	//pitch += yoffset;


	/*
		���ڸ����ǣ�Ҫ���û����ܿ������89�ȵĵط�����90��ʱ�ӽǻᷢ����ת���������ǰ�89����Ϊ���ޣ���
		ͬ��Ҳ������С��-89�ȡ������ܹ���֤�û�ֻ�ܿ�����ջ���£����ǲ��ܳ�Խ�������
	*/
	//if (pitch > 89.0f)
	//	pitch = 89.0f;
	//if (pitch < -89.0f)
	//	pitch = -89.0f;

	////ͨ�������Ǻ�ƫ�����������Եõ������ķ�������
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
	int width = 800;
	int heght = 600;

	GLFWwindow* window = glfwCreateWindow(width, heght, "LearnOpenGL", NULL, NULL);
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

	glViewport(0, 0, width, heght); //�������OpenGL��Ⱦ���ڵĳߴ��С�����ӿ�(Viewport)==>�����ӿ�

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	*pWindow = window;
	return 0;
}

map<int, string> OpenglStatesMap;
void intShaders()
{
	OpenglStatesMap[0] = "base";
	OpenglStatesMap[1] = "shader1";
	OpenglStatesMap[2] = "shader2";
	OpenglStatesMap[3] = "shader3";
	OpenglStatesMap[4] = "shader4";
	OpenglStatesMap[5] = "shader5";
	OpenglStatesMap[6] = "shader6";
	OpenglStatesMap[7] = "shader7";
	OpenglStatesMap[8] = "shader8";
	OpenglStatesMap[9] = "shader9";
	OpenglStatesMap[10] = "shader10";
}

int main(int argc, char* argv[])
{
	glfwInit();

	// ����opegl�汾��Ϊ3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//ͬ����ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile) ��ȷ����GLFW������Ҫʹ�ú���ģʽ��ζ������ֻ��ʹ��OpenGL���ܵ�һ���Ӽ�
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window;
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
	//	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	//	0.5f, -0.5f, 0.0f,  // ���½�
	//	-0.5f, -0.5f, 0.0f, // ���½�
	//	-0.5f, 0.5f, 0.0f   // ���Ͻ�
	//};

	//unsigned int indices[] = { // ע��������0��ʼ! 
	//	0, 1, 3, // ��һ��������
	//	1, 2, 3  // �ڶ���������
	//};

	////1 �������ݴ���
	//unsigned int VBO,VAO,EBO;
	//glUtils->bindVBOAndVAO(&VBO, &VAO, vertices, sizeof(vertices), true, &EBO, indices, sizeof(indices));

	////2 ������ɫ��
	//unsigned int vertexShader;
	////createShaderWithSource(GL_VERTEX_SHADER, &vertexShader, vertexShaderSource);
	//glUtils->createShaderWithFile(GL_VERTEX_SHADER, &vertexShader, "shader/base.vert");

	////3Ƭ����ɫ��
	//unsigned int fragmentShader;
	////createShaderWithSource(GL_FRAGMENT_SHADER, &fragmentShader, fragmentShaderSource);
	//glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &fragmentShader, "shader/base.frag");


	////4������ɫ����������ɫ�������Ҽ���
	//unsigned int shaderProgram;
	//glUtils->linkShader(&shaderProgram, vertexShader, fragmentShader);

	////5 ���Ӷ�������
	////��һ���������Ƕ����locationֵ
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	//glEnableVertexAttribArray(0); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	//---------------------------------------------------------------------------------------------------


	/*
		GLM���0.9.9�汾��Ĭ�ϻὫ�������ͳ�ʼ��Ϊһ�����������Ԫ�ؾ�Ϊ0���������ǵ�λ���󣨶Խ�Ԫ��Ϊ1������Ԫ��Ϊ0����
		�����ʹ�õ���0.9.9��0.9.9���ϵİ汾������Ҫ�����еľ����ʼ����Ϊ glm::mat4 mat = glm::mat4(1.0f)��
		��������뱾�̵̳Ĵ��뱣��һ�£���ʹ�õ���0.9.9�汾��GLM�����߸������������ʼ�����еľ���
	*/

	intShaders();

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	OpenglState *glState = new OpenglStateMultTextureLight();
	int index = glState->getShaderIndex();
	string shaderName = OpenglStatesMap[index];
	string vertFile = "shader/" + shaderName + ".vert";
	string fragFile = "shader/" + shaderName + ".frag";
	glState->init(vertFile, fragFile);
	unsigned int VBO, VAO, EBO;
	VBO = glState->_VBO;
	VAO = glState->_VAO;
	EBO = glState->_EBO;

	unsigned int lightVBO = 0, lighgtVAO = 0, lightEBO = 0;
	if (glState->_isLight)
	{
		lightVBO = glState->_lightVBO;
		lighgtVAO = glState->_lightVAO;
		lightEBO = glState->_lightEBO;
	}



	/*
	glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ⱦѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
	glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
	glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
	*/
	
	

	// ��Ⱦѭ��
	glEnable(GL_DEPTH_TEST); // ������Ȳ���
	while (!glfwWindowShouldClose(window))
	{
		//��¼��һ֡��ʹ�õ�ʱ��
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// ����
		processInput(window);

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// �����ɫ�������Ȼ���

		//glUseProgram(shaderProgram); //������ɫ����������Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ��
		//glBindVertexArray(VAO);     // ʹ��VAO�����ÿһ����Ⱦ��ʱ��ֱ��ʹ��VAO�洢�õ�����ָ��

		//if (isUseEBO)
		//{
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //ʹ����������
		//}
		//else
		//{
		//	glDrawArrays(GL_TRIANGLES, 0, 3);
		//}
		glState->_param1 = mixValue;
		glState->_param2 = camera->Position; //���¹۲����
		glState->_param3 = camera->Front; //���¹۲����
		glState->_param4 = camera->Zoom;   //���¾���ͶӰ
		glState->rendeCommand();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//6 �����VAO��VBO��ֱ�Ӱ�Ϊ0Ҳ�ǽ��glBindVertexArray(0��
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteBuffers(1, &lightEBO);
	glDeleteVertexArrays(1, &lighgtVAO);
	glDeleteBuffers(1, &lightVBO);

	glfwTerminate(); //����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	return 0;
}


