// Clx79.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h>
#include <boost/program_options.hpp>
#include "Graphics/Graphics.h"
#include "Interface/Target.h"
#include "Architecture/Memory.h"
#include "Architecture/Video.h"
#include "Bootstrap.h"

namespace options = boost::program_options;

#ifdef _DEBUG
void printDefines() {
	std::cout << "Additional defines go here \n";
}
#endif

template <typename T> T getParameter(options::variables_map value_map, std::string parameter_key) {
	T result = value_map[parameter_key].as<T>();
#ifdef _DEBUG
	std::cout << parameter_key << ":=" << result << "\n";
#endif
	return result;
}

void run_engine(const int &no_threads, const int &width, const int &logical_height, const int &physical_height,
	            pixel *&render, Target *target, const bool &log_threads, const bool &log_total, byte *&ram) {
	pixel* image = new pixel[width * logical_height];
	make_picture_blank(image, width, logical_height);
	bool quit = false;
	bool allClosed = false;
	int frames = 0;
	while (!quit) {
		display_ram(image, width, logical_height, ram, 0x0000, 0x1000);
		upscale_picture(image, render, width, logical_height, physical_height);

		target->loop(quit, frames, allClosed);
	}
	delete image;
}

int main(int argc, char *argv[])
{
#ifdef _DEBUG
	printDefines();
#endif

	// Declare the supported options.
	options::options_description desc("Allowed options");
	desc.add_options()
		("help", "produces the help message")
		("auto-quit", options::value<bool>()->default_value(true)->implicit_value(true), "set auto-quit for profiling")
		("engine", options::value<int>()->default_value(0), "set engine, 0 for Message, 1 for Checkers, 2 for Lines ")
		("target", options::value<int>()->default_value(0), "set target, 0 for SDL, 1 for Headless")
		("color", options::value<int>()->default_value(1), "set color, 0 for green, 1 for amber, 2 for white")
		;

	options::variables_map vm;
	options::store(options::parse_command_line(argc, argv, desc), vm);
	options::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
#pragma warning( push )
#pragma warning( disable : 6031)
		_getch();
#pragma warning( pop )
		return 1;
	}

	std::cout << "Hello World!\n";
	bool auto_quit = getParameter<bool>(vm, "auto-quit");
	int target_number = getParameter<int>(vm, "target");

	set_render_color(getParameter<int>(vm, "color"));

	int width = 640;
	int logical_height = 200;
	int physical_height = 480;
	pixel* render = new pixel[width * physical_height];
	make_picture_black(render, width, physical_height);
	Target* target;
	switch (target_number) {
	case 0:
		target = new SDLTarget(render, width, physical_height, false);
		break;
	case 1:
	default:
		target = new Headless();
		break;
	}
	target->set_auto_continue(false);

	byte* ram = new byte[4096 * 4];
	clear(ram, 0x05, 4096 * 4);

	std::vector<Patch> patches = read_patches();

	for (std::vector<Patch>::const_iterator i = patches.begin(); i != patches.end(); ++i) {
		patch(ram, (*i));
	}

	// TODO D.I Remove and change with the actual loader...
	patches = manual_loader_test();
	for (std::vector<Patch>::const_iterator i = patches.begin(); i != patches.end(); ++i) {
		patch(ram, (*i));
	}

	run_engine(1, width, logical_height, physical_height, render, target, false, false, ram);

	if (!auto_quit) {
		std::cin.clear();
		std::cout << "Press any key to exit \n";
#pragma warning( push )
#pragma warning( disable : 6031)
		_getch();
#pragma warning( pop )
	}

	delete render;
	delete target;
	delete ram;

	return 0;
}
