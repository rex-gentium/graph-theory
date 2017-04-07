#include "Graph.h"

int main() {
	Graph graph;

	graph.readGraph("C:\\dump\\test_adjList.txt");
	
	vector<int> tour = graph.getEuleranTourFleri();
	
	//graph.writeGraph("D:\\output_graph.txt");
	
	return 0;
}