#pragma once
#include "Node.h"
#include <string>
#include <map>

//Totes les connexions seran bidireccionals

class Connection
{
private:
	std::string fromNodeId;
	std::string toNodeId;
	float cost;

public:
	Connection(float _cost, std::string _fromNodeId, std::string _toNodeId);
	~Connection();

	float GetCost() { return cost; }
	std::string GetFromNode() { return fromNodeId; }
	std::string GetToNode() { return toNodeId; }
};