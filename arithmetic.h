#pragma once
#include "scene.h"

class Arithmetic :public Scene {
private:
	int score = 0;
	unsigned quantity = 1;
	struct Bonus {
		unsigned time;
		unsigned range;
	}bonus = { 5000,5 };
	struct Operation {
		bool _switch;
		struct interval {
			int min;
			int max;
		}a, b;
	}operation[4] = { 
		{ 1,{0,10},{0,10} },
		{ 1,{0,10},{0,10} },
		{ 0,{0,10},{0,10} },
		{ 0,{0,10},{0,10} }
	};

	IMAGE temp_screen = IMAGE(x_screen, y_screen);
	bool once_grey = 1;
	bool last_inputed = 0;
	bool answered = 1;
	bool is_saved = 0;
	bool is_bonused = 0;
	string id;
	int a = 0;
	int b = 0;
	string show_score;
	string question;
	string answer;
	string in;
	string feedback;
	string password = "password";
	unsigned state = 0;
	unsigned temp_state = 0;
	unsigned op_index = 0;
	unsigned feedback_color = 0;
	unsigned wrong = 0;
	unsigned question_num = 0;
	RECT question_rect = { 13,y_screen / 2 - 35,x_screen - 13,y_screen / 2 - 15 };
public:
	Arithmetic() {
		CreateDirectoryW(L".\\data", nullptr);
	}
	~Arithmetic() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();

	bool get_password();
	bool log(string out_str);
	bool read();
	bool save();

	friend class Admin;
};

void Arithmetic::enter() {
	id = current_id;
	enter_loading(4, "用户数据加载中。");
	get_password();
	if (read()) {
		enter_loading(2, "用户数据加载成功。");
		log("started");
	}
	else {
		enter_loading(2, "用户数据不存在。");
		enter_loading(4, "用户数据创建中。");
		save();
		enter_loading(2, "用户数据创建成功。");
		log("created");
	}
}
void Arithmetic::draw() {
	settextstyle(20, 0, L"仿宋", 0, 0, 0, 0, 0, 0);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);
	if (state < 2) {
		line(13, y_screen / 2 + 10, x_screen - 13, y_screen / 2 + 10);
		settextcolor(0x000000);
		drawtext(string_to_lpcwstr(question), &question_rect, DT_LEFT);
		settextcolor(0xCC2222);
		drawtext(string_to_lpcwstr(show_score), &question_rect, DT_RIGHT);
		settextcolor(0x000000);
		outtextxy(13, y_screen / 2 - 10, string_to_lpcwstr(in));
		if (state == 1 || last_inputed) {
			switch (feedback_color) {
			case 0:
				settextcolor(0x000000);
				break;
			case 1:
				settextcolor(0x22CC22);
				break;
			case 2:
				settextcolor(0x2222CC);
				break;
			default:
				break;
			}
			drawtext(string_to_lpcwstr(feedback), &feedback_rect, DT_RIGHT);
		}
	}
	else if (state != 6) {
		setfillcolor(0xFFFFFF);
		setlinecolor(0x000000);
		putimage(0, 0, &temp_screen);
		fillroundrect(13, y_screen / 2 - 15, x_screen - 13, y_screen / 2 + 15, 10, 10);
		drawtext(string_to_lpcwstr(question), &screen_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else {
		settextcolor(0x000000);
		drawtext(string_to_lpcwstr(question), &question_rect, DT_LEFT);
		settextcolor(0x22CC22);
		outtextxy(13, y_screen / 2 - 10, string_to_lpcwstr(in));
		settextcolor(0xCC2222);
		drawtext(string_to_lpcwstr(feedback), &feedback_rect, DT_RIGHT);
	}
}
void Arithmetic::proceed(clock_t delta) {
	static clock_t timer = 0;
	show_score = to_string(score) + "分";
	switch (state) {
	case 0:
		if (answered) {
			do {
				op_index = rand(0, 3);
			} while (!operation[op_index]._switch);
			Operation& current_op = operation[op_index];
			a = rand(current_op.a.min, current_op.a.max);
			if (op_index == 3) {
				do {
					b = rand(current_op.b.min, current_op.b.max);
				} while (b == 0);
			}
			else {
				b = rand(current_op.b.min, current_op.b.max);
			}
			if (op_index == 3) {
				int c = a / b, d = a % b;
				answer = to_string(c) + "..." + to_string(d);
			}
			else {
				int c = 0;
				switch (op_index) {
				case 0:
					c = a + b;
					break;
				case 1:
					c = a - b;
					break;
				case 2:
					c = a * b;
					break;
				default:
					break;
				}
				answer = to_string(c);
			}
			answered = 0;
			timer = 0;
		}
		question = to_string(a) + operation_signal[op_index] + to_string(b) + '=';
		timer += delta;
		break;
	case 1:
		if (in == answer) {
			feedback_color = 1;
			feedback = "回答正确。";
			if (answered == 0) {
				score += max(0, (int)(bonus.range * (1 - (double)timer / bonus.time)));
			}
			answered = 1;
			question_num += 1;
		}
		else {
			feedback_color = 2;
			feedback = "回答错误。";
			score -= bonus.range;
			wrong += 1;
		}
		last_inputed = 1;
		state = 0;
		in.clear();
		if (question_num == quantity) {
			state = 6;
		}
		break;
	case 2:
		question = "是否返回主菜单【Y/N】?";
		temp_state = state;
		if (once_grey) {
			greyify_screen(&temp_screen);
			once_grey = 0;
		}
		break;
	case 3:
		if (proceed_loading(question, delta, 5, "正在返回主菜单。")) {
			current_mode = "caidan";
			is_ended = 1;
		}
		break;
	case 4:
		question = "是否创建新题目【Y/N】?";
		break;
	case 5:
		if (proceed_loading(question, delta, 5, "正在返回主菜单。")) {
			current_mode = "caidan";
			is_ended = 1;
		}
		break;
	case 6:
		if (!is_bonused) {
			int wrong_rate = (int)((double)wrong * 10000 / (quantity + wrong));
			question = "答题结束，错误率为" + to_string(wrong_rate / 100) + '.' + to_string(wrong_rate % 100) + "%。";
			int bonus_score = quantity - wrong_rate / (10000 / quantity);
			score += bonus_score;
			in = "奖励" + to_string(bonus_score) + "分，是否保存【Y/N】？";
			feedback = "当前" + to_string(score)+"分。";
			is_bonused = 1;
		}
		break;
	case 7:
		if (once_grey) {
			greyify_screen(&temp_screen);
			once_grey = 0;
		}
		if (is_saved) {
			if (proceed_loading(question, delta, 5, "正在保存，请稍后。")) {
				is_saved = 0;
				once_grey = 1;
				state = 4;
			}
		}else {
			if (proceed_loading(question, delta, 5, "已放弃保存，请稍后。")) {
				is_saved = 0;
				once_grey = 1;
				state = 4;
			}
		}
		break;
	default:
		break;
	}
}
void Arithmetic::input(const ExMessage& msg) {
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
		if ((state == 0 || state % 2 != 0) && msg.vkcode == VK_ESCAPE) {
			temp_state = state;
			state = 2;
		}
		else {
			switch (state) {
			case 2:
				if (msg.vkcode == 'Y' ||
					msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
					state = 3;
				}
				else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
					state = temp_state;
					temp_state = 0;
					once_grey = 1;
				}
				break;
			case 4:
				if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
					state = 5;
				}
				else if (msg.vkcode == 'Y' ||
					msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
					*this = Arithmetic();
					this->id = current_id;
					this->read();
				}
				break;
			case 6:
				if (msg.vkcode == 'Y' ||
					msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
					save();
					is_saved = 1;
					log("saved=" + to_string(score));
					state = 7;
				}
				else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
					is_saved = 0;
					log("quit to save");
					state = 7;
				}
				break;
			default:
				break;
			}
		}
	}
}
void Arithmetic::exit() {

}

bool Arithmetic::get_password() {
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
bool Arithmetic::log(string out_str = string("")) {
	ofstream ofs(".\\data\\log.txt", ios::app);
	if (!ofs.is_open()) {
		return 0;
	}
	ofs << get_time_string() << ": ";
	ofs << "ID: " << id << "; ";
	ofs << "Issue: "<< out_str << endl;
	return 1;
}
bool Arithmetic::save() {
	ostringstream oss;
	oss << score << ' ' << quantity << ' ' << bonus.time << ' '
		<< bonus.range << ' ';
	for (int i = 0; i < 4; ++i) {
		oss << operation[i]._switch << ' '
			<< operation[i].a.min << ' ' << operation[i].a.max << ' '
			<< operation[i].b.min << ' ' << operation[i].b.max << ' ';
	}
	string original = oss.str();
	encrypt(original, password);
	ofstream ofs(".\\data\\" + id + ".bin", ios::binary);
	if (!ofs.is_open()) {
		return 0;
	}
	ofs.write(original.data(), original.size());
	ofs.close();
	return 1;
}
bool Arithmetic::read() {
	ifstream ifs(".\\data\\" + id + ".bin", ios::binary);
	if(!ifs.is_open()) {
		return 0;
	}
	string original((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	ifs.close();
	encrypt(original, password);
	istringstream iss(original);
	iss >> score >> quantity >> bonus.time
		>> bonus.range;
	for (int i = 0; i < 4; ++i) {
		iss >> operation[i]._switch
			>> operation[i].a.min >> operation[i].a.max
			>> operation[i].b.min >> operation[i].b.max;
	}
	return 1;
}