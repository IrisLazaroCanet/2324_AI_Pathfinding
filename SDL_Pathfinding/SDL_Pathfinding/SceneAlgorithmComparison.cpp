#include "SceneAlgorithmComparison.h"

ScenePathfindingComparison::ScenePathfindingComparison()
{
	//TODO: Construct graph using maze  
	maze = new Grid("../res/maze.csv");
	//graph = new Graph(...);
	graph = new Graph();
	
	srand((unsigned int)time(NULL));

	//Init problem instances
	for (int i = 0; i < NUM_INSTANCES; i++)
	{
		//Initialize start with a random cell
		Vector2D start_cell(-1, -1);
		while (!maze->isValidCell(start_cell))
			start_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

		Node* startNode = new Node(start_cell);

		//Initialize goal with a random cell (but at least 3 cells far from the agent)
		Vector2D goal_cell(-1, -1);
		while ((!maze->isValidCell(goal_cell)) || (Vector2D::Distance(goal_cell, start_cell) < 3))
			goal_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

		Node* goalNode = new Node(goal_cell);

		problemInstances.push_back(new PathfindingProblem(graph, startNode, goalNode));

		delete startNode;
		delete goalNode;
	}

	finishedComputing = false;
}

ScenePathfindingComparison::~ScenePathfindingComparison()
{
	for (int i = 0; i < (int)problemInstances.size(); i++)
	{
		delete problemInstances[i];
	}
}

void ScenePathfindingComparison::update(float dtime, SDL_Event* event)
{
	SolveProblems();
	SaveResultsToFile();
}

void ScenePathfindingComparison::draw()
{
	//(No need to draw anything)
}

const char* ScenePathfindingComparison::getTitle()
{
	return "SDL Path Finding :: PathFinding Algorithm Comparison (no render)";
}

void ScenePathfindingComparison::SolveProblems()
{
	if (!finishedComputing)
	{
		std::cout << "START COMPUTING\n" << std::endl;
		for (int i = 0; i < (int)problemInstances.size(); i++)
		{
			std::cout << "Solve instance #" << i << std::endl;
			problemInstances[i]->SolveProblem();
			std::cout << "\n" << std::endl;
		}

		finishedComputing = true;
		std::cout << "FINISHED COMPUTING" << std::endl;
	}
}

void ScenePathfindingComparison::SaveResultsToFile()
{
	//TODO: Save data in file (call a FileSerializer singleton)
}
