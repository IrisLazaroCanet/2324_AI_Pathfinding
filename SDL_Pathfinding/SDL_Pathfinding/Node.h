#pragma once
#include "Vector2D.h"
#include <string>


//A l'hora de construir la instància de Graph, es recorreran totes les cel·les de Grid
//Si isValidCell() => afegir aquella cel·la com a Node al Graph
//El Node es guarda quina cel·la és

//Alhora, es defineixen les Connections del Graph

//En el moment d'executar l'algorisme, a l'inici es cridarà un "HeuristicCalculator" (Singleton) que, donada una cel·la, un objectiu,
//i una funció heurística (manhattan, chebyshev...), calcularà h(n).
//A continuació, es podrà assignar, mitjançant aquest valor, l'heurística del node del graf

//Quan s'hagi fet per tots els grafs, es procedirà a executar l'algorisme


class Node
{
private:
	Vector2D cell;
	//float heuristic;	//Assignat per HC singleton en escollir un algorisme


	std::string id;  // Add an ID for the node

public:
	Node() { }
	Node(Vector2D _cell) { cell = _cell; }
	~Node() { }

	//void SetHeuristic(float _heuristic) { heuristic = _heuristic; }
	//Sfloat GetHeuristic() { return heuristic; }

	// Add a method to get the ID of the node
	std::string GetId() const { return id; }


	Vector2D GetCell() { return cell; }
};