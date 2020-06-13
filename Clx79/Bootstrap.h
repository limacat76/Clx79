#pragma once
#include "Architecture/Memory.h"
#include <vector>

std::vector<Patch> read_patches();

namespace memory_loader {

	void make_new_patch();

	Patch get_current_patch();

	void push_byte();

}

void manual_loader_test();