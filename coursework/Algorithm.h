#pragma once
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"

class Algorithm {
public:
	static GraphContent* getSpaingTreePrima(const GraphContent * graph);		// O(v^2)
	static GraphContent* getSpaingTreeKruscal(const GraphContent * graph);		// O(v*log(v))
	static GraphContent* getSpaingTreeBoruvka(const GraphContent * graph);		// O(e*log(v))
};

