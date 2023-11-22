#include "GreedyAlgorithm.h"

Path* GreedyAlgorithm::FindPath(Graph* graph, Node* origin, Node* goal)
{
    std::cout << "Calculate path using Greedy algorithm" << std::endl;

    // Create a priority queue for GBFS
    std::priority_queue<std::pair<float, Node*>, std::vector<std::pair<float, Node*>>, std::greater<std::pair<float, Node*>>> pq;

    // Create a map to keep track of parents for backtracking
    std::map<Node*, Node*> parentMap;
    parentMap[origin] = nullptr;

    // Push the origin node with the heuristic distance to the goal to the priority queue
    pq.push(std::make_pair(origin->GetHeuristic(), origin));

    while (!pq.empty())
    {
        Node* current = pq.top().second;
        pq.pop();

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
            Node* neighbor = graph->GetNodeFromId(connection->GetToNode()).second;

            // If the neighbor node has not been visited
            if (parentMap.find(neighbor) == parentMap.end())
            {
                parentMap[neighbor] = current;
                pq.push(std::make_pair(neighbor->GetHeuristic(), neighbor));
            }
        }
    }

    // If no path is found, return nullptr
    return nullptr;
}
