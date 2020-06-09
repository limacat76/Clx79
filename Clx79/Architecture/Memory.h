#pragma once
#include <stdint.h>

typedef uint8_t byte;

struct Patch
{
	size_t size;
	byte* content;
};

void patch(byte* target, Patch patch, size_t destination);

void clear(byte* target, byte content, size_t size);