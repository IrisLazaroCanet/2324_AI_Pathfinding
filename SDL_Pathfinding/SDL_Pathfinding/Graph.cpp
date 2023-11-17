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

Graph::Graph(Grid* grid)
{
	//Used to store all cells (name & terrain value)
	std::vector<std::pair<std::string, int>> namedPositions;

	//Used for horizontal connections - one vector for each row
	std::vector<std::vector<std::pair<std::string, int>>> rowData;

	//Used for vertical connections - one vector for each col
	std::vector<std::vector<std::pair<std::string, int>>> colData;

	//Create vector of std::pairs -> node name & terrain int
	//Iterate vector of std::pairs to make row vectors
	//Iterate vector of std::pairs to make col vectors

	//Iterate all terrain to store all its positions, with names
	for (int i = 0; i < grid->getTerrain().size(); i++)
	{
		for (int j = 0; j < grid->getTerrain()[i].size(); j++)
		{
			if(grid->getTerrain()[i][j] != 0)
				namedPositions.push_back(std::make_pair(GenerateValidNodeName(6), grid->getTerrain()[i][j]));
			else
				namedPositions.push_back(std::make_pair("", grid->getTerrain()[i][j]));
		}
	}

	//Fill rows
	int rowNum = 0;
	for (int i = 0; i < grid->getNumCellY(); i++)
	{
		rowData.push_back(std::vector<std::pair<std::string, int>>());
	}

	for (int i = 0; i < namedPositions.size(); i++)
	{
		rowData[rowNum].push_back(namedPositions[i]);

		if (i % grid->getNumCellX() == 0 && i >= grid->getNumCellX())
		{
			rowNum++;
		}
	}

	//Fill cols
	int colNum = 0;
	for (int i = 0; i < grid->getNumCellX(); i++)
	{
		colData.push_back(std::vector<std::pair<std::string, int>>());
	}

	for (int i = 0; i < namedPositions.size(); i++)
	{
		colData[colNum].push_back(namedPositions[i]);
		colNum++;

		if (i % grid->getNumCellY() == 0)
		{
			colNum = 0;
		}
	}

	//Test: print data
	std::cout << "rowData" << std::endl;
	for (int i = 0; i < rowData.size(); i++)
	{
		for (int j = 0; j < rowData[i].size(); j++)
		{
			std::cout << rowData[i][j].second << " ";
		}

		std::cout << "\n";
	}


	/*
	for (int i = 0; i < grid.getTerrain().size(); i++)
	{
		std::vector<std::pair<std::string, int>> row;
		
		for (int j = 0; j < grid.getTerrain()[i].size(); j++)
		{
			//Fill row
			row.push_back(std::make_pair(GenerateValidNodeName(5), grid.getTerrain()[i][j]));
		}

		rowData.push_back(row);
	}
	*/


	//if (grid.isValidTerrainPosition(i, j));
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

std::string Graph::GenerateNodeName(int charAmount)
{

	std::string glyphs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string generatedName = "";

	for (int i = 0; i < charAmount; i++)
	{
		generatedName += glyphs[rand() % glyphs.size()];
	}

	return generatedName;
}

std::string Graph::GenerateValidNodeName(int charAmount)
{
	std::string generatedName;
	bool validName;

	do
	{
		generatedName = GenerateNodeName(10);

		if (unavailableNames.size() == 0)
			validName = true;

		for(std::string id : unavailableNames)
		{
			if (generatedName == id)
			{
				validName = false;
				break;
			}
			else
				validName = true;
		}

	} while (!validName);

	unavailableNames.push_back(generatedName);
	return generatedName;
}
