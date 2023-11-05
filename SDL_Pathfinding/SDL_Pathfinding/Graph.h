#pragma once
#include "Node.h"
#include "Connection.h"

#include <string>
#include <vector>
#include <map>

class Graph
{
public:
	Graph() { }
	Graph(std::map<std::string, Node*> _nodes, std::map<std::pair<std::string, std::string>, int> _connectionsInfo);
	~Graph();

	std::vector<Connection*> GetConnections (std::string fromNodeId);
	std::pair<std::string, Node*> GetNodeFromId(std::string id);

private:
	std::vector<Connection*> InitializeConnections(std::map<std::pair<std::string, std::string>, int> connectionsInfo);

	std::map<std::string, Node*> nodes;
	std::vector<Connection*> connections;	
};