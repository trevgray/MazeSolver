#include "PrimsMaze.h"
#include "Randomizer.h"
#include <iostream>
PrimsMaze::PrimsMaze() {
	mazeSize = 5;
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			nodeArray[x][y] = Node(x, y);
		}
	}
}

PrimsMaze::PrimsMaze(int n) {
	mazeSize = n;
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			nodeArray[x][y] = Node(x, y);
		}
	}
}

void PrimsMaze::Generate(int n) {
	bool frontierLoop = true;
	MATH::Randomizer r;
	//mark random node
	int x = r.rand(0, n);
	int y = r.rand(0, n);
	MarkNode(nodeArray[x][y]);
	while (frontierLoop == true) { //we need a frontier list
		//get random frontier node
		int randomFrontierIndex = r.rand(0, frontier.size());
		Node randomFrontier = frontier[randomFrontierIndex];
		frontier.erase(frontier.begin() + randomFrontierIndex);
		//get random neightbors of frontier node
		std::vector<Node> incompleteNeightbors = NeighborNodes(randomFrontier, true);
		int randomNeightborsIndex = r.rand(0, incompleteNeightbors.size());
		Node randomNeightbor = incompleteNeightbors[randomNeightborsIndex];
		//
		//SetNodeWalls(randomNeightbor);
		MarkNode(nodeArray[randomFrontier.x][randomFrontier.y]);
		if (frontier.size() == 0) {
			frontierLoop = false;
		}
	}
}

void PrimsMaze::MarkNode(Node markedNode) {
	std::cout << "finished: " << markedNode.x << " " << markedNode.y << std::endl;
	nodeArray[markedNode.x][markedNode.y].nodeState.SetState(2);
	addFrontier(nodeArray[markedNode.x - 1][markedNode.y]);
	addFrontier(nodeArray[markedNode.x + 1][markedNode.y]);
	addFrontier(nodeArray[markedNode.x][markedNode.y - 1]);
	addFrontier(nodeArray[markedNode.x][markedNode.y + 1]);
}

std::vector<Node> PrimsMaze::NeighborNodes(Node markedNode, bool function) {
	std::vector<Node> neighbors;
	if (function == true) { //if function = true - find any incomplete or working neighbors
		if (markedNode.x - 1 >= 0 && nodeArray[markedNode.x - 1][markedNode.y].nodeState.GetState() == 0) {
			neighbors.push_back(nodeArray[markedNode.x - 1][markedNode.y]);
		}
		if (markedNode.x + 1 < mazeSize && nodeArray[markedNode.x + 1][markedNode.y].nodeState.GetState() == 0) {
			neighbors.push_back(nodeArray[markedNode.x + 1][markedNode.y]);
		}
		if (markedNode.y - 1 >= 0 && nodeArray[markedNode.x][markedNode.y - 1].nodeState.GetState() == 0) {
			neighbors.push_back(nodeArray[markedNode.x][markedNode.y - 1]);
		}
		if (markedNode.y + 1 < mazeSize && nodeArray[markedNode.x][markedNode.y + 1].nodeState.GetState() == 0) {
			neighbors.push_back(nodeArray[markedNode.x][markedNode.y + 1]);
		}
	}
	else { //if function = false - find only complete neighbors
		if (markedNode.x - 1 >= 0 && (nodeArray[markedNode.x - 1][markedNode.y].nodeState.GetState() == 2)) {
			neighbors.push_back(nodeArray[markedNode.x - 1][markedNode.y]);
		}
		if (markedNode.x + 1 < mazeSize && nodeArray[markedNode.x + 1][markedNode.y].nodeState.GetState() == 2) {
			neighbors.push_back(nodeArray[markedNode.x + 1][markedNode.y]);
		}
		if (markedNode.y - 1 >= 0 && nodeArray[markedNode.x][markedNode.y - 1].nodeState.GetState() == 2) {
			neighbors.push_back(nodeArray[markedNode.x][markedNode.y - 1]);
		}
		if (markedNode.y + 1 < mazeSize && nodeArray[markedNode.x][markedNode.y + 1].nodeState.GetState() == 2) {
			neighbors.push_back(nodeArray[markedNode.x][markedNode.y + 1]);
		}
	}
	return neighbors;
}

void PrimsMaze::addFrontier(Node addedNode) {
	std::cout << "frontier: " << addedNode.x << " " << addedNode.y << std::endl;
	if (addedNode.x >= 0 && addedNode.y >= 0 && addedNode.y < mazeSize && addedNode.x < mazeSize && nodeArray[addedNode.x][addedNode.y].nodeState.GetState() == 0) {
		nodeArray[addedNode.x][addedNode.y].nodeState.SetState(1);
		frontier.push_back(nodeArray[addedNode.x][addedNode.y]);
		//std::cout << "frontier: " << addedNode.x << " " << addedNode.y << std::endl;
	}
}

void PrimsMaze::SetNodeWalls(Node markedNode) {
	MATH::Randomizer r;
	std::vector<Node> completeNeightbors = NeighborNodes(markedNode, false);
	int randomNeightborsIndex = r.rand(0, completeNeightbors.size());
	Node randomNeightbor = completeNeightbors[randomNeightborsIndex];
	if (randomNeightbor.x > markedNode.x) {
		markedNode.leftWall = false;
		randomNeightbor.rightWall = false;
	}
}