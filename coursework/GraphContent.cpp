#include "GraphContent.h"

GraphContent::GraphContent() {}

GraphContent::~GraphContent() {}

bool GraphContent::isBridge(int from, int to)
{
	int weight = getWeight(from, to);
	removeEdge(from, to);
	DSU components = getUnityComponents();
	int withoutEdgeCount = components.getSetCount();
	components.unite(from, to);
	int withEdgeCount = components.getSetCount();
	if (isWeighted)
		addEdge(from, to, weight);
	else
		addEdge(from, to);
	return withEdgeCount != withoutEdgeCount;
}
