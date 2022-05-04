#include "MazeDisplay.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Randomizer.h"
#include <iostream>
#include "VMath.h"
using namespace MATH;

MazeDisplay::MazeDisplay(SDL_Window* sdlWindow_) : mazeSize(0), wallTexture(nullptr) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

MazeDisplay::~MazeDisplay() {
	SDL_DestroyRenderer(renderer);
}

bool MazeDisplay::OnCreate() {
	mazeSize = 10;
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
		/// Do some tricks with the image coords
		Vec3 upperLeft(0.0f, 0.0f, 0.0f);
		Vec3 lowerRight(static_cast<float>(wallImage->w), static_cast<float>(wallImage->h), 0.0f);
		Vec3 ulWorld = invProjectionMatrix * upperLeft;
		Vec3 lrWorld = invProjectionMatrix * lowerRight;
		Vec3 worldCoordsFromScreenCoords = lrWorld - ulWorld;
		///float r = worldCoordsFromScreenCoords.x / 2.0f;
		SDL_FreeSurface(wallImage);

	}

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 1024, 1024);
	unsigned char* pixels;
	int pitch;
	SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
	// set pixels to solid white
	for (int i = 0; i < pitch * 1024; i++) {
		pixels[i] = 255;
	}
	SDL_UnlockTexture(texture);

	return true;
}

void MazeDisplay::OnDestroy() {
	/// We really need to deal with the textures better, they are left dangling right now
	SDL_DestroyRenderer(renderer);
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
	SDL_Rect square;
	Vec3 screenCoords;
	int w, h, windowWidth, windowHeight;
	int currentPosX = 0, currentPosY = 0;
	//getting node size on screen
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	int nodeSizeWidth = windowWidth / mazeSize;
	int nodeSizeHeight = windowHeight / mazeSize;
	//displaying the nodes
	for (int nodeLoopWidth = 0; nodeLoopWidth < mazeSize; nodeLoopWidth++) {
		for (int nodeLoopHeight = 0; nodeLoopHeight < mazeSize; nodeLoopHeight++) {
			//set square for current node
			square.x = currentPosX;
			square.y = currentPosY;
			square.w = nodeSizeWidth;
			square.h = nodeSizeHeight;
			//render the node
			SDL_RenderCopyEx(renderer, wallTexture, nullptr, &square, 0, nullptr, SDL_FLIP_NONE);
			//set up position for next node
			currentPosY += nodeSizeHeight;
		}
		//set up next now
		currentPosX += nodeSizeWidth;
		currentPosY = 0;
	}
	SDL_RenderPresent(renderer);
}