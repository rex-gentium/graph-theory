﻿#pragma once
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
	static bool checkEuler(const GraphContent * graph, bool & isCircleExists, int & tourStart);
	static vector<int> getEuleranTour(const GraphContent * graph);
	static vector<int> getEuleranTourFleri(const GraphContent * graph);
};

