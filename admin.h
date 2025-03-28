#pragma once
#include "arithmetic.h"

class Admin :public Scene {
private:
	string variable;
	Arithmetic* current = nullptr;

	string in;
	string feedback;
	string out;
	string password = "password";
	unsigned state = 12;
	unsigned temp_state = 0;
	IMAGE temp_screen = IMAGE(x_screen, y_screen);
	bool once_grey = 1;
	bool is_wrong = 0;
	bool is_inputed = 0;
	bool is_legal = 0;

public:
	Admin() = default;
	~Admin() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();

	bool get_password();
};

void Admin::enter() {
	enter_loading(5, "����Ա��������С�");
	get_password();
}
void Admin::draw() {
	settextstyle(20, 0, L"����", 0, 0, 0, 0, 0, 0);
	settextcolor(0x000000);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);

	if ((state < 2 || state > 3) && state < 8 || state>11) {
		line(13, y_screen / 2 + 10, x_screen - 13, y_screen / 2 + 10);
		outtextxy(13, y_screen / 2 - 35, string_to_lpcwstr(out));
		outtextxy(16, y_screen / 2 - 10, string_to_lpcwstr(in));
		if (state % 2 || is_wrong) {
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
void Admin::proceed(clock_t delta) {
	switch (state) {
	case 0:
		out = "������Ŀ���û�ID��";
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
					current = new Arithmetic();
					current->id = in;
					if (current->read()) {
						current->log("started");
					}
					else {
						current->save();
						current->log("created");
					}
					in.clear();
					is_wrong = 0;
					state = 4;
				}
			}
			else {
				if (proceed_loading(feedback, delta, 5)) {
					is_wrong = 1;
					state -= 1;
					in.clear();
					feedback = "�����ظ��������ԱID��";
				}
			}
		}
		break;
	case 2:
		out = "�Ƿ񷵻ص�¼���桾Y/N��?";
		if (once_grey) {
			greyify_screen(&temp_screen);
			once_grey = 0;
		}
		break;
	case 3:
		if (proceed_loading(out, delta, 5, "���ڷ��ص�¼���档")) {
			current_mode = "kousuan";
			is_ended = 1;
		}
		break;
	case 4:
		out = "���������޸������ƣ�";
		break;
	case 5:
		out = "���������";
		if (is_inputed) {
			is_legal = 0;
			for (const string& s : legal_varible) {
				if (in == s) {
					is_legal = 1;
					break;
				}
			}
			is_inputed = 0;
		}
		if (is_legal) {
			if (proceed_loading(feedback, delta, 5, "�ñ������ڣ�������ת��")) {
				variable = in;
				in.clear();
				state = 6;
				is_wrong = 0;
			}
		}
		else {
			if (proceed_loading(feedback, delta, 5)) {
				is_wrong = 1;
				state -= 1;
				in.clear();
				feedback = "�ñ��������ڣ����������롣";
			}
		}
		break;
	case 6:
		out = "������Ŀ��ֵ��";
		break;
	case 7:
		out = "���������";
		if (!in.empty()) {
			if (is_inputed) {
				istringstream iss(in);
				size_t length = variable.length();
				auto switch_operation = [&](const size_t i) {
					if (length == 3) {
						iss >> current->operation[i]._switch
							>> current->operation[i].a.min
							>> current->operation[i].a.max
							>> current->operation[i].b.min
							>> current->operation[i].b.max;
					}
					else if (length == 5) {
						if (variable[4] == 'a') {
							iss >> current->operation[i].a.min
								>> current->operation[i].a.max;
						}
						else {
							iss >> current->operation[i].b.min
								>> current->operation[i].b.max;
						}
					}
					else {
						if (variable[4] == 'a') {
							if (variable[7] == 'i') {
								iss >> current->operation[i].a.min;
							}
							else {
								iss >> current->operation[i].a.max;
							}
						}
						else if (variable[4] == 'b') {
							if (variable[7] == 'i') {
								iss >> current->operation[i].b.min;
							}
							else {
								iss >> current->operation[i].b.max;
							}
						}
						else {
							iss >> current->operation[i]._switch;
						}
					}
					};
				switch (variable[0]) {
				case 's':
					if (variable[1] == 'c') {
						iss >> current->score;
					}
					else {
						switch_operation(1);
					}
					break;
				case 'b':
					if (length == 5) {
						iss >> current->bonus.time >> current->bonus.range;
					}
					else {
						if (variable[6] == 't') {
							iss >> current->bonus.time;
						}
						else {
							iss >> current->bonus.range;
						}
					}
					break;
				case 'a':
					switch_operation(0);
					break;
				case 'm':
					switch_operation(2);
					break;
				case 'd':
					switch_operation(3);
					break;
				case 'q':
					iss >> current->quantity;
					break;
				case 'p':
					if (!in.empty()) {
						ofstream ofs(".\\data\\password.bin", ios::binary);
						if (ofs.is_open()) {
							string original = in;
							encrypt(original);
							ofs.write(original.data(), original.size());
							ofs.close();
						}
						password = in;
						current->save();
					}
					break;
				default:
					break;
				}
				is_inputed = 0;
				is_wrong = 0;
			}
			if (is_wrong == 0) {
				if (proceed_loading(feedback, delta, 5, "�����룬���ڱ��档")) {
					current->save();
					if (variable == "password") {
						in = "/invisible/";
					}
					current->log(variable + "->" + in);
					in.clear();
					state = 8;
					is_inputed = 1;
				}
			}
		}
		else {
			if (proceed_loading(feedback, delta, 5)) {
				is_wrong = 1;
				state -= 1;
				feedback = "����ֵ����Ϊ�ա�";
				in.clear();
			}
		}
		break;
	case 8:
		out = "�������û������ѡ��U/V��?";
		if (once_grey) {
			greyify_screen(&temp_screen);
			once_grey = 0;
		}
		break;
	case 9:
		if (proceed_loading(out, delta, 5, "���ڷ����û�ѡ����档")) {
			state = 0;
		}
		break;
	case 10:
		if (proceed_loading(out, delta, 5, "���ڷ��ر���ѡ����档")) {
			state = 4;
		}
		break;
	case 12:
		out = "���������룺";
		break;
	case 13:
		out = "���������";
		if (in == password) {
			if (proceed_loading(feedback, delta, 5, "������ȷ�������ؼ��ء�")) {
				state = 0;
				in.clear();
			}
		}
		else {
			if (proceed_loading(feedback, delta, 5, "������󣬳�����ֹ��")) {
				current_mode = "tuichu";
				is_ended = 1;
			}
		}
	default:
		break;
	}
}
void Admin::input(const ExMessage& msg) {
	if (state % 2 == 0 && state != 8) {
		graphic_input(in, msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			in.pop_back();
			state += 1;
			is_inputed = 1;
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
		else if (state == 8) {
			if (msg.vkcode == 'U') {
				state = 9;
				once_grey = 1;
			}
			else if (msg.vkcode == 'V') {
				state = 10;
				once_grey = 1;
			}
		}
	}
}
void Admin::exit() {
	delete current;
}

bool Admin::get_password() {
	ifstream ifs(".\\data\\password.bin", ios::binary);
	string original;
	if (!ifs.is_open()) {
		ofstream ofs(".\\data\\password.bin", ios::binary);
		if (!ofs.is_open()) {
			return 0;
		}
		original = password;
		encrypt(original);
		ofs.write(original.data(), original.size());
		ofs.close();
	}
	else {
		original = string((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
		ifs.close();
		encrypt(original);
		istringstream iss(original);
		iss >> password;
	}
	return 1;
}