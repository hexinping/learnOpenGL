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
#include "BallObject.h"

SpriteRenderer  *Renderer;

// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const GLfloat PLAYER_VELOCITY(500.0f);

GameObject      *Player;


// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;

BallObject     *Ball;

Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{ 

}

Game::~Game()
{
	delete Renderer;
	delete Player;
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
	ResourceManager::LoadTexture("resource/background.jpg", "background");
	ResourceManager::LoadTexture("resource/awesomeface.png",  "face");
	ResourceManager::LoadTexture("resource/block.png", "block");
	ResourceManager::LoadTexture("resource/block_solid.png", "block_solid");
	ResourceManager::LoadTexture("resource/paddle.png", "paddle");
	// 加载关卡
	GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 1;
	this->LevelIndex = 0;


	initObjects();

}

void Game::initObjects()
{
	//初始化玩家挡板
	glm::vec2 playerPos = glm::vec2(
		this->Width / 2 - PLAYER_SIZE.x / 2,
		this->Height - PLAYER_SIZE.y
		);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	//初始化小球
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::GetTexture("face"));

}
void Game::Update(GLfloat dt)
{
	Ball->Move(dt, this->Width);
}


void Game::ProcessInput(GLFWwindow *window, GLfloat dt)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// 移动挡板

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
				
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
				
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Ball->Stuck = false;
		}	

	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		// 绘制背景
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);

		// 绘制关卡
		this->Levels[this->LevelIndex].Draw(*Renderer);

		//绘制挡板
		Player->Draw(*Renderer);

		//绘制小球
		Ball->Draw(*Renderer);
	}
}