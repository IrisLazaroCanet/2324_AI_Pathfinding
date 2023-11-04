#pragma once
#include "Path.h"
#include "Graph.h"

#include <iostream>
#include <queue>

class PathfindingAlgorithm
{
public:
	PathfindingAlgorithm() { }
	~PathfindingAlgorithm() { }

	//TODO: Check if parameters are okay
	virtual Path* FindPath(Graph* graph, Node* origin, Node* goal) = 0;

	int GetExploredNodes() { return (int)visited.size(); }

private:
	//TODO: Check if queue is the appropiate data structure
	std::queue<Node*> frontier;

	//TODO: Check if vector is the approapiate data structure
	std::vector<Node*> visited;
};