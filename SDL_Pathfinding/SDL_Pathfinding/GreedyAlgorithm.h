#pragma once
#include "PathfindingAlgorithm.h"

class GreedyAlgorithm : public PathfindingAlgorithm
{
public:
	GreedyAlgorithm() { }
	~GreedyAlgorithm() { }

	Path* FindPath(Graph* graph, Node* origin, Node* goal) override;
};