#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "Vector2D.h"
#include "PathCalculator.h"
#include "Scene.h"
#include "Agent.h"
#include "PathFollowing.h"
#include "Grid.h"

class ScenePathfindingAlgorithmsWithEnemies : public Scene
{
public:
	ScenePathfindingAlgorithmsWithEnemies();
	~ScenePathfindingAlgorithmsWithEnemies();
	void update(float dtime, SDL_Event* event);
	void draw();
	const char* getTitle();

private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;

	Grid* maze;
	bool draw_grid;

	Graph* graph;
	std::string agentPositionID;
	std::string coinPositionID;

	void drawMaze();
	void drawCoin();
	SDL_Texture* background_texture;
	SDL_Texture* coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);
	
	void movementEnemies();
	int agent1PathIndex;
	int agent2PathIndex;
};