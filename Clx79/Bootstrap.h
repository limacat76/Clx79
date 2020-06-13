#pragma once
#include "Architecture/Memory.h"
#include <string>
#include <vector>

namespace memory_loader {

	void make_new_patch(const unsigned int destination, std::vector<Patch>& p);

	Patch* get_current_patch(std::vector<Patch>& p);

	void push_byte(const unsigned int value, std::vector<Patch>& p);

}

std::vector<Patch> file_loader_test(std::string filename);