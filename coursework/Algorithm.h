#pragma once
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"

class Algorithm {
public:
	static GraphContent* getSpaingTreePrima(const GraphContent * graph);		// O(v^2)
	static GraphContent* getSpaingTreeKruscal(EdgeList * graph);				// O(e*log(e))
	static GraphContent* getSpaingTreeBoruvka(const EdgeList * graph);		// O(e*log(v))
};

