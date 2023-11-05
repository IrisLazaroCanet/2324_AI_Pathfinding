#pragma once
#include "Vector2D.h"

//A l'hora de construir la inst�ncia de Graph, es recorreran totes les cel�les de Grid
//Si isValidCell() => afegir aquella cel�la com a Node al Graph
//El Node es guarda quina cel�la �s

//Alhora, es defineixen les Connections del Graph

//En el moment d'executar l'algorisme, a l'inici es cridar� un "HeuristicCalculator" (Singleton) que, donada una cel�la, un objectiu,
//i una funci� heur�stica (manhattan, chebyshev...), calcular� h(n).
//A continuaci�, es podr� assignar, mitjan�ant aquest valor, l'heur�stica del node del graf

//Quan s'hagi fet per tots els grafs, es procedir� a executar l'algorisme


class Node
{
private:
	Vector2D cell;
	//float heuristic;	//Assignat per HC singleton en escollir un algorisme


public:
	Node() { }
	Node(Vector2D _cell) { cell = _cell; }
	~Node() { }

	//void SetHeuristic(float _heuristic) { heuristic = _heuristic; }
	//Sfloat GetHeuristic() { return heuristic; }
	Vector2D GetCell() { return cell; }
};