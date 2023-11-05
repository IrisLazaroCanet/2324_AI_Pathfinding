#include "FrontierElement.h"

FrontierElement::FrontierElement(Node* _node, Node* _cameFrom, float _costSoFar, float _heuristic)
{
	node = _node;
	cameFrom = _cameFrom;
	costSoFar = _costSoFar;
	heuristic = _heuristic;
	priority = CalculatePriority();
}

FrontierElement::~FrontierElement()
{
	delete node;
	delete cameFrom;
}

float FrontierElement::CalculatePriority()
{
	float priority = costSoFar + heuristic;
	return priority;
}
