#pragma once
#include "Graph.h"
#include "PathCalculator.h"
#include "HeuristicCalculator.h"
#include <string>
#include <map>

class PathfindingProblem
{
public:
	PathfindingProblem(Graph* _graph, Node* _start, Node* _goal);
	~PathfindingProblem();

	void SolveProblem();
	std::map<std::string, int> GetExploredNodes() { return exploredNodes; }

private:
	Graph* graph;
	Node* start;
	Node* goal;

	const std::string BFS_KEY = "BFS";
	const std::string DIJKSTRA_KEY = "Dijkstra";
	const std::string GREEDY_KEY = "Greedy";
	const std::string ASTAR_KEY = "A*";

	//Per cada algorisme (string), quants nodes s'han explorat
	std::map<std::string, int> exploredNodes;
};