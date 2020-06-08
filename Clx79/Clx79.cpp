// Clx79.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h>
#include <boost/program_options.hpp>
#include "Graphics/Graphics.h"
#include "Interface/Target.h"

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

void upscale_picture(pixel *&image, pixel *&render, const int &width, const int &logical_height, const int &physical_height) {
	// TODO need to calculate the aspect ratio, but now we are assuming that:
	// horizontal is fixed
	// vertical physical is delta + two times logical vertical (480 - (200 * 2) = 80)
	int v_upscale = logical_height  * 2;
	int v_band = (physical_height - v_upscale) / 2;

	int ys = 0;
	for (int y = v_band; y < v_band + v_upscale; y += 2, ys++) {
		for (int x = 0; x < width; x++) {
			render[x + y * width] = image[x + ys * width];
			render[x + (y + 1) * width] = image[x + ys * width];
		}
	}
}

void make_picture_stripes(pixel* a_picture, int width, int height) {
	bool green = false;
	for (int row = 0; row < height; row++) {
		int new_row = row % 8;
		if (new_row == 0) {
			green = !green;
		}
		for (int column = 0; column < width; column++) {
			a_picture[row * width + column] = (green ? 0xff00ff00 : 0xff000000);
		}
	}
}

void make_picture_checkers(pixel* a_picture, int width, int height) {
	bool green = false;
	for (int row = 0; row < height; row++) {
		int new_row = row % 8;
		if (new_row == 0) {
			green = !green;
		}
		int new_column = 0;
		bool r_green = green;
		for (int column = 0; column < width; column++, new_column++) {
			if (new_column >= 8) {
				new_column = 0;
				r_green = !r_green;
			}
			a_picture[row * width + column] = (r_green ? 0xff00ff00 : 0xff000000);
		}
	}
}

void run_engine(const int &no_threads, const int &width, const int &logical_height, const int &physical_height, pixel *&render, Target *target, const bool &log_threads, const bool &log_total) {
	pixel* image = new pixel[width * logical_height];
	make_picture_blank(image, width, logical_height);
	bool quit = false;
	bool allClosed = false;
	int frames = 0;
	while (!quit) {
		make_picture_checkers(image, width, logical_height);
		upscale_picture(image, render, width, logical_height, physical_height);

		target->loop(quit, frames, allClosed);
	}
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
	run_engine(1, width, logical_height, physical_height, render, target, false, false);

	if (!auto_quit) {
		std::cin.clear();
		std::cout << "Press any key to exit \n";
#pragma warning( push )
#pragma warning( disable : 6031)
		_getch();
#pragma warning( pop )
	}

	return 0;
}
