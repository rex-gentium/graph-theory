#include "Graph.h"

int main() {
	Graph graph;

	graph.readGraph("D:\\test_graph.txt");
	//graph.transformToListOfEdges();
	Graph spaingTree = graph.getSpaingTreePrima();
	spaingTree.writeGraph("D:\\output_graph.txt");
	spaingTree = graph.getSpaingTreeKruscal();
	spaingTree.writeGraph("D:\\output_graph.txt");
	spaingTree = graph.getSpaingTreeBoruvka();

	spaingTree.writeGraph("D:\\output_graph.txt");
	
	return 0;
}