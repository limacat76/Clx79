#pragma once
#include "../Graphics/Graphics.h"
#include "Memory.h"

void upscale_picture(pixel *&image, pixel *&render, const int &width, const int &logical_height, const int &physical_height);

void display_ram(pixel* a_picture, int width, int height, byte* ram, int screen_ram, int character_rom);