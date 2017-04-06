#include "Graph.h"

int main() {
	Graph graph;

	graph.readGraph("D:\\test_graph.txt");
	
	vector<int> tour = graph.getEuleranTourFleri();
	
	graph.writeGraph("D:\\output_graph.txt");
	
	return 0;
}