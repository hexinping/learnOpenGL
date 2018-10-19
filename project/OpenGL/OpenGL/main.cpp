
#include "main.h"
#include <iostream>
#include <map>
#include "OpenglUtils.h"
#include "OpenglState.h"
#include "OpenglStateTriangle.h"
#include "OpenglStateRect.h"
#include "OpenglStateMultAttr.h"
#include "OpenglStateTriangleMove.h"
#include "OpenglStateTexture.h"
#include "OpenglStateMultTexture.h"

#include "practice/practice_2_1.h"

float mixValue = 0.2f;

using namespace std;


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

	OpenglState *glState = new OpenglStateMultTexture();
	int index = glState->getShaderIndex();
	string shaderName = OpenglStatesMap[index];
	string vertFile = "shader/" + shaderName + ".vert";
	string fragFile = "shader/" + shaderName + ".frag";
	glState->init(vertFile, fragFile);
	unsigned int VBO, VAO, EBO;
	VBO = glState->_VBO;
	VAO = glState->_VAO;
	EBO = glState->_EBO;



	/*
	glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ⱦѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
	glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
	glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
	*/
	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window);

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
		glState->rendeCommand();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//6 �����VAO��VBO��ֱ�Ӱ�Ϊ0Ҳ�ǽ��glBindVertexArray(0��
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate(); //����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	return 0;
}


