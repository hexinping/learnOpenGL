/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

SpriteRenderer  *Renderer;

Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{ 

}

Game::~Game()
{

}

void Game::Init()
{
	string name = "sprite";
	// 加载着色器
	Shader shader = ResourceManager::LoadShader("shader/sprite.vert", "shader/sprite.frag", nullptr, name);
	// 配置着色器
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

	shader.Use();
	shader.SetInteger("image", 0);
	shader.SetMatrix4("projection", projection);
	// 设置专用于渲染的控制
	Renderer = new SpriteRenderer(shader);

	
	// 加载纹理
	ResourceManager::LoadTexture("resource/awesomeface.png" , "face");
}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLFWwindow *window, GLfloat deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Game::Render()
{
	Renderer->DrawSprite(ResourceManager::GetTexture("face"),
		glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}