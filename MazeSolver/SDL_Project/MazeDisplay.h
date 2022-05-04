#pragma once

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include "Ball.h"
#include <vector>

using namespace MATH;

#define NUM_OBJECTS 1

class MazeDisplay : public Scene {
private:
	SDL_Window* window;
	Matrix4 projectionMatrix;
	Matrix4 invProjectionMatrix;
	SDL_Renderer* renderer;
	SDL_Texture* wallTexture;
	int mazeSize;

public:
	MazeDisplay(SDL_Window* sdlWindow);
	~MazeDisplay();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
};
