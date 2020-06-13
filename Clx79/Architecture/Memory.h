#pragma once
#include <stdint.h>
#include <vector>

typedef uint8_t byte;

struct Patch
{
	size_t destination;
	std::vector<byte> content;
};

void patch(byte* target, Patch patch);

void clear(byte* target, byte content, size_t size);