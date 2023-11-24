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

class SceneTSP : public Scene
{
public:
	SceneTSP();
	~SceneTSP();
	void update(float dtime, SDL_Event* event);
	void draw();
	const char* getTitle();

private:
	std::vector<Agent*> agents;
	std::vector<Vector2D> coinPositions;
	const float NUM_LOCATIONS = 5;

	Grid* maze;
	bool draw_grid;
	std::map<std::string, bool> activeCoins;

	Graph* graph;
	std::string agentPositionID;
	std::vector<std::string> coinPositionIDs;
	std::string closestNodeID;

	Path* path;


	void drawMaze();
	void drawCoins();
	SDL_Texture* background_texture;
	SDL_Texture* coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);

	std::string FindClosestNodeID(std::string originID);
};