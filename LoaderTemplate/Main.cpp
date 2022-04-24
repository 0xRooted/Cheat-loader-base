#include "Main.hpp"

void protection_thread(__process_info* control_struct)
{
	while (control_struct->running)
	{
		if (IsDebuggerPresent())
		{
			// Temp Code
			MessageBoxA(
				NULL,
				xorstr_("Debugger Found Reporting"),
				xorstr_("Security"),
				MB_OK
			);
			exit(0);
		}
	}
}

/*
Example Loader For Cheat
------------------------
List Of Features:
	1) Checks Permissions
	2) Maps Driver Using Built In Kdmapper
	3) Header File For Driver || Do Not Use This For p2c Use A Api/Server
	4) Good Header File With Usefull Functions
	5) Console Colors Header Creadits To: https://github.com/imfl/color-console
*/

HANDLE iqvw64e_device_handle{};
int main(void)
{
	//Simple Protection Thread Somewhat protect add your own
	struct __process_info* Control_Struct = new __process_info();
	std::thread(protection_thread, Control_Struct).detach();
	// Lets check for permission (Are We Admin Or Not)
	if (utils::IsElevated() == TRUE)
	{
		// Just added this shit for no reason
		utils::rename_myself(); // Renames Exe To Random Name :)
		struct UsersData* user = new UsersData();
		
		FILE* fp;
		AllocConsole();

		freopen_s(
			&fp,
			xorstr_("CONIN$"),
			xorstr_("r+"),
			stdin);

		freopen_s(
			&fp,
			xorstr_("CONOUT$"),
			xorstr_("w+"),
			stderr);

		freopen_s(
			&fp,
			xorstr_("CONOUT$"),
			xorstr_("w+"),
			stdout);

		SetConsoleOutputCP(CP_UTF8);

		setvbuf(
			stdout,
			nullptr,
			_IOFBF,
			1024); // Change 1024 to desired amount needed

		std::cout
			<< u8"╦  ┌─┐┌─┐┌┬┐┌─┐┬─┐\n"
			<< u8"║  │ │├─┤ ││├┤ ├┬┘\n"
			<< u8"╩═╝└─┘┴ ┴─┴┘└─┘┴└─\n"
			<< hue::red_on_black;

		hue::reset();

		std::cout << xorstr_("Username: ") << hue::red_on_black;
		std::getline(std::cin, user->username);
		std::cout << hue::red_on_black << xorstr_("Password: ") << hue::red_on_black;
		std::cout << hue::black_on_black;
		std::getline(std::cin, user->password);
		std::cout << hue::red_on_black << xorstr_("License Key: ") << hue::red_on_black;
		std::cout << hue::black_on_black;
		std::getline(std::cin, user->license_key);

		//Auth Code Start

		if (user->username == xorstr_("test") && user->password == xorstr_("uwu") && user->license_key == xorstr_("0000-0000-0000"))
		{
			MessageBoxA(
				NULL,
				xorstr_("Login Sucessfull"),
				xorstr_("Auth"),
				MB_OK
			);

			//Loading driver start here
			iqvw64e_device_handle = intel_driver::Load();

			if (iqvw64e_device_handle == INVALID_HANDLE_VALUE)
			{
				MessageBoxA(
					NULL,
					xorstr_("Failed To Load Driver"),
					xorstr_("Message"),
					MB_OK);
				exit(0);
			}

			NTSTATUS exitCode = 0;
			if (!kdmapper::MapDriver(iqvw64e_device_handle, rawData, 0, 0)) {
				intel_driver::Unload(iqvw64e_device_handle);
				MessageBoxA(
					NULL,
					xorstr_("Failed To Load Driver"),
					xorstr_("Message"),
					MB_OK);
				exit(0);
			}

			if (!intel_driver::Unload(iqvw64e_device_handle))
			{
				MessageBoxA(
					NULL,
					xorstr_("Failed To Load Driver"),
					xorstr_("Message"),
					MB_OK);
				exit(0);
			}

			MessageBoxA(
				NULL,
				xorstr_("Loaded Driver"),
				xorstr_("Message"),
				MB_OK);
			//Loading driver ends here


			//Cheat Thread Starts Here or ImGui Rendering

			
			// Ending Code
			delete user;
			delete Control_Struct;
			exit(0);
		}
		else
		{
			MessageBoxA(
				NULL,
				xorstr_("Login Unsucessfull"),
				xorstr_("Auth"),
				MB_OK
			);
			exit(0);
		}
	}
	else
	{
		utils::rename_myself();
		MessageBoxA(
			NULL,
			xorstr_("Please run this program as admin"),
			xorstr_("Alert"),
			MB_OK
		);
		exit(0);
	}
}