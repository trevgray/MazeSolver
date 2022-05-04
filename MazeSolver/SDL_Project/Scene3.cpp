#include "Scene3.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Randomizer.h"
#include <iostream>
#include "VMath.h"
using namespace MATH;

Scene3::Scene3(SDL_Window* sdlWindow_){
	Randomizer r;
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	//pos vec             vel vec              accel vec           mass
	gameObject[0] = new Ball(Vec3(-10.0f, -4.0f, 0.0f),
							    Vec3(0.0, 0.0f, 0.0f),
								Vec3(0.0f, 0.0f, 0.0f), 1.0f);

	gameObject[1] = new Ball(Vec3(10.0f, 5.0f, 0.0f),
								Vec3(0.0, 0.0f, 0.0f),
								Vec3(0.0f, 0.0f, -0.0f), 1.0f);

	gameObject[2] = new Ball(Vec3(0.0f, 0.0f, 0.0f),
								Vec3(0.0, 0.0f, 0.0f),
								Vec3(0.0f, 0.0f, 0.0f), 1.0f);

}

Scene3::~Scene3(){
	for(int i = 0; i < NUM_OBJECTS; ++i){
		delete gameObject[i];
	}
	SDL_DestroyRenderer(renderer);
}

bool Scene3::OnCreate() {
	int w, h;
	float xAxis = 15.0f;
	float yAxis = 7.5f;
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(-xAxis, xAxis, -yAxis, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	invProjectionMatrix = MMath::inverse(projectionMatrix);
	
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* ballImage = IMG_Load("ball.png");
	SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer,ballImage);
	if(ballTexture == nullptr) printf("%s\n",SDL_GetError());
	if(ballImage == nullptr){
		std::cerr << "Can't open the image" << std::endl;
		return false;
	} else {
		
		/// Do some tricks with the image coords
		Vec3 upperLeft(0.0f,0.0f,0.0f); 
		Vec3 lowerRight(static_cast<float>(ballImage->w), static_cast<float>(ballImage->h),0.0f);
		Vec3 ulWorld = invProjectionMatrix * upperLeft;
		Vec3 lrWorld = invProjectionMatrix * lowerRight;
		Vec3 worldCoordsFromScreenCoords = lrWorld - ulWorld;
		///float r = worldCoordsFromScreenCoords.x / 2.0f;
		
		for(int i = 0; i < NUM_OBJECTS; i++){
			gameObject[i]->setTexture(ballTexture);
			gameObject[i]->setImageSizeWorldCoords(worldCoordsFromScreenCoords);
		}
		SDL_FreeSurface(ballImage);
		
	}
	return true;
}

void Scene3::OnDestroy() { 
	/// We really need to deal with the textures better, they are left dangling right now
	SDL_DestroyRenderer(renderer);
}

void Scene3::Update(const float deltaTime) {
	for(int i = 0; i < NUM_OBJECTS; i = i + 1){
		gameObject[i]->Update(deltaTime);
	}
}


void Scene3::HandleEvents(const SDL_Event& sdlEvent){
	Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f);
	Vec3 mouseWorldCoords = invProjectionMatrix * mouseCoords;
	if (sdlEvent.type == SDL_MOUSEBUTTONDOWN && sdlEvent.button.button == SDL_BUTTON_LEFT) {
		//printf("Left mouse\n");
		Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y),0.0f);
		///mouseCoords.print();
		Vec3 worldCoords = invProjectionMatrix * mouseCoords;
		worldCoords.z = 0.0f;
		//worldCoords.print();
		for (int i = 0; i < NUM_OBJECTS; ++i) {
			Vec3 pos = gameObject[i]->getPos();
			float distance = VMath::distance(pos , worldCoords);
			float radius = gameObject[i]->getRadius();
			if(distance < radius){
				printf("ball %d\n",i);
				gameObject[i]->selected();
				initialVec3 = gameObject[i]->getPos();
			}
		}

	}else if(sdlEvent.type == SDL_MOUSEMOTION && SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)){
		for (int i = 0; i < NUM_OBJECTS; ++i) {
			if (gameObject[i]->getSelected() == true) {
				gameObject[i]->setPos(mouseWorldCoords);
				//initialVec3 = gameObject[i]->getPos();
			}
		}
			//printf("moving\n");
	}
	
	else if(sdlEvent.type == SDL_MOUSEBUTTONUP && sdlEvent.button.button == SDL_BUTTON_LEFT){
		for (int i = 0; i < NUM_OBJECTS; ++i) {
			if (gameObject[i]->getSelected() == true) {
				initialVec3 = gameObject[i]->getPos() - initialVec3;
				gameObject[i]->setVel(initialVec3);
				printf("yoyoyo %d\n", i);
				gameObject[i]->deselected();
			}
		}
	}
}


void Scene3::Render() {
	SDL_SetRenderDrawColor(renderer, 0,0,0,0);
	SDL_RenderClear(renderer);
	SDL_Rect square;
	Vec3 screenCoords;
	int w,h;
	
	for(int i = 0; i < NUM_OBJECTS; ++i){
		SDL_QueryTexture(gameObject[i]->getTexture(), nullptr, nullptr, &w, &h);
		screenCoords = projectionMatrix * gameObject[i]->getPos();
		square.x = static_cast<int>(screenCoords.x) - w/2;
		square.y = static_cast<int>(screenCoords.y) - h/2;
		square.w = w;
		square.h = h;
		SDL_RenderCopyEx(renderer, gameObject[i]->getTexture(),nullptr, &square, gameObject[i]->getRotation(), nullptr, SDL_FLIP_NONE);
		
	}
	SDL_RenderPresent(renderer);
}