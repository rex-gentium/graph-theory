#include "Graph.h"

int main() {
	Graph graph;

	//graph.readGraph("C:\\dump\\test_adjMatrix.txt");
	//graph.readGraph("C:\\test_weightedAdjMatrix.txt");
	//graph.readGraph("C:\\dump\\test_adjList.txt");
	//graph.readGraph("C:\\test_weightedAdjList.txt");
	graph.readGraph("C:\\dump\\test_edgeList.txt");
	//graph.readGraph("C:\\dump\\test_weightedEdgeList.txt");
	
	graph.addEdge(3, 4, 1);
	graph.removeEdge(3, 7);
	graph.removeEdge(3, 8);
	graph.removeEdge(2, 5);
	graph.addEdge(6, 4, 8);
	graph.changeEdge(5, 5, 8);
	graph.addEdge(3, 3, 7);
	graph.removeEdge(8, 3);
	graph.removeEdge(1, 1);
	graph.addEdge(7, 1, 4);
	graph.changeEdge(4, 8, 3);
	graph.changeEdge(7, 1, 2);
	graph.addEdge(8, 5, 9);
	graph.addEdge(1, 3, 1);
	graph.changeEdge(4, 3, 2);

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