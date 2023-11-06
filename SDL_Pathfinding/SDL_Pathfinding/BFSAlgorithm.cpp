#include "BFSAlgorithm.h"
Path* BFSAlgorithm::FindPath(Graph* graph, Node* origin, Node* goal)
{
    std::cout << "Calculate path using BFS algorithm" << std::endl;

    // Create a queue for BFS
    std::queue<Node*> queue;
    queue.push(origin);

    // Create a map to keep track of parents for backtracking
    std::map<Node*, Node*> parentMap;

    while (!queue.empty())
    {
        Node* current = queue.front();
        queue.pop();

        // If the goal node is reached, construct the path and return it
        if (current == goal)
        {
            Path* path = new Path;
            Node* currentNode = goal;

            // Backtrack from the goal to the origin using parentMap
            while (currentNode != nullptr)
            {
                path->points.insert(path->points.begin(), currentNode->GetCell());
                currentNode = parentMap[currentNode];
            }

            return path;
        }

        // Get the connections of the current node from the graph
        std::vector<Connection*> connections = graph->GetConnections(current->GetId());

        for (Connection* connection : connections)
        {
            std::pair<std::string, Node*> neighborInfo = graph->GetNodeFromId(connection->GetToNode());
            Node* neighbor = neighborInfo.second; // Extract the Node* from the pair

            // Check if the neighbor node has not been visited
            if (parentMap.find(neighbor) == parentMap.end())
            {
                queue.push(neighbor);
                parentMap[neighbor] = current; // Set the parent for backtracking
            }
        }
    }

    // If no path is found, return nullptr
    return nullptr;
}


