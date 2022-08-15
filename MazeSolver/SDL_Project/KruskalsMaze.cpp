#include "KruskalsMaze.h"

KruskalsMaze::KruskalsMaze() {
	mazeSize = 5;
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			nodeArray[x][y] = Node(x, y);
		}
	}
}

KruskalsMaze::KruskalsMaze(int n) {
	mazeSize = n; //generate the maze
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			nodeArray[x][y] = Node(x, y);
		}
	}
}

void KruskalsMaze::Generate()
{
}
