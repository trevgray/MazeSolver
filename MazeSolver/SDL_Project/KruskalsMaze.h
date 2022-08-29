#pragma once
#include "Maze.h"
#include <vector>

class KruskalsMaze : public Maze {
private:
	std::vector<int> uniqueRandomNums;

	std::vector<std::vector<Node>> nodeSets;
	int lastNodeSetIndex;
public:
	KruskalsMaze();
	KruskalsMaze(int n);
	void Generate();
	void MarkNode(Node markedNode);
	//void RemoveNodeFromSet(Node node);
	void AddNodeToSet(Node nodeToAdd, Node nodeWithSet);
	void RemoveWall(Node markedNode, Node randomDifferentSetNeightbor);
	std::vector<Node> NodeSetCheck(std::vector<Node> unCheckedNodes, Node markedNode);
	std::vector<Node> GetNodeSet(Node node);
	std::vector<Node> NeighborNodes(Node markedNode, int function);
};