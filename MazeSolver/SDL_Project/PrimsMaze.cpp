#include "PrimsMaze.h"
#include <iostream>
#include <cstdlib>

PrimsMaze::PrimsMaze() {
	mazeSize = 5;
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			nodeArray[x][y] = Node(x, y);
		}
	}
}

PrimsMaze::PrimsMaze(int n) {
	mazeSize = n; //generate the maze
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			nodeArray[x][y] = Node(x, y);
		}
	}
}

void PrimsMaze::Generate() {
	srand((unsigned int)time(NULL));
	bool frontierLoop = true;
	//mark random node
	int x = rand() % mazeSize;
	int y = rand() % mazeSize;
	MarkNode(nodeArray[x][y]);
	while (frontierLoop == true) {
		//get random frontier node
		int randomFrontierIndex = rand() % frontier.size();
		Node randomFrontier = frontier[randomFrontierIndex];
		frontier.erase(frontier.begin() + randomFrontierIndex); //erase it from the frontier array
		//set the node walls based on the random frontier
		SetNodeWalls(randomFrontier);
		//mark the node as done and add frontiers to all the valid neightbors
		MarkNode(nodeArray[randomFrontier.x][randomFrontier.y]);
		if (frontier.size() == 0) {
			frontierLoop = false; //exit if the frontier vector is empty
		}
	}
}

void PrimsMaze::MarkNode(Node markedNode) {
	//std::cout << "finished marked: " << markedNode.x << " " << markedNode.y << std::endl;
	nodeArray[markedNode.x][markedNode.y].nodeState.SetState(2); //mark the node as done
	if (markedNode.x - 1 != -1) { //if the location will be outside the maze in the -x direction
		addFrontier(nodeArray[markedNode.x - 1][markedNode.y]);
	}
	if (markedNode.x + 1 < mazeSize) { //if the location will be outside the maze in the +x direction
		addFrontier(nodeArray[markedNode.x + 1][markedNode.y]);
	}
	if (markedNode.y - 1 != -1) { //if the location will be outside the maze in the -y direction
		addFrontier(nodeArray[markedNode.x][markedNode.y - 1]);
	}
	if (markedNode.y + 1 < mazeSize) { //if the location will be outside the maze in the +y direction
		addFrontier(nodeArray[markedNode.x][markedNode.y + 1]);
	}
}

std::vector<Node> PrimsMaze::NeighborNodes(Node markedNode, int function) {
	std::vector<Node> neighbors; //declare a array of neighbors
	if (markedNode.x - 1 >= 0 && nodeArray[markedNode.x - 1][markedNode.y].nodeState.GetState() == function) { //check if the neigbor is vaild then get the neighbor to the -x direction
		neighbors.push_back(nodeArray[markedNode.x - 1][markedNode.y]);
	}
	if (markedNode.x + 1 < mazeSize && nodeArray[markedNode.x + 1][markedNode.y].nodeState.GetState() == function) { //check if the neigbor is vaild then get the neighbor to the +x direction
		neighbors.push_back(nodeArray[markedNode.x + 1][markedNode.y]);
	}
	if (markedNode.y - 1 >= 0 && nodeArray[markedNode.x][markedNode.y - 1].nodeState.GetState() == function) { //check if the neigbor is vaild then get the neighbor to the -y direction
		neighbors.push_back(nodeArray[markedNode.x][markedNode.y - 1]);
	}
	if (markedNode.y + 1 < mazeSize && nodeArray[markedNode.x][markedNode.y + 1].nodeState.GetState() == function) { //check if the neigbor is vaild then get the neighbor to the +y direction
		neighbors.push_back(nodeArray[markedNode.x][markedNode.y + 1]);
	}
	return neighbors;
}

void PrimsMaze::addFrontier(Node addedNode) {
	if (addedNode.x >= 0 && addedNode.y >= 0 && addedNode.y < mazeSize && addedNode.x < mazeSize && nodeArray[addedNode.x][addedNode.y].nodeState.GetState() == 0) { //check if the node is inside the maze
		nodeArray[addedNode.x][addedNode.y].nodeState.SetState(1); //set it to working
		frontier.push_back(nodeArray[addedNode.x][addedNode.y]); //add to frontier array
		//std::cout << "frontier: " << addedNode.x << " " << addedNode.y << std::endl;
	}
}

void PrimsMaze::SetNodeWalls(Node markedNode) {
	std::vector<Node> completeNeightbors = NeighborNodes(markedNode, 2); //get complete neighbors around the frontier
	if (completeNeightbors.size() > 0) { //if empty don't do anything - only applies to the first node
		int randomNeightborsIndex = rand() % completeNeightbors.size();
		Node randomNeightbor = completeNeightbors[randomNeightborsIndex]; //get random complete neighbor
		if (randomNeightbor.x < markedNode.x) { //if the complete neighbor is to the left of the node - remove the walls between the nodes
			nodeArray[markedNode.x][markedNode.y].leftWall = false;
			nodeArray[randomNeightbor.x][randomNeightbor.y].rightWall = false;
		}
		if (randomNeightbor.x > markedNode.x) { //if the complete neighbor is to the right of the node - remove the walls between the nodes
			nodeArray[markedNode.x][markedNode.y].rightWall = false;
			nodeArray[randomNeightbor.x][randomNeightbor.y].leftWall = false;
		}
		if (randomNeightbor.y < markedNode.y) { //if the complete neighbor is to the top of the node - remove the walls between the nodes
			nodeArray[markedNode.x][markedNode.y].topWall = false;
			nodeArray[randomNeightbor.x][randomNeightbor.y].bottomWall = false;
		}
		if (randomNeightbor.y > markedNode.y) { //if the complete neighbor is to the bottom of the node - remove the walls between the nodes
			nodeArray[markedNode.x][markedNode.y].bottomWall = false;
			nodeArray[randomNeightbor.x][randomNeightbor.y].topWall = false;
		}
	}
}