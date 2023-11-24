#include "PathfindingProblem.h"

PathfindingProblem::PathfindingProblem(Graph* _graph, Node* _start, Node* _goal)
{
	graph = _graph;
	start = _start;
	goal = _goal;
}

PathfindingProblem::~PathfindingProblem()
{
	delete graph;
	delete start;
	delete goal;
}

void PathfindingProblem::SolveProblem()
{	
	//A*
	PC->SetAlgorithmToAStar();
	PC->FindPath(graph, start, goal);
	exploredNodes[ASTAR_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();

	//BFS
	PC->SetAlgorithmToBFS();
	PC->FindPath(graph, start, goal);
	exploredNodes[BFS_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();


	//Dijkstra
	PC->SetAlgorithmToDijkstra();
	PC->FindPath(graph, start, goal);
	exploredNodes[DIJKSTRA_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();
 
	//Greedy
	PC->SetAlgorithmToGreedy();
	PC->FindPath(graph, start, goal);
	exploredNodes[GREEDY_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();

}
