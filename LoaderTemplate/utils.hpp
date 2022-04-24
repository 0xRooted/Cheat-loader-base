#pragma once
#include <Windows.h>
#include <string>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <string>
#include <random>
#include <iostream>
#include <vector>
#include <d3d9.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

namespace utils {
	void SetWindowTo(int top);
	/*
		* top params !
		* ________________
		1 | HWND_BOTTOM
		2 | HWND_NOTOPMOST
		3 | HWND_TOP
		4 | HWND_TOPMOST
	*/

	namespace imgui_utils {
		void TextCenter(const char* fmt, ...);
		bool ButtonCenter(const char* fmt, ...);
		float getx();
		float gety();

		void line(int newId);
		void linevertical();

		void center_text(const char* text, int lineId, bool separator);
		void center_text_ex(const char* text, float width_available, int lineId, bool separator);

		namespace other
		{

			float get_window_size_x();
			float get_window_size_y();
			char* get_window_name();
			bool ColorPicker(const char* label, ImColor* color);
		}
	};
	void killProcessByName(const char* filename);
	void exit_nvidia(void);
	std::string ws2s(const std::wstring& wstr);
	std::string random_string(size_t len);
	template<typename T>
	T random(T range_from, T range_to) {
		std::random_device                  rand_dev;
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<T>    distr(range_from, range_to);
		return distr(generator);
	}
	bool proc_running_utils(std::string processName);
	BOOL IsElevated();
	void ElevateNow();
	std::string executable_name();
	void rename_myself(void);
	void strreplace(std::string& orgString, const std::string search, const std::string replace);
	ImVec2 Window_screen_res(); // 1 = Width || 2 = Height
	ImVec2 Window_center_res(); // 1 = Width || 2 = Height
	std::vector<std::string> split(std::string s, std::string delimiter, int count);
	int count_occurence_of_char(std::string str, char c);
};