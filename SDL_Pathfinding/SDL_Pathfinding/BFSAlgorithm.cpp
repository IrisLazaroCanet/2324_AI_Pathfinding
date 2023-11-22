#include "BFSAlgorithm.h"
Path* BFSAlgorithm::FindPath(Graph* graph, Node* origin, Node* goal)
{
    std::cout << "Calculate path using BFS algorithm" << std::endl;

    // Create a queue for BFS
    std::queue<Node*> queue;
    queue.push(origin);

    // Create a map to keep track of parents for backtracking
    std::map<Node*, Node*> parentMap;
    parentMap[origin] = nullptr;

    while (!queue.empty())
    {
        Node* current = queue.front();
        queue.pop();

        visited[current->GetId()] = true;

        // If the goal node is reached, construct the path and return it
        if (current->GetCell() == goal->GetCell())
        {
            Path* path = new Path;
            Node* currentNode = current;

            // Backtrack from the goal to the origin using parentMap
            while (currentNode != nullptr)
            {
                path->points.insert(path->points.begin(), currentNode->GetCell());
                currentNode = parentMap[currentNode];
            }

            std::cout << "Explored nodes: " << visited.size() << std::endl;

            return path;
        }

        // Get the connections of the current node from the graph
        std::vector<Connection*> connections = graph->GetConnections(current->GetId());

        for (Connection* connection : connections)
        {
            std::pair<std::string, Node*> neighborInfo = graph->GetNodeFromId(connection->GetToNode());
            //Node* neighbor = neighborInfo.second; // Extract the Node* from the pair
            std::string neighborId = neighborInfo.first;  //Extract the node's ID from the pair

            // Check if the neighbor node has not been visited
            bool foundNeighbor = false;
            for (std::map<Node*, Node*>::iterator it = parentMap.begin(); it != parentMap.end(); it++)
            {
                if (it->first->GetId() == neighborId)
                    foundNeighbor = true;
            }

            if (!foundNeighbor)
            {
                queue.push(neighborInfo.second);
                parentMap[neighborInfo.second] = current; // Set the parent for backtracking
                //std::cout << "Neighbor info: " << neighborInfo.first << ", " << current->GetId() << std::endl;
            }

            /*
            if (parentMap.find(neighbor) == parentMap.end())
            {
                queue.push(neighbor);
                parentMap[neighbor] = current; // Set the parent for backtracking
            }
            */
        }
    }

    // If no path is found, return nullptr
    return nullptr;
}


