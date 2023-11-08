#include "PathfindingProblem.h"

PathfindingProblem::PathfindingProblem(Graph* _graph, Node* _start, Node* _goal)
{
	graph = _graph;
	start = _start;
	goal = _goal;
}

PathfindingProblem::~PathfindingProblem()
{
	delete graph;
	delete start;
	delete goal;
}

void PathfindingProblem::SolveProblem()
{
	//No cal utilitzar agents perquè el seguiment del camí no és important
	//el que importa aquí és el càlcul del camí, l'algorisme en si

	//TODO: Revisar comunicació entre PC, graf del problema i nodes del graf
	//TODO: Escriure (bé) al document:
		//Utilitzar distància de Chebyshev perquè tant el cost horitzontal-vertical com el diagonal valen el mateix
		//És a dir, que és com una distància diagonal (que està indicada per quan l'agent es pot moure en totes direccions
		//en una graella) però on tot moviment té el mateix cost
		//No volíem diferenciar horitzontal-diagonal de vertical -> "discritizació" de l'espai en cel·les, totes elles valen
		//el mateix -> el que importa és el moviment entre cel·les, no la distància "real" en l'espai
	HC->SetFunctionToChebyshev();

	//for each(Node* node in graph)
		//node->SetHeuristic(HC->CalculateHeuristic);

	
	//A*
	PC->SetAlgorithmToAStar();
	PC->FindPath(graph, start, goal);
	exploredNodes[ASTAR_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();
	std::cout << exploredNodes[ASTAR_KEY] << std::endl;

	//BFS
	PC->SetAlgorithmToBFS();
	PC->FindPath(graph, start, goal);
	exploredNodes[BFS_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();
	std::cout << exploredNodes[BFS_KEY] << std::endl;

	//Dijkstra
	PC->SetAlgorithmToDijkstra();
	PC->FindPath(graph, start, goal);
	exploredNodes[DIJKSTRA_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();
	std::cout << exploredNodes[DIJKSTRA_KEY] << std::endl;

	//Greedy
	PC->SetAlgorithmToGreedy();
	PC->FindPath(graph, start, goal);
	exploredNodes[GREEDY_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();
	std::cout << exploredNodes[GREEDY_KEY] << std::endl;
}
