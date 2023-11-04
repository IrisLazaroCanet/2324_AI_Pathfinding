#pragma once
#include "PathfindingAlgorithm.h"

class AStarAlgorithm : public PathfindingAlgorithm
{
public:
	AStarAlgorithm() { }
	~AStarAlgorithm() { }

	Path* FindPath(Graph* graph, Node* origin, Node* goal) override;
};