#pragma once
#include "PathfindingAlgorithm.h"

class DijkstraAlgorithm : public PathfindingAlgorithm
{
public:
	DijkstraAlgorithm() { }
	~DijkstraAlgorithm() { }

	Path* FindPath(Graph* graph, Node* origin, Node* goal) override;
};