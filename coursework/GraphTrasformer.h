#pragma once
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"

class GraphTrasformer 
{
public:
	/* group: trasfers all data to destination form, simultaneously erases data from the given form*/

	static bool transferContent(AdjacencyMatrix * given, AdjacencyList * dest);
	static bool transferContent(AdjacencyMatrix * given, EdgeList * dest);
	static bool transferContent(AdjacencyList * given, AdjacencyMatrix * dest);
	static bool transferContent(AdjacencyList * given, EdgeList * dest);
	static bool transferContent(EdgeList * given, AdjacencyMatrix * dest);
	static bool transferContent(EdgeList * given, AdjacencyList * dest);
};

