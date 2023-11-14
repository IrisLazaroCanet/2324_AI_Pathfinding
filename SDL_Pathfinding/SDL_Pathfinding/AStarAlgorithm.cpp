#include "AStarAlgorithm.h"

Path* AStarAlgorithm::FindPath(Graph* graph, Node* origin, Node* goal)
{
    std::cout << "Calculate path using A star algorithm" << std::endl;

    // Create a priority queue for A*
    std::priority_queue<std::pair<float, Node*>, std::vector<std::pair<float, Node*>>, std::greater<std::pair<float, Node*>>> pq;

    // Create a map to keep track of costs and parents for backtracking
    std::map<Node*, float> costSoFar;
    std::map<Node*, Node*> parentMap;

    // Initialize cost to the origin as 0
    costSoFar[origin] = 0.0f;

    // Push the origin node with the estimated total cost (g + h) to the priority queue
    pq.push(std::make_pair(costSoFar[origin] + origin->GetHeuristic(), origin));

    while (!pq.empty())
    {
        Node* current = pq.top().second;
        pq.pop();

        // If the goal node is reached, construct the path and return it
        if (current->GetCell() == goal->GetCell())
        {
            Path* path = new Path;
            Node* currentNode = goal;

            // Backtrack from the goal to the origin using parentMap
            //TODO: Revisar ordre d'insert dins el path
            int insertCounter = 0;
            while (currentNode != nullptr)
            {
                //TODO: Path sols retorna un node (l'últim)
                path->points.insert(path->points.begin() + insertCounter, currentNode->GetCell());
                //path->points.push_back(currentNode->GetCell());
                currentNode = parentMap[currentNode];
                insertCounter++;
            }

            return path;
        }

        // Get the connections of the current node from the graph
        std::vector<Connection*> connections = graph->GetConnections(current->GetId());

        for (Connection* connection : connections)
        {
            Node* neighbor = graph->GetNodeFromId(connection->GetToNode()).second;

            float newCost = costSoFar[current] + connection->GetCost();

            // If the neighbor node has not been visited or a shorter path is found
            if (costSoFar.find(neighbor) == costSoFar.end() || newCost < costSoFar[neighbor])
            {
                costSoFar[neighbor] = newCost;
                parentMap[neighbor] = current;
                pq.push(std::make_pair(newCost + neighbor->GetHeuristic(), neighbor));
            }
        }
    }

    // If no path is found, return nullptr
    return nullptr;
}




/**/