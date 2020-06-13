#include "Memory.h"

void patch(byte* target, Patch patch) {
	for (int x = 0; x < patch.size; x++) {
		target[patch.destination + x] = patch.content[x];
	}
}

void clear(byte* target, byte content, size_t size) {
	for (int x = 0; x < size; x++) {
		target[x] = content;
	}
}
