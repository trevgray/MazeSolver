#pragma once
#include <iostream>

class UserInterface {
private:
	int mazeIndex;
	int algorithmIndex;
	int mazeSize;
	bool gradualGenerationBool;
	bool skipMazeGen;
public:
	UserInterface() {
		mazeIndex = 0;
		algorithmIndex = 0;
		mazeSize = 1;
		gradualGenerationBool = false;
		skipMazeGen = false;
	}
	void runDisplay() {
		std::cout << "------ SDL Maze Generator & Solver By: Trevor Gray ------" << std::endl;
		bool inputLoop;

		inputLoop = true;
		while (inputLoop) {
			std::cout << "Select Maze size (Max of 50):" << std::endl;
			std::cin >> mazeSize;
			if (mazeSize > 0 && mazeSize <= 50) {
				inputLoop = false;
			}
		}

		inputLoop = true;
		while (inputLoop) {
			std::cout << "Select Maze Algorithm (0 = Prim's Algorithm, 1 = Kruskal's Algorithm):" << std::endl;
			std::cin >> mazeIndex;
			if (mazeIndex >= 0 && mazeIndex < 2) {
				inputLoop = false;
			}
		}

		inputLoop = true;
		while (inputLoop) {
			std::cout << "Select Solving Algorithm (0 = Depth First Search):" << std::endl;
			std::cin >> algorithmIndex;
			if (algorithmIndex >= 0 && algorithmIndex < 1) {
				inputLoop = false;
			}
		}

		inputLoop = true;
		while (inputLoop) {
			std::cout << "Do you want Gradual Generation? (0 == No, 1 == Yes):" << std::endl;
			std::cin >> gradualGenerationBool;
			if (gradualGenerationBool == 1) {
				std::cout << "Do you want to skip maze generation? (0 == No, 1 == Yes):" << std::endl;
				std::cin >> skipMazeGen;
				if (skipMazeGen == 0 || skipMazeGen == 1) {
					inputLoop = false;
				}
			}
			if (gradualGenerationBool == 0) {
				inputLoop = false;
			}
		}
	}
	int GetMazeSize() { return mazeSize; }
	int GetMazeIndex() { return mazeIndex; }
	int GetAlgorithmIndex() { return algorithmIndex; }
	bool GetGradualGenerationBool() { return gradualGenerationBool; }
	bool GetSkipMazeGenBool() { return skipMazeGen; }
};