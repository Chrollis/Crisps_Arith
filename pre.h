#pragma once
#include <Windows.h>
#include <graphics.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <functional>

using namespace std;

constexpr unsigned x_screen = 320;
constexpr unsigned y_screen = 200;
constexpr clock_t wait_for = 300;
constexpr unsigned fps = 24;
const string waiting = "正在处理请求，请稍后。";
const vector<string> legal_mode = {
	"caidan",
	"kousuan",
	"tuichu",
	"kousuan admin"
};
const vector<string> legal_varible = {
	"score",
	"quantity",
	"bonus",
	"bonus.time",
	"bonus.range",
	"add",
	"add._switch",
	"add.a",
	"add.a.min",
	"add.a.max",
	"add.b",
	"add.b.min",
	"add.b.max",
	"sub",
	"sub._switch",
	"sub.a",
	"sub.a.min",
	"sub.a.max",
	"sub.b",
	"sub.b.min",
	"sub.b.max",
	"mul",
	"mul._switch",
	"mul.a",
	"mul.a.min",
	"mul.a.max",
	"mul.b",
	"mul.b.min",
	"mul.b.max",
	"div",
	"div._switch",
	"div.a",
	"div.a.min",
	"div.a.max",
	"div.b",
	"div.b.min",
	"div.b.max",
	"password"
};
const string operation_signal[4] = { "+","-","×","÷" };
const char loading[4] = { '/','-','\\','-' };

RECT screen_rect = { 0,0,x_screen,y_screen };
RECT feedback_rect = { 13,y_screen / 2 + 15,x_screen - 13,y_screen / 2 + 35 };
string current_id;
string current_mode;
bool is_ended = 0;

inline int rand(int min, int max) {
	srand(clock() * rand());
	return rand() % (max + 1 - min) + min;
}
bool hide_file(const wchar_t* path, bool _switch = 0) {
	WIN32_FIND_DATA data;
	HANDLE hfind = FindFirstFile(path, &data);
	if (hfind == INVALID_HANDLE_VALUE) return false;
	DWORD attributes = data.dwFileAttributes;
	if (!(attributes & FILE_ATTRIBUTE_HIDDEN)) {
		SetFileAttributes(path, attributes | FILE_ATTRIBUTE_HIDDEN);
	}
	return true;
}
bool show_hidden_file(const wchar_t* path) {
	WIN32_FIND_DATA data;
	HANDLE hfind = FindFirstFile(path, &data);
	if (hfind == INVALID_HANDLE_VALUE) return false;
	DWORD attributes = data.dwFileAttributes;
	if (attributes & FILE_ATTRIBUTE_HIDDEN) {
		SetFileAttributes(path, attributes & ~FILE_ATTRIBUTE_HIDDEN);
	}
	return true;
}
string get_time_string() {
	string str(21, 0);
	time_t now_time;
	tm init = { 0, 0, 0, 1, 0, 1900, 0, 0, -1 };
	tm* time_now = &init;
	time(&now_time);
	localtime_s(time_now, &now_time);
	strftime(&str[0], 21, "%Y/%m/%d, %H:%M:%S", time_now);
	str.pop_back();
	return str;
}
void graphic_input(string& str, const ExMessage& msg) {
	int k = msg.vkcode;
	static bool is_shift = 0;
	static bool is_caps = 0;
	const char shift_num[10] = { ')','!','@','#','$','%','^','&','*','(' };
	const char signal[11] = { ';','=',',','-','.','/','`','[','\\',']','\''};
	const char shift_signal[11] = { ':','+','<','_','>','?','~','{','|','}','\"' };
	const char num_signal[6] = { '*','+','\n','-','.','/' };
	if (msg.message == WM_KEYDOWN) {
		if (k >= '0' && k <= '9') {
			k = is_shift ? '1' : '0';
		}
		else if (k >= 'A' && k <= 'Z') {
			k = is_shift ^ is_caps ? 'A' : 'a';
		}
		else if (k >= VK_NUMPAD0 && k <= VK_NUMPAD9) {
			k = VK_NUMPAD0;
		}
		else if (k >= VK_MULTIPLY && k <= VK_DIVIDE) {
			k = VK_MULTIPLY;
		}
		else if (k >= VK_OEM_1 && k <= VK_OEM_7) {
			k = is_shift ? VK_OEM_PLUS : VK_OEM_1;
		}
		switch (k) {
		case VK_BACK:
		case VK_DELETE:
			if (!str.empty()) {
				str.pop_back();
			}
			break;
		case VK_SHIFT:
			is_shift = 1;
			break;
		case VK_CAPITAL:
			is_caps = !is_caps;
			break;
		case VK_TAB:
		case VK_RETURN:
		case VK_SPACE:
		case '0':
			str += msg.vkcode;
			break;
		case '1':
			str += shift_num[msg.vkcode - k + 1];
			break;
		case 'A':
			str += msg.vkcode;
			break;
		case 'a':
			str += msg.vkcode - 'A' + 'a';
			break;
		case VK_NUMPAD0:
			str += msg.vkcode - VK_NUMPAD0 + '0';
			break;
		case VK_MULTIPLY:
			str += num_signal[msg.vkcode - k];
			break;
		case VK_OEM_1:
			str += signal[msg.vkcode - k];
			break;
		case VK_OEM_PLUS:
			str += shift_signal[msg.vkcode - k + 1];
			break;
		default:
			break;
		}
	}
	else if (msg.message == WM_KEYUP) {
		switch (k) {
		case VK_SHIFT:
			is_shift = 0;
			break;
		default:
			break;
		}
	}
}
inline wstring string_to_wstring(const string& str) {
	if (str.empty()) {
		return L"";
	}
	int size_needed = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), nullptr, 0);
	if (size_needed == 0) {
		return L"";
	}
	wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
	return wstr;
}
#define string_to_lpcwstr(x) string_to_wstring(x).c_str()
void encrypt(string& input, const string& key = "password") {
	size_t length = key.length();
	size_t end = input.length();
	size_t key_index = 0;
	for (size_t i = 0; i < end; i++) {
		input[i] ^= key[key_index++];
		key_index %= length;
	}
}

void enter_loading(const int circle, const char* out_str) {
	settextstyle(20, 0, L"仿宋", 0, 0, 0, 0, 0, 0);
	settextcolor(0x000000);
	setbkmode(TRANSPARENT);
	setlinecolor(0x000000);
	setlinestyle(PS_SOLID, 2);
	string temp;

	for (int i = 0; i < circle; i++) {
		setfillcolor(0xAAAAAA);
		solidrectangle(0, 0, x_screen, y_screen);
		setfillcolor(0xFFFFFF);
		fillroundrect(4, 4, x_screen - 4, y_screen - 4, 10, 10);
		temp = string(out_str) + loading[i % 4];
		drawtext(string_to_lpcwstr(temp), &screen_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		FlushBatchDraw();
		Sleep(wait_for);
	}
}
bool proceed_loading(string& feedback, clock_t delta, const int circle, const string& feedback_str = waiting) {
	static int counter = 0;
	static clock_t timer = 0;

	timer += delta;
	if (timer > wait_for) {
		timer -= wait_for;
		counter++;
	}

	if (counter < 2) {
		feedback = waiting + loading[counter % 4];
		return 0;
	}
	else if (counter < circle) {
		feedback = feedback_str + loading[counter % 4];
		return 0;
	}
	else {
		counter = 0;
		timer = 0;
		return 1;
	}
}
void greyify_screen(IMAGE* screen, double para = 0.6) {
	getimage(screen, 0, 0, x_screen, y_screen);
	DWORD* p_screen = GetImageBuffer(screen);
	for (int i = 0; i < x_screen * y_screen; i++) {
		p_screen[i] = max((int)(p_screen[i] * para), (int)(0xFFFFFF * (1 - para)));
	}
}

class FpsController {
private:
	int interval = 83;
	int fps_target = 12;
	clock_t begin = 0;
	clock_t end = 0;
	int actual_fps = 0;
public:
	FpsController() = default;
	~FpsController() = default;

	void start();
	void delay()const;
	void set(const int fps);
	void draw();
};

void FpsController::start() {
	begin = clock();
}
void FpsController::delay()const {
	Sleep((DWORD)(max(0, interval + clock() - begin)));
}
void FpsController::set(const int fps) {
	interval = 500 / fps;
	fps_target = fps;
}
void FpsController::draw() {
	static int counter = 0;
	if (counter == 0) {
		actual_fps = 1000 * fps_target / (clock() - end);
		end = clock();
	}
	counter = (++counter) % fps_target;
	ostringstream oss;
	oss << "Fps = " << actual_fps << " : " << fps_target;
	settextcolor(0x000000);
	settextstyle(15, 0, L"Times New Roman", 0, 0, 0, 1, 0, 0);
	setbkmode(TRANSPARENT);
	outtextxy(8, y_screen - 23, string_to_lpcwstr(oss.str()));
}


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