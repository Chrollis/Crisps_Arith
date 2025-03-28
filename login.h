#pragma once
#include "scene.h"

class Login : public Scene {
private:
	string in;
	string feedback;
	string out;
	unsigned state = 0;
	unsigned temp_state = 0;
	IMAGE temp_screen = IMAGE(x_screen, y_screen);
	bool once_grey = 1;
	bool is_wrong = 0;

public:
	Login() = default;
	~Login() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();
};

void Login::enter() {
	enter_loading(5, "��½��������С�");
}
void Login::draw() {
	settextstyle(20, 0, L"����", 0, 0, 0, 0, 0, 0);
	settextcolor(0x000000);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);

	if (state < 2) {
		line(13, y_screen / 2 + 10, x_screen - 13, y_screen / 2 + 10);
		outtextxy(13, y_screen / 2 - 35, string_to_lpcwstr(out));
		outtextxy(16, y_screen / 2 - 10, string_to_lpcwstr(in));
		if (state == 1 || is_wrong) {
			drawtext(string_to_lpcwstr(feedback), &feedback_rect, DT_RIGHT);
		}
	}
	else {
		setfillcolor(0xFFFFFF);
		setlinecolor(0x000000);
		putimage(0, 0, &temp_screen);
		fillroundrect(13, y_screen / 2 - 15, x_screen - 13, y_screen / 2 + 15, 10, 10);
		drawtext(string_to_lpcwstr(out), &screen_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}
void Login::proceed(clock_t delta) {
	switch (state) {
	case 0:
		out = "�������û�ID��";
		break;
	case 1:
		out = "���������";
		if (in.length() > 10) {
			if (proceed_loading(feedback, delta, 5)) {
				is_wrong = 1;
				state -= 1;
				in.clear();
				feedback = "ID���������������롣";
			}
		}
		else if (in.empty()) {
			if (proceed_loading(feedback, delta, 5)) {
				is_wrong = 1;
				state -= 1;
				in.clear();
				feedback = "ID����Ϊ�գ����������롣";
			}
		}
		else {
			if (in != "admin") {
				if (proceed_loading(feedback, delta, 5, "���û����Ϸ������ڼ��ء�")) {
					current_id = in;
					current_mode = "kousuan_B";
					is_ended = 1;
				}
			}
			else {
				if (proceed_loading(feedback, delta, 5, "���ڼ��ع���Աϵͳ��")) {
					current_mode = "kousuan_A";
					is_ended = 1;
				}
			}
		}
		break;
	case 2:
		out = "�Ƿ񷵻����˵���Y/N��?";
		if (once_grey) {
			greyify_screen(&temp_screen);
			once_grey = 0;
		}
		break;
	case 3:
		if (proceed_loading(out, delta, 5, "���ڷ������˵���")) {
			current_mode = "caidan";
			is_ended = 1;
		}
		break;
	default:
		break;
	}
}
void Login::input(const ExMessage& msg) {
	if (state == 0) {
		graphic_input(in, msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = 1;
			in.pop_back();
		}
	}
	if (msg.message == WM_KEYDOWN) {
		if (state != 2 && msg.vkcode == VK_ESCAPE) {
			temp_state = state;
			state = 2;
		}
		else if (state == 2) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
				state = 3;
			}
			else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
				state = temp_state;
				temp_state = 0;
				once_grey = 1;
			}
		}
	}
}
void Login::exit() {

}