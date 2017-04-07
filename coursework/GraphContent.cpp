#include "GraphContent.h"

GraphContent::GraphContent() {}

GraphContent::~GraphContent() {}

bool GraphContent::isBridge(int from, int to) const
{
	DSU components = getUnityComponents(from, to);
	int withoutEdgeCount = components.getSetCount();
	components.unite(from, to);
	int withEdgeCount = components.getSetCount();
	return withEdgeCount != withoutEdgeCount;
}
