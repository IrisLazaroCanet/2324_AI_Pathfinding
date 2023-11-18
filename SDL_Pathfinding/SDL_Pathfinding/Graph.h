#pragma once
#include "Node.h"
#include "Connection.h"
#include "Grid.h"

#include <string>
#include <vector>
#include <map>

class Graph
{
public:
	Graph() { }
	Graph(std::map<std::string, Node*> _nodes, std::map<std::pair<std::string, std::string>, int> _connectionsInfo);
	Graph(Grid* grid);
	~Graph();

	std::vector<Connection*> GetConnections (std::string fromNodeId);
	std::pair<std::string, Node*> GetNodeFromId(std::string id);
	Vector2D CellToPix(Vector2D cell);
	std::pair<std::string, Node*> GetRandomNodeWithID();
	std::string GetRandomNodeID();

private:
	std::vector<Connection*> InitializeConnections(std::map<std::pair<std::string, std::string>, int> connectionsInfo);
	std::string GenerateNodeName(int charAmount);
	std::string GenerateValidNodeName(int charAmount);
	int GetRandomIndex(int size);

	std::vector<std::string> unavailableNames;

	std::map<std::string, Node*> nodes;
	std::vector<Connection*> connections;	
};