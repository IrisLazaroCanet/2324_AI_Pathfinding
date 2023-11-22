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

protected:
	//TODO: Check if vector is the appropiate data structure
	std::vector<Node*> frontier;

	//TODO: Check if vector is the approapiate data structure
	//std::vector<Node*> visited;
	std::map<std::string, bool> visited;
};