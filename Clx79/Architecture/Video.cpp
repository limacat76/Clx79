#include "Video.h"

void upscale_picture(pixel *&image, pixel *&render, const int &width, const int &logical_height, const int &physical_height) {
	// TODO D.I we need to calculate the aspect ratio, but now we are assuming that:
	// horizontal is fixed
	// vertical physical is delta + two times logical vertical (480 - (200 * 2) = 80)
	int v_upscale = logical_height * 2;
	int v_band = (physical_height - v_upscale) / 2;

	int ys = 0;
	for (int y = v_band; y < v_band + v_upscale; y += 2, ys++) {
		for (int x = 0; x < width; x++) {
			render[x + y * width] = image[x + ys * width];
			render[x + (y + 1) * width] = image[x + ys * width];
		}
	}
}

void display_ram(pixel* a_picture, int width, int height, byte* ram, int screen_ram, int character_rom) {
	for (int row = 0; row < height; row++) {
		for (int column = 0; column < width; column += 8) {
			byte selected_character = ram[screen_ram + (row / 8 * 80) + (column / 8)];
			byte line = ram[character_rom + row % 8 + selected_character * 8];
			for (int r_y = 7; r_y >= 0; r_y--) {
				bool on = (line & 0x01) == 0x01;
				a_picture[row * width + column + r_y] = (on ? render_color : BLACK);
				line = line >> 1;
			}
		}
	}
}