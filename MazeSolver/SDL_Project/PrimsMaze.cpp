#include "PrimsMaze.h"
PrimsMaze::PrimsMaze() {
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			nodeArray[x][y] = Node();
		}
	}
}

PrimsMaze::PrimsMaze(int n) {
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			nodeArray[x][y] = Node();
		}
	}
}

void PrimsMaze::Generate(int n) {
	
}
