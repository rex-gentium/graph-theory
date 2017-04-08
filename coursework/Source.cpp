#include "Graph.h"

int main() {
	Graph graph;

	graph.readGraph("D:\\test_graph.txt");
	
	Graph spaingTree = graph.getSpaingTreeBoruvka();
	spaingTree.transformToAdjMatrix();
	spaingTree.writeGraph("D:\\output_graph.txt");
	
	return 0;
}