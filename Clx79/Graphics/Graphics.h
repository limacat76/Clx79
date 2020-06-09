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
