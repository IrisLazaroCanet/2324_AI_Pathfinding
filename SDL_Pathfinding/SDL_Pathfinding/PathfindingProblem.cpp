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
	//No cal utilitzar agents perqu� el seguiment del cam� no �s important
	//el que importa aqu� �s el c�lcul del cam�, l'algorisme en si

	//TODO: Revisar comunicaci� entre PC, graf del problema i nodes del graf
	//TODO: Escriure (b�) al document:
		//Utilitzar dist�ncia de Chebyshev perqu� tant el cost horitzontal-vertical com el diagonal valen el mateix
		//�s a dir, que �s com una dist�ncia diagonal (que est� indicada per quan l'agent es pot moure en totes direccions
		//en una graella) per� on tot moviment t� el mateix cost
		//No vol�em diferenciar horitzontal-diagonal de vertical -> "discritizaci�" de l'espai en cel�les, totes elles valen
		//el mateix -> el que importa �s el moviment entre cel�les, no la dist�ncia "real" en l'espai
	HC->SetFunctionToChebyshev();

	//for each(Node* node in graph)
		//node->SetHeuristic(HC->CalculateHeuristic);

	
	//A*
	PC->SetAlgorithmToAStar();
	Path* testPath = PC->FindPath(graph, start, goal);
	exploredNodes[ASTAR_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();
	std::cout << "Explored nodes: " << exploredNodes[ASTAR_KEY] << "\n" << std::endl;

	//BFS
	PC->SetAlgorithmToBFS();
	PC->FindPath(graph, start, goal);
	exploredNodes[BFS_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();
	std::cout << "Explored nodes: " << exploredNodes[BFS_KEY] << "\n" << std::endl;

	//Dijkstra
	PC->SetAlgorithmToDijkstra();
	PC->FindPath(graph, start, goal);
	exploredNodes[DIJKSTRA_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();
	std::cout << "Explored nodes: " << exploredNodes[DIJKSTRA_KEY] << "\n" << std::endl;

	//Greedy
	PC->SetAlgorithmToGreedy();
	PC->FindPath(graph, start, goal);
	exploredNodes[GREEDY_KEY] = PC->GetCurrentAlgorithm()->GetExploredNodes();
	std::cout << "Explored nodes: " << exploredNodes[GREEDY_KEY] << "\n" << std::endl;
}
