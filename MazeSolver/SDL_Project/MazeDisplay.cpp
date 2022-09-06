#include "MazeDisplay.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Randomizer.h"
#include "VMath.h"
using namespace MATH;

MazeDisplay::MazeDisplay(SDL_Window* sdlWindow_, UserInterface& interfaceRef) : wallTexture(nullptr), maze(nullptr), algorithm(nullptr), traversedTexture(nullptr) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	interface = interfaceRef;
}

MazeDisplay::~MazeDisplay() {

}

void MazeDisplay::SetUpMazeAndAlgorithm() {
	//Mazes
	if (interface.GetMazeIndex() == 0) { //Prims Maze
		maze = new PrimsMaze(interface.GetMazeSize());
	}
	if (interface.GetMazeIndex() == 1) { //Kruskal Maze
		maze = new KruskalsMaze(interface.GetMazeSize());
	}
	//Solving
	if (interface.GetAlgorithmIndex() == 0) { //Depth first search
		algorithm = new DepthFirstSearch();
	}
}

bool MazeDisplay::OnCreate() {
	//Maze Stuff
	SetUpMazeAndAlgorithm();
	
	//SDL Stuff
	int w, h;
	float xAxis = 15.0f;
	float yAxis = 7.5f;
	SDL_GetWindowSize(window, &w, &h);
	//projection matrices
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(-xAxis, xAxis, -yAxis, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	invProjectionMatrix = MMath::inverse(projectionMatrix);
	//wall texture
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* tempTexture;
	tempTexture = IMG_Load("white.png");
	wallTexture = SDL_CreateTextureFromSurface(renderer, tempTexture);
	if (wallTexture == nullptr) printf("%s\n", SDL_GetError());
	if (tempTexture == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
	}
	//traversed texture
	tempTexture = IMG_Load("green.png");
	traversedTexture = SDL_CreateTextureFromSurface(renderer, tempTexture);
	if (traversedTexture == nullptr) printf("%s\n", SDL_GetError());
	if (tempTexture == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
	}
	SDL_FreeSurface(tempTexture);
	tempTexture = NULL;
	delete tempTexture;

	return true;
}

void MazeDisplay::OnDestroy() {
	//if (wallTexture) delete wallTexture;
	SDL_DestroyTexture(wallTexture);
	if (algorithm) delete algorithm;
	//if (traversedTexture) delete traversedTexture;
	SDL_DestroyTexture(traversedTexture);
	//if (maze) delete maze; //coming from the algorithm deconstuctor
	//if (window) delete window;
	SDL_DestroyRenderer(renderer);
}

void MazeDisplay::GenerateMazeAndRunAlgorithm() {
	if (maze->GetCompleted() == false && interface.GetSkipMazeGenBool() == true) {
		maze->Generate(false);
	}
	else if (maze->GetCompleted() == false) {
		maze->Generate(interface.GetGradualGenerationBool());
	}
	else if (algorithm->GetCompleted() == false) {
		algorithm->SolveMaze(maze, 0, maze->GetSize() - 1, maze->GetSize() - 1, 0, interface.GetGradualGenerationBool());
	}
}

void MazeDisplay::Update(const float deltaTime) {
	GenerateMazeAndRunAlgorithm();
}


void MazeDisplay::HandleEvents(const SDL_Event& sdlEvent) {
	//Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f);
	//Vec3 mouseWorldCoords = invProjectionMatrix * mouseCoords;
}


void MazeDisplay::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_Rect currentSquare;
	SDL_Rect currentWall;
	int windowWidth, windowHeight;
	int currentPosX = 0, currentPosY = 0;
	//getting node size on screen
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	int nodeSizeWidth = windowWidth / maze->GetSize();
	int nodeSizeHeight = windowHeight / maze->GetSize();
	//displaying the nodes
	for (int nodeLoopWidth = 0; nodeLoopWidth < maze->GetSize(); nodeLoopWidth++) {
		for (int nodeLoopHeight = 0; nodeLoopHeight < maze->GetSize(); nodeLoopHeight++) {
			//set square for current node
			currentSquare.x = currentPosX;
			currentSquare.y = currentPosY;
			currentSquare.w = nodeSizeWidth;
			currentSquare.h = nodeSizeHeight;
			//render the node
			//check if the node is traversed
			if (maze->nodeArray[nodeLoopWidth][nodeLoopHeight].traversed == true) {
				currentWall.x = currentSquare.x + (nodeSizeWidth / 3);
				currentWall.y = currentSquare.y + (nodeSizeHeight / 3);
				currentWall.w = currentSquare.w / 3;
				currentWall.h = currentSquare.h / 3;
				SDL_RenderCopyEx(renderer, traversedTexture, nullptr, &currentWall, 0, nullptr, SDL_FLIP_NONE);
			}
			//add left wall
			if (maze->nodeArray[nodeLoopWidth][nodeLoopHeight].leftWall == true) {
				currentWall.x = currentSquare.x;
				currentWall.y = currentSquare.y;
				currentWall.w = nodeSizeWidth / 10;
				currentWall.h = currentSquare.h;
				SDL_RenderCopyEx(renderer, wallTexture, nullptr, &currentWall, 0, nullptr, SDL_FLIP_NONE);
			}
			//add top wall
			if (maze->nodeArray[nodeLoopWidth][nodeLoopHeight].topWall == true) {
				currentWall.x = currentSquare.x;
				currentWall.y = currentSquare.y;
				currentWall.w = currentSquare.w;
				currentWall.h = nodeSizeHeight / 10;
				SDL_RenderCopyEx(renderer, wallTexture, nullptr, &currentWall, 0, nullptr, SDL_FLIP_NONE);
			}
			//add right wall
			if (maze->nodeArray[nodeLoopWidth][nodeLoopHeight].rightWall == true) {
				currentWall.x = currentSquare.x + nodeSizeWidth;
				currentWall.y = currentSquare.y;
				currentWall.w = -nodeSizeWidth / 10;
				currentWall.h = currentSquare.h;
				SDL_RenderCopyEx(renderer, wallTexture, nullptr, &currentWall, 0, nullptr, SDL_FLIP_NONE);
			}
			//add bottom wall
			if (maze->nodeArray[nodeLoopWidth][nodeLoopHeight].bottomWall == true) {
				currentWall.x = currentSquare.x;
				currentWall.y = currentSquare.y + nodeSizeHeight;
				currentWall.w = currentSquare.w;
				currentWall.h = -nodeSizeHeight / 10;
				SDL_RenderCopyEx(renderer, wallTexture, nullptr, &currentWall, 0, nullptr, SDL_FLIP_NONE);
			}
			//set up position for next node
			currentPosY += nodeSizeHeight;
		}
		//set up next now
		currentPosX += nodeSizeWidth;
		currentPosY = 0;
	}
	SDL_RenderPresent(renderer);
}