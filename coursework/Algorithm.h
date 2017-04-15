#pragma once
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"

class Algorithm {
public:
	// остовные деревья
	static GraphContent* getSpaingTreePrima(const GraphContent * graph);		// O(v^2)
	static GraphContent* getSpaingTreeKruscal(EdgeList * graph);				// O(e*log(e))
	static GraphContent* getSpaingTreeBoruvka(const EdgeList * graph);		// O(e*log(v))
	static GraphContent* getSpaingTreeKruscal(const GraphContent * graph);		// O(v*log(v))
	static GraphContent* getSpaingTreeBoruvka(const GraphContent * graph);		// O(e*log(v))
	// эйлеровы циклы
	static bool checkEuler(const GraphContent * graph, bool & isCircleExists, int & tourStart); // O(v^2) - AdjMatrix, O(v*log(v)) - AdjList, max(v,e) - EdgeList 
	static vector<int> getEuleranTour(const GraphContent * graph);			// O(e)
	static vector<int> getEuleranTourFleri(const AdjacencyMatrix * graph);	// O(e*v^2)
	static vector<int> getEuleranTourFleri(const AdjacencyList * graph);	// O(e*v*log(v))
	static vector<int> getEuleranTourFleri(const EdgeList * graph);			// O(e^3)
private:
	static bool checkEulerDegrees(const GraphContent * graph, bool & isCircleExists, int & tourStart); // O(
};

