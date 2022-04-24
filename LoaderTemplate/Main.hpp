#pragma once
#include "colors.hpp"
#include "utils.hpp"
#include "xorstr.hpp"
#include "driver_data.h"
#include "mapper/kdmapper.hpp"
#include <thread>
#include <cpr/cpr.h> // Requests Library

struct __process_info 
{
	bool running = true; // Use This For All Control Loops (Like A Kill Switch)
}; // Can Be Shared With Multiple Functions :)

struct UsersData
{
	std::string username{}, password{}, license_key{};
};

void protection_thread(struct __process_info *control_struct);