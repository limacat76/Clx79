#pragma once
#include "Architecture/Memory.h"
#include <vector>

std::vector<Patch> read_patches();

namespace memory_loader {

	void make_new_patch(const unsigned int destination, std::vector<Patch>& p);

	Patch* get_current_patch(std::vector<Patch>& p);

	void push_byte(const unsigned int value, std::vector<Patch>& p);

}

std::vector<Patch> manual_loader_test();