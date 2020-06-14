/**
MIT License

Copyright (c) 2020 Davide Inglima <limacat@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include "Memory.h"
#include <iostream>

void patch(byte* target, Patch patch) {
#ifdef DEBUG_PATCHING_MESSAGES
	std::cout << "applying patch to " << std::hex << patch.destination << " of size " << std::dec << patch.content.size() << std::endl;
#endif

	for (int x = 0; x < patch.content.size(); x++) {
#ifdef DEBUG_PATCHING_MESSAGES
		std::cout << "writing " << std::hex << patch.content[x] << " to " << std::hex << patch.destination + x << std::endl;
#endif

		target[patch.destination + x] = patch.content[x];
	}
}

void clear(byte* target, byte content, size_t size) {
	for (int x = 0; x < size; x++) {
		target[x] = content;
	}
}
