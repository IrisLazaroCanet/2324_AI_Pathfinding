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

class ScenePathfindingAlgorithms : public Scene
{
public:
	ScenePathfindingAlgorithms();
	~ScenePathfindingAlgorithms();
	void update(float dtime, SDL_Event* event);
	void draw();
	const char* getTitle();

private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;

	Grid* maze;
	bool draw_grid;

	Graph* graph;
	std::pair<std::string, Node*> agentPositionNodeWithID;
	std::pair<std::string, Node*> coinPositionNodeWithID;
	
	void drawMaze();
	void drawCoin();
	SDL_Texture* background_texture;
	SDL_Texture* coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);
};