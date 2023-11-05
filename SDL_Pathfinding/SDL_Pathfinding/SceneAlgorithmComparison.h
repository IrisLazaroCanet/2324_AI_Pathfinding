#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "Scene.h"
#include "Vector2D.h"
#include "Grid.h"
#include "Graph.h"
#include "PathfindingProblem.h"
#include "FileSerializer.h"

class ScenePathfindingComparison : public Scene
{
public:
	ScenePathfindingComparison();
	~ScenePathfindingComparison();
	void update(float dtime, SDL_Event* event);
	void draw();
	const char* getTitle();

	void SolveProblems();
	void SaveResultsToFile();

private:
	Graph* InitializeGraph(Grid* grid);
	
	Grid* maze;
	Graph* graph;
	std::vector<PathfindingProblem*> problemInstances;

	bool finishedComputing;
	const int NUM_INSTANCES = 20;
};