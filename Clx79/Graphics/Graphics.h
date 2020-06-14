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
#pragma once
#include <string>
#include <stdint.h>

extern unsigned int render_color;

void set_render_color(int color);

const unsigned int GREEN = 0xff00ff00;

const unsigned int BLACK = 0xff000000;

const unsigned int AMBER = 0xfff08e34;

const unsigned int WHITE = 0xffffffff;

typedef uint32_t pixel;

typedef uint32_t color;

// needs to stay for testing new targets (DirectX, Vulkan, Mantle, File Output...)
void make_picture_noise(pixel* a_picture, int width, int height);

// needs to stay for testing new targets (DirectX, Vulkan, Mantle, File Output...)
void make_picture_test(pixel* a_picture, int width, int height);

// it's a dark grey screen
void make_picture_blank(pixel* a_picture, int width, int height);

// it's a completely black screen
void make_picture_black(pixel* a_picture, int width, int height);

// big blocky lines
void make_picture_stripes(pixel* a_picture, int width, int height);

// a checker pattern
void make_picture_checkers(pixel* a_picture, int width, int height);
