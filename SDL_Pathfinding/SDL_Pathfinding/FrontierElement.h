#pragma once
#include "Node.h"

class FrontierElement
{
public:
	FrontierElement(Node* _node, Node* _cameFrom, float _costSoFar, float _heuristic);
	~FrontierElement();

	Node* GetCameFrom() { return cameFrom; }
	float GetCostSoFar() { return costSoFar; }
	float GetHeuristic() { return heuristic; }
	float GetPriority() { return priority; }

private:
	float CalculatePriority();

	Node* node;
	Node* cameFrom;
	float costSoFar;
	float heuristic;
	float priority;
};