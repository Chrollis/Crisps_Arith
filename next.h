#pragma once
#include "pre.h"

class MainMenu : public Scene {
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
	MainMenu() = default;
	~MainMenu() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();
};

void MainMenu::enter() {
	current_mode.clear();
	enter_loading(5, "菜单界面加载中。");
}
void MainMenu::draw() {
	settextstyle(20, 0, L"仿宋", 0, 0, 0, 0, 0, 0);
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
void MainMenu::proceed(clock_t delta) {
	switch (state) {
	case 0:
		out = "请输入模式名拼音：";
		break;
	case 1:
		out = "输入结束。";
		if ([&] {
			bool f = 0;
			for (const string& s : legal_mode) {
				if (in == s) {
					f = 1;
					break;
				}
			}
			return f;
			}()) {
			if (proceed_loading(feedback, delta, 5, "该模式存在，正在跳转。")) {
				current_mode = in;
				is_ended = 1;
			}
		}
		else {
			if (proceed_loading(feedback, delta, 5)) {
				is_wrong = 1;
				state -= 1;
				in.clear();
				feedback = "该模式不存在，请重新输入。";
			}
		}
		break;
	case 2:
		out = "是否退出程序【Y/N】?";
		if (once_grey) {
			greyify_screen(&temp_screen);
			once_grey = 0;
		}
		break;
	case 3:
		if (proceed_loading(out, delta, 5, "正在退出。")) {
			current_mode = "tuichu";
			is_ended = 1;
		}
		break;
	default:
		break;
	}
}
void MainMenu::input(const ExMessage& msg) {
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
void MainMenu::exit() {

}

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
	enter_loading(5, "登陆界面加载中。");
}
void Login::draw() {
	settextstyle(20, 0, L"仿宋", 0, 0, 0, 0, 0, 0);
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
		out = "请输入用户ID：";
		break;
	case 1:
		out = "输入结束。";
		if (in.length() > 10) {
			if (proceed_loading(feedback, delta, 5)) {
				is_wrong = 1;
				state -= 1;
				in.clear();
				feedback = "ID过长，请重新输入。";
			}
		}
		else if (in.empty()) {
			if (proceed_loading(feedback, delta, 5)) {
				is_wrong = 1;
				state -= 1;
				in.clear();
				feedback = "ID不得为空，请重新输入。";
			}
		}
		else {
			if (in != "admin") {
				if (proceed_loading(feedback, delta, 5, "该用户名合法，正在加载。")) {
					current_id = in;
					current_mode = "kousuan_B";
					is_ended = 1;
				}
			}
			else {
				if (proceed_loading(feedback, delta, 5, "正在加载管理员系统。")) {
					current_mode = "kousuan_A";
					is_ended = 1;
				}
			}
		}
		break;
	case 2:
		out = "是否返回主菜单【Y/N】?";
		if (once_grey) {
			greyify_screen(&temp_screen);
			once_grey = 0;
		}
		break;
	case 3:
		if (proceed_loading(out, delta, 5, "正在返回主菜单。")) {
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

class Admin :public Scene {
private:
	string variable;
	Arithmetic* current = nullptr;

	string in;
	string feedback;
	string out;
	unsigned state = 0;
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
};

void Admin::enter() {
	enter_loading(5, "管理员界面加载中。");
}
void Admin::draw() {
	settextstyle(20, 0, L"仿宋", 0, 0, 0, 0, 0, 0);
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
		out = "请输入目标用户ID：";
		break;
	case 1:
		out = "输入结束。";
		if (in.length() > 10) {
			if (proceed_loading(feedback, delta, 5)) {
				is_wrong = 1;
				state -= 1;
				in.clear();
				feedback = "ID过长，请重新输入。";
			}
		}
		else if (in.empty()) {
			if (proceed_loading(feedback, delta, 5)) {
				is_wrong = 1;
				state -= 1;
				in.clear();
				feedback = "ID不得为空，请重新输入。";
			}
		}
		else {
			if (in != "admin") {
				if (proceed_loading(feedback, delta, 5, "该用户名合法，正在加载。")) {
					current = new Arithmetic();
					current->id = in;
					current->get_password();
					if (current->read()) {
						current->log("started");
					}
					else {
						current->save();
						current->log("created");
					}
					in.clear();
					is_wrong = 0;
					state = 12;
				}
			}
			else {
				if (proceed_loading(feedback, delta, 5)) {
					is_wrong = 1;
					state -= 1;
					in.clear();
					feedback = "请勿重复输入管理员ID。";
				}
			}
		}
		break;
	case 2:
		out = "是否返回登录界面【Y/N】?";
		if (once_grey) {
			greyify_screen(&temp_screen);
			once_grey = 0;
		}
		break;
	case 3:
		if (proceed_loading(out, delta, 5, "正在返回登录界面。")) {
			current_mode = "kousuan";
			is_ended = 1;
		}
		break;
	case 4:
		out = "请输入所修改项名称：";
		break;
	case 5:
		out = "输入结束。";
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
			if (proceed_loading(feedback, delta, 5, "该变量存在，正在跳转。")) {
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
				feedback = "该变量不存在，请重新输入。";
			}
		}
		break;
	case 6:
		out = "请输入目标值：";
		break;
	case 7:
		out = "输入结束。";
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
						current->password = in;
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
				if (proceed_loading(feedback, delta, 5, "已输入，正在保存。")) {
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
				feedback = "输入值不得为空。";
				in.clear();
			}
		}
		break;
	case 8:
		out = "返回至用户或变量选择【U/V】?";
		if (once_grey) {
			greyify_screen(&temp_screen);
			once_grey = 0;
		}
		break;
	case 9:
		if (proceed_loading(out, delta, 5, "正在返回用户选择界面。")) {
			state = 0;
		}
		break;
	case 10:
		if (proceed_loading(out, delta, 5, "正在返回变量选择界面。")) {
			state = 4;
		}
		break;
	case 12:
		out = "请输入密码：";
		break;
	case 13:
		out = "输入结束。";
		if (in == current->password) {
			if (proceed_loading(feedback, delta, 5, "密码正确，正在重加载。")) {
				state = 4;
				in.clear();
			}
		}
		else {
			if (proceed_loading(feedback, delta, 5, "密码错误，程序终止。")) {
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