/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

//#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "PowerUp.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // Game state
    GameState              State;	
    GLboolean              Keys[1024];
    GLuint                 Width, Height;
    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
	void ProcessInput(GLFWwindow *window, GLfloat dt);
    void Update(GLfloat dt);
    void Render();

	void initObjects();
	void DoCollisions();
	void ResetLevel();
	void ResetPlayer();

	Collision  CheckCollision(BallObject &one, GameObject &two); // AABB - AABB collision
	GLboolean CheckCollision(GameObject &one, GameObject &two);

	Direction VectorDirection(glm::vec2 target);

	std::vector<GameLevel> Levels;
	GLuint                 Level;


	std::vector<PowerUp>  PowerUps;
	void SpawnPowerUps(GameObject &block);
	void UpdatePowerUps(GLfloat dt);
	GLboolean ShouldSpawn(GLuint chance);
	void ActivatePowerUp(PowerUp &powerUp);
	GLboolean isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);


};

#endif