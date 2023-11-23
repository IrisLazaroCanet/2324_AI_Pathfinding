#include "DijkstraAlgorithm.h"

Path* DijkstraAlgorithm::FindPath(Graph* graph, Node* origin, Node* goal)
{
    std::cout << "Calculate path using Dijkstra algorithm" << std::endl;

    // Create a priority queue for Dijkstra
    std::priority_queue<std::pair<float, Node*>, std::vector<std::pair<float, Node*>>, std::greater<std::pair<float, Node*>>> pq;

    // Initialize distance to all nodes as infinity (except for the origin)
    std::map<Node*, float> distance;
    distance[origin] = 0.0f;

    // Create a map to keep track of parents for backtracking
    std::map<Node*, Node*> parentMap;

    // Push the origin node with distance 0 to the priority queue
    pq.push(std::make_pair(0.0f, origin));

    while (!pq.empty())
    {
        Node* current = pq.top().second;
        float currentDistance = pq.top().first;
        pq.pop();

        visited[current->GetId()] = true;

        // If the goal node is reached, construct the path and return it
        if (current == goal)
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

            // Calculate the new distance to the neighbor through the current node
            float newDistance = currentDistance + connection->GetCost();

            // If the neighbor node has not been visited or a shorter path is found
            if (!visited[neighbor->GetId()] && (distance.find(neighbor) == distance.end() || newDistance < distance[neighbor]))
            {
                distance[neighbor] = newDistance;
                parentMap[neighbor] = current;
                pq.push(std::make_pair(newDistance, neighbor));
            }
        }
    }

    // If no path is found, return nullptr
    return nullptr;
}



