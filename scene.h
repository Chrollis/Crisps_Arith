#pragma once
#include "util.h"

class Scene {
public:
	Scene() = default;
	~Scene() = default;

	virtual void enter() {}
	virtual void draw() {}
	virtual void proceed(clock_t delta) {}
	virtual void input(const ExMessage& msg) {}
	virtual void exit() {}
};