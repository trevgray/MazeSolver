#pragma once
#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include <vector>
#include "PrimsMaze.h"

using namespace MATH;

class MazeDisplay : public Scene {
private:
	SDL_Window* window;
	Matrix4 projectionMatrix;
	Matrix4 invProjectionMatrix;
	SDL_Renderer* renderer;
	SDL_Texture* wallTexture;

	PrimsMaze* maze;
public:
	MazeDisplay(SDL_Window* sdlWindow);
	~MazeDisplay();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
};
