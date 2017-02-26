#pragma once
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"

class Algorithm {
public:
	static AdjacencyMatrix* getSpaingTreePrima(const AdjacencyMatrix * graph);	// O(v^2)
	static AdjacencyList* getSpaingTreePrima(const AdjacencyList * graph);		// O(v^2)
	static EdgeList* getSpaingTreePrima(const EdgeList * graph);				// O(e)

	static EdgeList* getSpaingTreeKruscal(const GraphContent * graph);				// O(e*log(e))
};

