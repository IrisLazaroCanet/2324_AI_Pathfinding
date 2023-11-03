#include "Connection.h"

Connection::Connection(float _cost, Node* _fromNode, Node* _toNode)
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
