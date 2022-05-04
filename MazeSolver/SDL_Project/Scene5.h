#ifndef Scene5_H
#define Scene5_H

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include "Ball.h"
#include <vector>

using namespace MATH;

#define NUM_OBJECTS 1

class Scene5 : public Scene {
private:
	SDL_Window* window;
	Matrix4 projectionMatrix;
	Matrix4 invProjectionMatrix;
	//Body* gameObject[NUM_OBJECTS];
	std::vector<Ball*> gameObjVector;
	SDL_Renderer* renderer;


public:
	Vec3 initialVec3;
	Scene5(SDL_Window* sdlWindow);
	~Scene5();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
};

#endif
