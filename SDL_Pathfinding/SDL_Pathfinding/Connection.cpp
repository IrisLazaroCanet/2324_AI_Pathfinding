#include "Connection.h"

Connection::Connection(float _cost, std::string _fromNodeId, std::string _toNodeId)
{
	cost = _cost;
	fromNode = _fromNode;
	toNode = _toNode;
}

Connection::~Connection()
{
	delete fromNode;
	delete toNode;
}
