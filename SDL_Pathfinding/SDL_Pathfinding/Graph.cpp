#include "Graph.h"

///<summary>
///_nodes: key is the id of the node ("A", "B"...); value is the actual node |
///_connectionsInfo: key is a pair of the ids of the connected nodes (e.g. "A", "B"), value is the weight of the connection
///</summary>
Graph::Graph(std::map<std::string, Node*> _nodes, std::map<std::pair<std::string, std::string>, int> _connectionsInfo)
{
	nodes = _nodes;
	connections = InitializeConnections(_connectionsInfo);
}

Graph::Graph(Grid grid)
{
	//Used for horizontal connections
	std::vector<std::pair<std::string, int>> rowData;

	//Used for vertical connections
	std::vector<std::pair<std::string, int>> colData;
}

Graph::~Graph()
{
	for (std::map<std::string, Node*>::iterator it = nodes.begin(); it != nodes.end(); it++)
		delete it->second;

	for (int i = 0; i < (int)connections.size(); i++)
		delete connections[i];
}

std::vector<Connection*> Graph::GetConnections(std::string fromNodeId)
{
	std::vector<Connection*> returnedConnections;

	for (int i = 0; i < (int)connections.size(); i++)
	{
		if (connections[i]->GetFromNode() == fromNodeId)
			returnedConnections.push_back(connections[i]);
	}

	return returnedConnections;
}

///<summary>
///id: the name of the node ("A", "B"...)
///</summary>
std::pair<std::string, Node*> Graph::GetNodeFromId(std::string id)
{
	return std::pair<std::string, Node*>(id, nodes[id]);
}

std::vector<Connection*> Graph::InitializeConnections(std::map<std::pair<std::string, std::string>, int> connectionsInfo)
{
	std::vector<Connection*> returnedConnections;

	for (std::map<std::pair<std::string, std::string>, int>::iterator it = connectionsInfo.begin(); it != connectionsInfo.end(); it++)
	{
		returnedConnections.push_back(new Connection(
			it->second,			//(float) the weight of the connection
			it->first.first,	//(string) the id of the fromNode
			it->first.second	//(string) the id of the toNode
		));
	}

	return returnedConnections;
}
