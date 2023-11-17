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
	//Create nodes
	for (int i = 0; i < grid->getTerrain().size(); i++)
	{
		for (int j = 0; j < grid->getTerrain()[i].size(); j++)
		{
			if (grid->getTerrain()[i][j] != 0)
			{
				std::string name = GenerateValidNodeName(10);
				namedPositions.push_back(std::make_pair(name, grid->getTerrain()[i][j]));
				nodes[name] = new Node(Vector2D(i, j), name);
			}
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
		if (i % grid->getNumCellX() == 0 && i >= grid->getNumCellX())
		{
			rowNum++;
		}

		rowData[rowNum].push_back(namedPositions[i]);
	}

	//Create connections between rows
	for (int i = 0; i < rowData.size(); i++)
	{
		for (int j = 0; j < rowData[i].size(); j++)
		{
			//Check connection with previous (left) node
			if (j % grid->getNumCellX() != 0)
			{
				if (rowData[i][j - 1].first != "")
				{
					//Create connection (cost always 1 for now)
					connections.push_back(new Connection(1, rowData[i][j - 1].first, rowData[i][j].first));
				}
			}

			//Check connection with next (right) node
			if (j % grid->getNumCellX() != grid->getNumCellX() - 1)
			{
				if (rowData[i][j + 1].first != "")
				{
					//Create connection (cost always 1 for now)
					connections.push_back(new Connection(1, rowData[i][j + 1].first, rowData[i][j].first));
				}
			}
		}
		std::cout << "\n";
		/*
		//Check connection with previous (left) node
		if (i % grid->getNumCellX() != 0)
		{
		}

		//Check connection with next (right) node
		if (i % grid->getNumCellX() != grid->getNumCellX()-1)
		{
		}
		*/
	}


	//Fill cols
	int colNum = 0;
	for (int i = 0; i < grid->getNumCellX(); i++)
	{
		colData.push_back(std::vector<std::pair<std::string, int>>());
	}

	for (int i = 0; i < namedPositions.size(); i++)
	{
		if (i % grid->getNumCellX() == 0 && i >= grid->getNumCellX())
		{
			colNum = 0;
		}

		colData[colNum].push_back(namedPositions[i]);
		colNum++;
	}

	//Create connections between cols
	for (int i = 0; i < colData.size(); i++)
	{
		for (int j = 0; j < colData[i].size(); j++)
		{
			//Check connection with previous (up) node
			if (j % grid->getNumCellY() != 0)
			{
				if (colData[i][j - 1].first != "")
				{
					//Create connection (cost always 1 for now)
					connections.push_back(new Connection(1, colData[i][j - 1].first, colData[i][j].first));
				}
			}

			//Check connection with next (down) node
			if (j % grid->getNumCellY() != grid->getNumCellY() - 1)
			{
				if (colData[i][j + 1].first != "")
				{
					//Create connection (cost always 1 for now)
					connections.push_back(new Connection(1, colData[i][j + 1].first, colData[i][j].first));
				}
			}
		}
	}

	//TEST
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

Vector2D Graph::CellToPix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x * CELL_SIZE + offset, cell.y * CELL_SIZE + offset);
}

std::pair<std::string, Node*> Graph::GetRandomNodeWithID()
{
	std::map <std::string, Node*>::iterator item = nodes.begin();
	std::advance(item, GetRandomIndex(nodes.size()));

	return std::make_pair(item->first, nodes[item->first]);
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

int Graph::GetRandomIndex(int size)
{
	return std::rand() % ((size - 1)+1 - 0) + 0;
}
