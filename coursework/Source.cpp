#include "Graph.h"

int main() {
	Graph graph;

	//graph.readGraph("C:\\dump\\test_adjMatrix.txt");
	//graph.readGraph("C:\\test_weightedAdjMatrix.txt");
	//graph.readGraph("C:\\dump\\test_adjList.txt");
	//graph.readGraph("C:\\test_weightedAdjList.txt");
	graph.readGraph("C:\\dump\\test_edgeList.txt");
	//graph.readGraph("C:\\test_weightedEdgeList.txt");
	
	graph.addEdge(1, 3, 0);

	//graph.transformToAdjMatrix();
	//graph.transformToAdjList();
	//graph.transformToListOfEdges();

	//graph.writeGraph("C:\\dump\\output_adjMatrix.txt");
	//graph.writeGraph("D:\\output_weightedAdjMatrix.txt");
	//graph.writeGraph("C:\\dump\\output_adjList.txt");
	//graph.writeGraph("D:\\output_weightedAdjList.txt");
	graph.writeGraph("C:\\dump\\output_edgeList.txt");
	//graph.writeGraph("D:\\output_weightedEdgeList.txt");
	return 0;
}