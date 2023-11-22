#include "SceneAlgorithmComparison.h"

ScenePathfindingComparison::ScenePathfindingComparison()
{
	maze = new Grid("../res/maze.csv");
	graph = new Graph(maze);
	
	srand((unsigned int)time(NULL));

	//Init problem instances
	for (int i = 0; i < NUM_INSTANCES; i++)
	{
		std::string startCellID = graph->GetRandomNodeID();
		Vector2D start_cell = graph->GetNodeFromId(startCellID).second->GetCell();
		Node* startNode = new Node(start_cell, startCellID);

		std::string goalCellID = graph->GetRandomNodeID();
		Vector2D goal_cell = graph->GetNodeFromId(goalCellID).second->GetCell();
		Node* goalNode = new Node(goal_cell, goalCellID);

		problemInstances.push_back(new PathfindingProblem(graph, startNode, goalNode));
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
	for (int i = 0; i < (int)problemInstances.size(); i++)
	{
		FS->WriteMapToBinaryFile("problem_results.txt", problemInstances[i]->GetExploredNodes());
	}
}

Graph* ScenePathfindingComparison::InitializeGraph(Grid* grid)
{
	Node* S = new Node(Vector2D(0.5, 0.f), "S");
	Node* A = new Node(Vector2D(0.f, 1.f), "A");
	Node* B = new Node(Vector2D(1.f, 1.f), "B");
	Node* C = new Node(Vector2D(0.f, 2.f), "C");
	Node* D = new Node(Vector2D(1.f, 2.f), "D");
	Node* G = new Node(Vector2D(0.5f, 3.f), "G");
	std::map<std::string, Node*> nodes;
	nodes["S"] = S;
	nodes["A"] = A;
	nodes["B"] = B;
	nodes["C"] = C;
	nodes["D"] = D;
	nodes["G"] = G;

	std::map<std::pair<std::string, std::string>, int> _connectionsInfo;
	_connectionsInfo[std::make_pair("S", "A")] = 3;
	_connectionsInfo[std::make_pair("S", "B")] = 5;
	_connectionsInfo[std::make_pair("A", "D")] = 2;
	_connectionsInfo[std::make_pair("B", "D")] = 6;
	_connectionsInfo[std::make_pair("B", "G")] = 5;
	_connectionsInfo[std::make_pair("D", "C")] = 9;
	_connectionsInfo[std::make_pair("D", "G")] = 3;

	Graph* graph = new Graph(nodes, _connectionsInfo);

	return graph;
}
