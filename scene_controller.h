#pragma once
#include "main_menu.h"
#include "login.h"
#include "arithmetic.h"
#include "admin.h"

class SceneController {
private:
	Scene* current = nullptr;
public:
	enum SceneType {
		SMainMenu,
		SLogin,
		SArithmetic,
		SAdmin,
	};
public:
	SceneController() = default;
	~SceneController() = default;

	void switch_list(SceneType type);
	void set(SceneType type);
	void switch_to(SceneType type);

	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
};

void SceneController::switch_list(SceneType type) {
	switch (type) {
	case SMainMenu:
		current = new MainMenu;
		break;
	case SLogin:
		current = new Login;
		break;
	case SArithmetic:
		current = new Arithmetic;
		break;
	case SAdmin:
		current = new Admin;
		break;
	default:
		break;
	}
}
void SceneController::set(SceneType type) {
	switch_list(type);
	current->enter();
}
void SceneController::switch_to(SceneType type) {
	current->exit();
	delete current;
	switch_list(type);
	current->enter();
}
void SceneController::proceed(clock_t delta) {
	current->proceed(delta);
}
void SceneController::draw() {
	setfillcolor(0xAAAAAA);
	solidrectangle(0, 0, x_screen, y_screen);
	setfillcolor(0xFFFFFF);
	setlinecolor(0x000000);
	setlinestyle(PS_SOLID, 2);
	fillroundrect(4, 4, x_screen - 4, y_screen - 4, 10, 10);
	current->draw();
}
void SceneController::input(const ExMessage& msg) {
	current->input(msg);
}