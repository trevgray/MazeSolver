#include "PrimsMaze.h"
#include "Randomizer.h"
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
		std::vector<Node> neightbors = NeighborNodes(randomFrontier);
		int randomNeightborsIndex = r.rand(0, neightbors.size());
		Node randomNeightbor = neightbors[randomNeightborsIndex];
		//set the direction of passages beteen neighbor and frontier
		SetNodeWalls(randomFrontier.x, randomFrontier.y, randomNeightbor.x, randomNeightbor.y);
		SetNodeWalls(randomNeightbor.x, randomNeightbor.y, randomFrontier.x, randomFrontier.y);
		MarkNode(nodeArray[randomFrontier.x][randomFrontier.y]);
		if (frontier.size() == 0) {
			frontierLoop = false;
		}
	}
}

void PrimsMaze::MarkNode(Node markedNode) {
	markedNode.nodeState.SetState(2);
	addFrontier(nodeArray[markedNode.x - 1][markedNode.y]);
	addFrontier(nodeArray[markedNode.x + 1][markedNode.y]);
	addFrontier(nodeArray[markedNode.x][markedNode.y - 1]);
	addFrontier(nodeArray[markedNode.x][markedNode.y + 1]);
}

std::vector<Node> PrimsMaze::NeighborNodes(Node markedNode) {
	std::vector<Node> neighbors;
	if ((markedNode.x > 0) && (nodeArray[markedNode.y][markedNode.x - 1].nodeState.GetState() != 2)) {
		neighbors.push_back(nodeArray[markedNode.x - 1][markedNode.y]);
	}
	if (markedNode.x + 1 < mazeSize && nodeArray[markedNode.y][markedNode.x + 1].nodeState.GetState() != 2) {
		neighbors.push_back(nodeArray[markedNode.x + 1][markedNode.y]);
	}
	if (markedNode.y > 0 && nodeArray[markedNode.y - 1][markedNode.x].nodeState.GetState() != 2) {
		neighbors.push_back(nodeArray[markedNode.x][markedNode.y - 1]);
	}
	if (markedNode.y + 1 < mazeSize && nodeArray[markedNode.y + 1][markedNode.x].nodeState.GetState() != 2) {
		neighbors.push_back(nodeArray[markedNode.x][markedNode.y + 1]);
	}
	
	//if (neighbors.empty() == true) {
	//	return nullptr;
	//}
	return neighbors;
}

void PrimsMaze::addFrontier(Node addedNode) {
	if (addedNode.x >= 0 && addedNode.y >= 0 && addedNode.y < mazeSize && addedNode.x < mazeSize && nodeArray[addedNode.y][addedNode.x].nodeState.GetState() == 0) {
		nodeArray[addedNode.y][addedNode.x].nodeState.SetState(1);
		frontier.push_back(nodeArray[addedNode.y][addedNode.x]);
	}
}

void PrimsMaze::SetNodeWalls(int fx, int fy, int tx, int ty) {
	if (fx < tx) {
		nodeArray[fx][fy].rightWall = true;
	}
	if (fx > tx) {
		nodeArray[fx][fy].leftWall = true;
	}
	if (fy < ty) {
		nodeArray[fx][fy].bottomWall = true;
	}
	if (fy > ty) {
		nodeArray[fx][fy].topWall = true;
	}
}