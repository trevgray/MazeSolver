#include "Scene5.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Randomizer.h"
#include <iostream>
#include "VMath.h"
using namespace MATH;

Scene5::Scene5(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//Ball* temp = new Ball(Vec3(r.rand(-15.0f, 15.0f), r.rand(-7.5f, 7.5f), 0.0f), Vec3(r.rand(-10.0f, 10.0f), r.rand(-10.0f, 10.0f), 0.0f), Vec3(0.0f, -8.7f, 0.0f), 1.0f);
	Randomizer r;
	gameObjVector.push_back(new Ball(Vec3(r.rand(-15.0f, 15.0f), r.rand(-7.5f, 7.5f), 0.0f), Vec3(r.rand(-10.0f, 10.0f), r.rand(-10.0f, 10.0f), 0.0f), Vec3(0.0f, -8.7f, 0.0f), 1.0f));
	//pos vec             vel vec              accel vec           mass
	/*gameObject[0] = new Ball(Vec3(-10.0f, 2.2f, 0.0f),
		Vec3(15.0f, -1.0f, 0.0f),
		Vec3(0.0f, 0.0f, 0.0f), 1.0f);

	gameObject[1] = new Ball(Vec3(10.0f, -2.2f, 0.0f),
		Vec3(-15.0f, 1.0f, 0.0f),
		Vec3(0.0f, 0.0f, -0.0f), 1.0f);

	gameObject[2] = new Ball(Vec3(5.0f, -2.2f, 0.0f),
		Vec3(-20.0f, 1.0f, 0.0f),
		Vec3(0.0f, 0.0f, -0.0f), 1.0f);

	gameObject[3] = new Ball(Vec3(13.0f, 6.2f, 0.0f),
		Vec3(-20.0f, 1.0f, 0.0f),
		Vec3(0.0f, 0.0f, -0.0f), 1.0f);*/
}

Scene5::~Scene5() {
	for (Ball* obj: gameObjVector) {
		delete obj;
	}
	SDL_DestroyRenderer(renderer);
}

bool Scene5::OnCreate() {
	int w, h;
	float xAxis = 15.0f;
	float yAxis = 7.5f;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(-xAxis, xAxis, -yAxis, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	invProjectionMatrix = MMath::inverse(projectionMatrix);

	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* ballImage = IMG_Load("ball.png");
	SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballImage);
	if (ballTexture == nullptr) printf("%s\n", SDL_GetError());
	if (ballImage == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
	}
	else {

		/// Do some tricks with the image coords
		Vec3 upperLeft(0.0f, 0.0f, 0.0f);
		Vec3 lowerRight(static_cast<float>(ballImage->w), static_cast<float>(ballImage->h), 0.0f);
		Vec3 ulWorld = invProjectionMatrix * upperLeft;
		Vec3 lrWorld = invProjectionMatrix * lowerRight;
		Vec3 worldCoordsFromScreenCoords = lrWorld - ulWorld;
		///float r = worldCoordsFromScreenCoords.x / 2.0f;

		for (Ball* obj : gameObjVector) {
			obj->setTexture(ballTexture);
			obj->setImageSizeWorldCoords(worldCoordsFromScreenCoords);
		}
		SDL_FreeSurface(ballImage);

	}
	return true;
}

void Scene5::OnDestroy() {
	/// We really need to deal with the textures better, they are left dangling right now
	SDL_DestroyRenderer(renderer);
}

void Scene5::Update(const float deltaTime) {
	for (Ball* obj : gameObjVector) {
		obj->Update(deltaTime);
	}
	//Collision Detection
	for (int i = 0; i < gameObjVector.size() - 1; i++) {
		for (int j = i + 1; j < gameObjVector.size(); j++) {
			float radius1 = gameObjVector[i]->getRadius();
			float radius2 = gameObjVector[j]->getRadius();
			float distance = VMath::distance(gameObjVector[i]->getPos(), gameObjVector[j]->getPos());
			if (distance < radius1 + radius2) {
				float e = 1.0f;
				Vec3 L = gameObjVector[i]->getPos() - gameObjVector[j]->getPos();
				Vec3 n = VMath::normalize(L);
				float v1p = VMath::dot(gameObjVector[i]->getVel(), n);
				float v2p = VMath::dot(gameObjVector[j]->getVel(), n);
				if (v1p - v2p > 0.0f) continue; //no sticking
				float v1pNew = ((gameObjVector[i]->getMass() - e * gameObjVector[j]->getMass()) * v1p) + ((1.0f + e) * gameObjVector[j]->getMass() * v2p) / (gameObjVector[i]->getMass() + gameObjVector[j]->getMass());
				float v2pNew = ((gameObjVector[j]->getMass() - e * gameObjVector[i]->getMass()) * v2p) + ((1.0f + e) * gameObjVector[i]->getMass() * v1p) / (gameObjVector[i]->getMass() + gameObjVector[j]->getMass());
				Vec3 v1New = gameObjVector[i]->getVel() + (v1pNew - v1p) * n;
				Vec3 v2New = gameObjVector[j]->getVel() + (v2pNew - v2p) * n;
				gameObjVector[i]->setVel(v1New);
				gameObjVector[j]->setVel(v2New);
			}
		}
	}
}


void Scene5::HandleEvents(const SDL_Event& sdlEvent) {
	Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f);
	Vec3 mouseWorldCoords = invProjectionMatrix * mouseCoords;
	if (sdlEvent.type == SDL_MOUSEBUTTONDOWN && sdlEvent.button.button == SDL_BUTTON_LEFT) {
		//printf("Left mouse\n");
		Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f);
		///mouseCoords.print();
		Vec3 worldCoords = invProjectionMatrix * mouseCoords;
		worldCoords.z = 0.0f;
		Randomizer r;
		Ball* temp = new Ball(worldCoords, Vec3(r.rand(-10.0f, 10.0f), r.rand(-10.0f, 10.0f), 0.0f), Vec3(0.0f, -8.7f, 0.0f), 1.0f);
		temp->setTexture(gameObjVector[0]->getTexture());
		temp->setImageSizeWorldCoords(gameObjVector[0]->getImageSizeWorldCoords());
		gameObjVector.push_back(temp);
		//worldCoords.print();
		/*for (Ball* obj: gameObjVector) {
			Vec3 pos = obj->getPos();
			float distance = VMath::distance(pos, worldCoords);
			float radius = obj->getRadius();
			if (distance < radius) {
				obj->selected();
				initialVec3 = obj->getPos();
			}
		}*/

	}
	else if (sdlEvent.type == SDL_MOUSEMOTION && SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		for (Ball* obj : gameObjVector) {
			if (obj->getSelected() == true) {
				obj->setPos(mouseWorldCoords);
				//initialVec3 = gameObject[i]->getPos();
			}
		}
		//printf("moving\n");
	}

	else if (sdlEvent.type == SDL_MOUSEBUTTONUP && sdlEvent.button.button == SDL_BUTTON_LEFT) {
		for (Ball* obj : gameObjVector) {
			if (obj->getSelected() == true) {
				initialVec3 = obj->getPos() - initialVec3;
				obj->setVel(initialVec3);
				//printf("yoyoyo %d\n", i);
				obj->deselected();
			}
		}
	}
}


void Scene5::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	for (Ball* obj : gameObjVector) {
		SDL_QueryTexture(obj->getTexture(), nullptr, nullptr, &w, &h);
		screenCoords = projectionMatrix * obj->getPos();
		square.x = static_cast<int>(screenCoords.x) - w / 2;
		square.y = static_cast<int>(screenCoords.y) - h / 2;
		square.w = w;
		square.h = h;
		SDL_RenderCopyEx(renderer, obj->getTexture(), nullptr, &square, obj->getRotation(), nullptr, SDL_FLIP_NONE);

	}
	SDL_RenderPresent(renderer);
}