#include "utils.hpp"
#include "xorstr.hpp"
#include <algorithm>
#include <cstdarg>

/*
		* top params !
		* ________________
		1 | HWND_BOTTOM
		2 | HWND_NOTOPMOS
		3 | HWND_TOP
		4 | HWND_TOPMOST
*/
void utils::SetWindowTo(int top)
{
	HWND param = 0;
	switch (top)
	{
	case 1: 
		param = HWND_BOTTOM;
		break;
	case 2:
		param = HWND_NOTOPMOST;
		break;
	case 3: 
		param = HWND_TOP;
		break;
	case 4: 
		param = HWND_TOPMOST;
		break;
	default:
		param = HWND_NOTOPMOST;
		break;
	}
	SetWindowPos(GetActiveWindow(), param, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void utils::imgui_utils::TextCenter(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    va_end(args);
	float font_size = ImGui::GetFontSize() * strlen(fmt) / 2;
	ImGui::SameLine(
		ImGui::GetWindowSize().x / 2 -
		font_size + (font_size / 2)
	);
	ImGui::Text(fmt);
}

bool utils::imgui_utils::ButtonCenter(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    va_end(args);
    float font_size = ImGui::GetFontSize() * strlen(fmt) / 2;
    ImGui::SameLine(
        ImGui::GetWindowSize().x / 2 -
        font_size + (font_size / 2)
    );
    return ImGui::Button(fmt);
}

void utils::killProcessByName(const char* filename)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        if (strcmp(pEntry.szExeFile, filename) == 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                (DWORD)pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}

void utils::exit_nvidia(void)
{
    for (int i = 0; i < 4; i++)
        utils::killProcessByName(xorstr_("NVIDIA Share.exe"));
}

std::string utils::ws2s(const std::wstring& wstr)
{
    std::string s(wstr.begin(), wstr.end());
    return s;
}

std::string utils::random_string(size_t len)
{
    auto randchar = []() -> char
    {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(len, 0);
    std::generate_n(str.begin(), len, randchar);
    return str;
}

bool utils::proc_running_utils(std::string processName)
{
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry))
    {
        while (Process32Next(snapshot, &entry))
        {
            if (strcmp(entry.szExeFile, processName.c_str()) == 0)
                exists = true;
        }
    }

    CloseHandle(snapshot);
    return exists;
}

BOOL utils::IsElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if (hToken) {
        CloseHandle(hToken);
    }
    return fRet;
}

void utils::ElevateNow()
{
    char szPath[MAX_PATH] = { 0 };
    if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
    {


        SHELLEXECUTEINFO sei = { sizeof(sei) };

        sei.lpVerb = "runas";
        sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;
        if (!ShellExecuteEx(&sei))
        {
            DWORD dwError = GetLastError();
            if (dwError == ERROR_CANCELLED)
                //Annoys you to Elevate it LOL
                CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ElevateNow, 0, 0, 0);
        }
        else
        {
            exit(0);
        }
    }
}

std::string utils::executable_name()
{
#if defined(PLATFORM_POSIX) || defined(__linux__) //check defines for your setup

    std::string sp;
    std::ifstream("/proc/self/comm") >> sp;
    return sp;

#elif defined(_WIN32)

    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return buf;

#else

    static_assert(false, "unrecognized platform");

#endif
}

void utils::rename_myself(void)
{
    WCHAR path[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, path, MAX_PATH);
    std::wstring a(path);
    std::string ab(a.begin(), a.end());
    std::string new_name = utils::random_string(utils::random(4, 30));
    new_name.append(".exe");
    rename(ab.c_str(), new_name.c_str());
}

void utils::strreplace(std::string &orgString, const std::string search, const std::string replace)
{
    for (size_t pos = 0; ; pos += replace.length())
    {
        pos = orgString.find(search, pos);
        if (pos == std::string::npos)
            break;
        orgString.erase(pos, search.length());
        orgString.insert(pos, replace);
    }
}

ImVec2 utils::Window_screen_res()
{
    return ImVec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

ImVec2 utils::Window_center_res()
{
    return ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
}

float utils::imgui_utils::getx()
{
    return ImGui::GetContentRegionAvail().x;
}
float utils::imgui_utils::gety()
{
    return ImGui::GetContentRegionAvail().y;
}

void utils::imgui_utils::line(int newId)
{
    std::string id = ("imguipp_line_" + std::to_string(newId));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
    {
        ImGui::BeginChild(id.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 1), false);
        ImGui::Separator();
        ImGui::EndChild();
    }
    ImGui::PopStyleColor();
}
void utils::imgui_utils::linevertical()
{
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();
}
void utils::imgui_utils::center_text(const char* text, int lineId, bool separator)
{
    if (text == nullptr)
        return;
    ImGui::Spacing();
    ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text(text);
    ImGui::Spacing();
    if (true == separator)
        line(lineId);
}
void utils::imgui_utils::center_text_ex(const char* text, float width_available, int lineId, bool separator)
{
    if (text == nullptr)
        return;
    ImGui::Spacing();
    ImGui::SameLine((width_available / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text(text);
    ImGui::Spacing();
    if (true == separator)
        line(lineId);
}
float utils::imgui_utils::other::get_window_size_x()
{
    return ImGui::GetWindowSize().x;
}
float utils::imgui_utils::other::get_window_size_y()
{
    return ImGui::GetWindowSize().y;
}

char* utils::imgui_utils::other::get_window_name()
{
    return ImGui::GetCurrentWindow()->Name;
}

bool utils::imgui_utils::other::ColorPicker(const char* label, ImColor* color)
{
    static const float HUE_PICKER_WIDTH = 20.0f;
    static const float CROSSHAIR_SIZE = 7.0f;
    static const ImVec2 SV_PICKER_SIZE = ImVec2(200, 200);

    bool value_changed = false;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 picker_pos = ImGui::GetCursorScreenPos();

    ImColor colors[] = { ImColor(255, 0, 0),
        ImColor(255, 255, 0),
        ImColor(0, 255, 0),
        ImColor(0, 255, 255),
        ImColor(0, 0, 255),
        ImColor(255, 0, 255),
        ImColor(255, 0, 0) };

    for (int i = 0; i < 6; ++i)
    {
        draw_list->AddRectFilledMultiColor(
            ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 10, picker_pos.y + i * (SV_PICKER_SIZE.y / 6)),
            ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 10 + HUE_PICKER_WIDTH,
                picker_pos.y + (i + 1) * (SV_PICKER_SIZE.y / 6)),
            colors[i],
            colors[i],
            colors[i + 1],
            colors[i + 1]);
    }

    float hue, saturation, value;
    ImGui::ColorConvertRGBtoHSV(
        color->Value.x, color->Value.y, color->Value.z, hue, saturation, value);
    auto hue_color = ImColor::HSV(hue, 1, 1);

    draw_list->AddLine(
        ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 8, picker_pos.y + hue * SV_PICKER_SIZE.y),
        ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 12 + HUE_PICKER_WIDTH,
            picker_pos.y + hue * SV_PICKER_SIZE.y),
        ImColor(255, 255, 255));
    {
        const int step = 5;
        ImVec2 pos = ImVec2(0, 0);

        ImVec4 c00(1, 1, 1, 1);
        ImVec4 c10(1, 1, 1, 1);
        ImVec4 c01(1, 1, 1, 1);
        ImVec4 c11(1, 1, 1, 1);
        for (int y = 0; y < step; y++) {
            for (int x = 0; x < step; x++) {
                float s0 = (float)x / (float)step;
                float s1 = (float)(x + 1) / (float)step;
                float v0 = 1.0 - (float)(y) / (float)step;
                float v1 = 1.0 - (float)(y + 1) / (float)step;

                ImGui::ColorConvertHSVtoRGB(hue, s0, v0, c00.x, c00.y, c00.z);
                ImGui::ColorConvertHSVtoRGB(hue, s1, v0, c10.x, c10.y, c10.z);
                ImGui::ColorConvertHSVtoRGB(hue, s0, v1, c01.x, c01.y, c01.z);
                ImGui::ColorConvertHSVtoRGB(hue, s1, v1, c11.x, c11.y, c11.z);

                draw_list->AddRectFilledMultiColor(
                    ImVec2(picker_pos.x + pos.x, picker_pos.y + pos.y),
                    ImVec2(picker_pos.x + pos.x + SV_PICKER_SIZE.x / step, picker_pos.y + pos.y + SV_PICKER_SIZE.y / step),
                    ImGui::ColorConvertFloat4ToU32(c00),
                    ImGui::ColorConvertFloat4ToU32(c10),
                    ImGui::ColorConvertFloat4ToU32(c11),
                    ImGui::ColorConvertFloat4ToU32(c01));

                pos.x += SV_PICKER_SIZE.x / step;
            }
            pos.x = 0;
            pos.y += SV_PICKER_SIZE.y / step;
        }
    }

    float x = saturation * SV_PICKER_SIZE.x;
    float y = (1 - value) * SV_PICKER_SIZE.y;
    ImVec2 p(picker_pos.x + x, picker_pos.y + y);
    draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), ImColor(255, 255, 255));
    draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), ImColor(255, 255, 255));
    draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), ImColor(255, 255, 255));
    draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), ImColor(255, 255, 255));

    ImGui::InvisibleButton("saturation_value_selector", SV_PICKER_SIZE);
    if (ImGui::IsItemHovered())
    {
        ImVec2 mouse_pos_in_canvas = ImVec2(
            ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);
        if (ImGui::GetIO().MouseDown[0])
        {
            mouse_pos_in_canvas.x =
                ImMin(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y);

            value = mouse_pos_in_canvas.y / SV_PICKER_SIZE.y;
            saturation = value == 0 ? 0 : (mouse_pos_in_canvas.x / SV_PICKER_SIZE.x) / value;
            value_changed = true;
        }
    }

    ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 10, picker_pos.y));
    ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

    if (ImGui::IsItemHovered())
    {
        if (ImGui::GetIO().MouseDown[0])
        {
            hue = ((ImGui::GetIO().MousePos.y - picker_pos.y) / SV_PICKER_SIZE.y);
            value_changed = true;
        }
    }

    *color = ImColor::HSV(hue, saturation, value);
    return value_changed | ImGui::ColorEdit3(label, &color->Value.x);
}

std::vector<std::string> utils::split(std::string s, std::string delimiter, int count) {
    int temp = 0;
    if (count == 0)
        temp = INT_MAX;
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {\
        if (temp == count) break;
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
        temp += 1;
    }

    res.push_back(s.substr(pos_start));
    return res;
}
int utils::count_occurence_of_char(std::string str, char c)
{
    int count = 0;

    for (int i = 0; i < str.size(); i++)
        if (str[i] == c) count++;

    return count;
}