#pragma once
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"

class Algorithm {
public:
	// остовные деревья
	static GraphContent* getSpaingTreePrima(const GraphContent * graph);		// O(v^2)
	static GraphContent* getSpaingTreeKruscal(EdgeList * graph);				// O(e*log(e))
	static GraphContent* getSpaingTreeBoruvka(const EdgeList * graph);			// O(e*log(v))
	// эйлеровы циклы
	static bool checkEuler(const GraphContent * graph, bool & isCircleExists, int & tourStart); // O(v^2) - AdjMatrix, O(v*log(v)) - AdjList, max(v,e) - EdgeList 
	static vector<int> getEuleranTour(const GraphContent * graph);		// O(e)
	static vector<int> getEuleranTourFleri(EdgeList * graph);			// O(e^2)
	// паросочетания
	static bool checkBipart(const AdjacencyList * graph, char * marks);
	static vector<pair<int, int>> getMaximumMatchingBipart(const AdjacencyList * graph);
	// потоки
	static GraphContent* getFlowFordFulkerson(const GraphContent * graph, int source, int sink);
private:
	// эйлеровы циклы
	static bool checkEulerDegrees(const GraphContent * graph, bool & isCircleExists, int & tourStart); // O(
	static set<pair<int, int>>::iterator Algorithm::chooseNextEdgeFleri(EdgeList * graph, int from);
	static set<tuple<int, int, int>>::iterator Algorithm::chooseNextWeightedEdgeFleri(EdgeList * graph, int from);
	// пометки
	static const char marked = 1, unmarked = 0;
	static bool hasUnmarked(const char * marks, int count, char unmarked = 0);
	static int getUnmarked(const char * marks, int count, char unmarked = 0);
	// паросочетания
	static bool improveMatching(const AdjacencyList * graph, int from, int * matching);
};

