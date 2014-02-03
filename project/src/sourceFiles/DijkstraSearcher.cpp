/*
 * Class for searching shortest paths.
 *
 * Implements Dijkstra's algorithm.
 *
 * Author: kimpe
 */

#include "../headerFiles/DijkstraSearcher.h"

DijkstraSearcher::DijkstraSearcher(int *mapSize, int **mapData) :
	arraySize(mapSize), dataArray(mapData),	numberOfNodes(mapSize[0] * mapSize[1]) {
		distance = new int[numberOfNodes];
		path = new int[numberOfNodes];
}

void DijkstraSearcher::initializeSingleSource(int* startLocation) {
	for (int nodeNumber = 0; nodeNumber < numberOfNodes; nodeNumber++) {
		distance[nodeNumber] = infinity;
		path[nodeNumber] = null;
	}
	int startNode = locationToNode(startLocation);
	distance[startNode] = 0;
}

void DijkstraSearcher::heapInsertAll() {
	for (int row = 0; row < arraySize[0]; row++) {
		for (int col = 0; col < arraySize[1]; col++) {
			int *heapNode = new int[6];
			int location[] = {row, col};
			heapNode[0] = locationToNode(location);
			heapNode[1] = distance[ heapNode[0] ];
			nodeVector.push_back(heapNode);
		}
	}
	HeapComparator comparator;
	std::make_heap( nodeVector.begin(), nodeVector.end(), comparator );
}

void DijkstraSearcher::search() {
	while ( !nodeVector.empty() ) {
		int* heapMin = nodeVector.front();
		nodeVector.erase( nodeVector.begin() );

		int u = heapMin[0];
		int* location = nodeToLocation(u);
		int row = location[0];
		int col = location[1];

		std::cout << "Top node is " << u << "\nlocated at (" << row << ", "
				  << col << ") \nwith value " << distance[u] << ".\n" << std::endl;

		if (0 < row) dealWithNeighbor(u, row - 1, col);
		if (row < arraySize[0] - 1) dealWithNeighbor(u, row + 1, col);
		if (0 < col) dealWithNeighbor(u, row, col - 1);
		if (col < arraySize[1] - 1) dealWithNeighbor(u, row, col + 1);
	}
}

void DijkstraSearcher::dealWithNeighbor(int u, int row, int col) {
	int location[] = {row, col};
	int v = locationToNode(location);

	std::cout << "Dealing with neighbor " << v << "\nlocated at (" << row << ", "
			  << col << ") \nwith value " << distance[v] << ".\n" << std::endl;

	relax(u, v);
}

void DijkstraSearcher::relax(int u, int v) {
	int* location = nodeToLocation(v);
	int row = location[0];
	int col = location[1];
	int w = dataArray[row][col];
	if (w == 0) w = infinity;

	if (distance[v] > distance[u] + w) {
		std::cout << "Updating node " << v << ": \nvalue " << distance[v]
				  << "\nchanged to " << distance[u] + w << ".\n" << std::endl;
		distance[v] = distance[u] + w;
		path[v] = u;
	}

	int referenceNode[] = {v, distance[v]};
	std::vector<int*>::iterator iter = std::find(nodeVector.begin(), nodeVector.end(), referenceNode);
	int* heapNode = *iter;
	heapNode[1] = distance[v];
	HeapComparator comparator;
	std::make_heap( nodeVector.begin(), nodeVector.end(), comparator);
}

int DijkstraSearcher::locationToNode(int* location) {
	return location[0] * arraySize[1] + location[1];
}

int* DijkstraSearcher::nodeToLocation(int nodeId) {
	int *location = new int[2];
	location[0] = std::floor(nodeId / arraySize[1]);
	location[1] = nodeId % arraySize[1];
	return location;
}

