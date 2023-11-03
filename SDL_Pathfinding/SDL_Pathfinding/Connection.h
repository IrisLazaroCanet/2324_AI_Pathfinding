#pragma once
#include "Node.h"

//Totes les connexions seran bidireccionals

class Connection
{
private:
	Node* fromNode;
	Node* toNode;
	float cost;

public:
	Connection(float _cost, Node* _fromNode, Node* _toNode);
	~Connection();

	float GetCost() { return cost; }
	Node* GetFromNode() { return fromNode; }
	Node* GetToNode() { return toNode; }
};