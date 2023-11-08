#include "SceneAlgorithmComparison.h"

ScenePathfindingComparison::ScenePathfindingComparison()
{
	//TODO: Construct graph using maze  
	maze = new Grid("../res/maze.csv");
	graph = InitializeGraph(maze);
	
	srand((unsigned int)time(NULL));

	//Init problem instances
	for (int i = 0; i < NUM_INSTANCES; i++)
	{
		//Initialize start with a random cell
		/*
		Vector2D start_cell(-1, -1);
		while (!maze->isValidCell(start_cell))
			start_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
		*/
		Vector2D start_cell = graph->GetNodeFromId("A").second->GetCell();
		Node* startNode = new Node(start_cell, "A");

		//Initialize goal with a random cell (but at least 3 cells far from the agent)
		/*
		Vector2D goal_cell(-1, -1);
		while ((!maze->isValidCell(goal_cell)) || (Vector2D::Distance(goal_cell, start_cell) < 3))
			goal_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
		*/

		Vector2D goal_cell = graph->GetNodeFromId("G").second->GetCell();
		Node* goalNode = new Node(goal_cell, "G");

		problemInstances.push_back(new PathfindingProblem(graph, startNode, goalNode));

		//delete startNode;
		//delete goalNode;
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


		//TEST: Get node A connections from graph
		std::vector<Connection*> testConnections;
		testConnections = graph->GetConnections("A");
		for (int i = 0; i < testConnections.size(); i++)
		{
			std::cout << testConnections[i]->GetToNode() << ", " << testConnections[i]->GetCost() << std::endl;
		}

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
	//TODO: Construct graph connections using terrain
	//Use grid functions?

	//TEST: Hardcoded nodes & connections to test
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
