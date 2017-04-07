#pragma once
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"

class Algorithm {
public:
	// остовные деревья
	static GraphContent* getSpaingTreePrima(const GraphContent * graph);		// O(v^2)
	static GraphContent* getSpaingTreeKruscal(const GraphContent * graph);		// O(v*log(v))
	static GraphContent* getSpaingTreeBoruvka(const GraphContent * graph);		// O(e*log(v))
	// эйлеровы циклы
	static bool checkEuler(const GraphContent * graph, bool & isCircleExists, int & tourStart); // O(v^2) - AdjMatrix, O(v*log(v)) - AdjList, max(v,e) - EdgeList 
	static vector<int> getEuleranTour(const AdjacencyMatrix * graph);		// O(
	static vector<int> getEuleranTour(const AdjacencyList * graph);
	static vector<int> getEuleranTour(const EdgeList * graph);
	static vector<int> getEuleranTourFleri(const AdjacencyMatrix * graph);	// O(e*v^2)
	static vector<int> getEuleranTourFleri(const AdjacencyList * graph);	// O(e*v*log(v))
	static vector<int> getEuleranTourFleri(const EdgeList * graph);			// O(e
private:
	static bool checkEulerDegrees(const GraphContent * graph, bool & isCircleExists, int & tourStart); // O(
};

