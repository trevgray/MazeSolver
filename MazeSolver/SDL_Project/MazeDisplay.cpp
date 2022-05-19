#include "MazeDisplay.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Randomizer.h"
#include <iostream>
#include "VMath.h"
using namespace MATH;

MazeDisplay::MazeDisplay(SDL_Window* sdlWindow_) : wallTexture(nullptr), maze(nullptr) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

MazeDisplay::~MazeDisplay() {
	SDL_DestroyRenderer(renderer);
}

bool MazeDisplay::OnCreate() {
	//Maze Stuff
	maze = new PrimsMaze(50);
	maze->Generate();
	
	//SDL Stuff
	int w, h;
	float xAxis = 15.0f;
	float yAxis = 7.5f;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(-xAxis, xAxis, -yAxis, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	invProjectionMatrix = MMath::inverse(projectionMatrix);

	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* wallImage = IMG_Load("white.png");
	wallTexture = SDL_CreateTextureFromSurface(renderer, wallImage);
	if (wallTexture == nullptr) printf("%s\n", SDL_GetError());
	if (wallImage == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
	}
	else {
		//Do some tricks with the image coords
		Vec3 upperLeft(0.0f, 0.0f, 0.0f);
		Vec3 lowerRight(static_cast<float>(wallImage->w), static_cast<float>(wallImage->h), 0.0f);
		Vec3 ulWorld = invProjectionMatrix * upperLeft;
		Vec3 lrWorld = invProjectionMatrix * lowerRight;
		Vec3 worldCoordsFromScreenCoords = lrWorld - ulWorld;
		//float r = worldCoordsFromScreenCoords.x / 2.0f;
		SDL_FreeSurface(wallImage);
		wallImage = NULL;
		delete wallImage;
	}

	return true;
}

void MazeDisplay::OnDestroy() {
	SDL_DestroyRenderer(renderer);
	delete wallTexture;
	delete maze;
	delete renderer;
	delete window;
}

void MazeDisplay::Update(const float deltaTime) {

}


void MazeDisplay::HandleEvents(const SDL_Event& sdlEvent) {
	Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f);
	Vec3 mouseWorldCoords = invProjectionMatrix * mouseCoords;
}


void MazeDisplay::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_Rect currentSquare;
	SDL_Rect currentWall;
	Vec3 screenCoords;
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
			if (maze->nodeArray[nodeLoopWidth][nodeLoopHeight].leftWall == true) {
				currentWall.x = currentSquare.x;
				currentWall.y = currentSquare.y;
				currentWall.w = nodeSizeWidth / 10;
				currentWall.h = currentSquare.h;
				SDL_RenderCopyEx(renderer, wallTexture, nullptr, &currentWall, 0, nullptr, SDL_FLIP_NONE);
			}
			if (maze->nodeArray[nodeLoopWidth][nodeLoopHeight].topWall == true) {
				currentWall.x = currentSquare.x;
				currentWall.y = currentSquare.y;
				currentWall.w = currentSquare.w;
				currentWall.h = nodeSizeHeight / 10;
				SDL_RenderCopyEx(renderer, wallTexture, nullptr, &currentWall, 0, nullptr, SDL_FLIP_NONE);
			}
			if (maze->nodeArray[nodeLoopWidth][nodeLoopHeight].rightWall == true) {
				currentWall.x = currentSquare.x + nodeSizeWidth;
				currentWall.y = currentSquare.y;
				currentWall.w = -nodeSizeWidth / 10;
				currentWall.h = currentSquare.h;
				SDL_RenderCopyEx(renderer, wallTexture, nullptr, &currentWall, 0, nullptr, SDL_FLIP_NONE);
			}
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