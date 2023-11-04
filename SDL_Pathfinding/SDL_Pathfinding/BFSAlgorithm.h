#pragma once
#include "PathfindingAlgorithm.h"

class BFSAlgorithm : public PathfindingAlgorithm
{
public:
	BFSAlgorithm() { }
	~BFSAlgorithm() { }

	Path* FindPath(Graph* graph, Node* origin, Node* goal) override;
};