#include "Graph.h"

int main() {
	Graph graph;

	//graph.readGraph("D:\\test_adjMatrix.txt");
	//graph.readGraph("D:\\test_weightedAdjMatrix.txt");
	//graph.readGraph("D:\\test_adjList.txt");
	//graph.readGraph("D:\\test_weightedAdjList.txt");
	//graph.readGraph("D:\\test_edgeList.txt");
	graph.readGraph("D:\\test_weightedEdgeList.txt");
	
	//graph.transformToAdjMatrix();
	graph.transformToAdjList();
	//graph.transformToListOfEdges();

	//graph.writeGraph("D:\\output_adjMatrix.txt");
	//graph.writeGraph("D:\\output_weightedAdjMatrix.txt");
	//graph.writeGraph("D:\\output_adjList.txt");
	graph.writeGraph("D:\\output_weightedAdjList.txt");
	//graph.writeGraph("D:\\output_edgeList.txt");
	//graph.writeGraph("D:\\output_weightedEdgeList.txt");
	return 0;
}