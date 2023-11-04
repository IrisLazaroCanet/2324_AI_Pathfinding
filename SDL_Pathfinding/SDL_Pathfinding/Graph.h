#pragma once
#include "Node.h"
#include "Connection.h"

#include <vector>

class Graph
{
public:
	Graph(std::vector<Node*> _nodes, std::vector<Connection*> _connections);
	~Graph();

	std::vector<Connection*> GetConnections (Node* fromNode);

private:
	std::vector<Node*> nodes;	//TODO: Revisar si realment fa falta
	std::vector<Connection*> connections;	
};