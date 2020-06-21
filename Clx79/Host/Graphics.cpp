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
#include <random>
#include <limits>
#include "Graphics.h"

namespace Host {
	namespace Graphics {

		unsigned int render_color = GREEN;

		void set_render_color(int color) {
			switch (color) {
			case 0:
				render_color = GREEN;
				break;
			case 1:
				render_color = AMBER;
				break;
			case 2:
				render_color = WHITE;
				break;
			default:
				render_color = GREEN;
				break;
			}
		}

		void make_picture_noise(pixel* a_picture, int width, int height) {
			std::random_device rd;
			std::mt19937_64 eng(rd());
			std::uniform_int_distribution<pixel> distr;
			const pixel all_alpha = 0xff000000;

			for (int row = 0; row < height; row++) {
				for (int column = 0; column < width; column++) {
					a_picture[row * width + column] = distr(eng) | all_alpha;
				}
			}
		}

		void make_picture_test(pixel* a_picture, int width, int height) {
			for (int row = 0; row < height; row++) {
				for (int column = 0; column < width; column++) {
					a_picture[row * width + column] = ((row % 255) << 16) + ((column % 255) << 8) + ((row + column) % 255);
				}
			}
		}

		void make_picture_blank(pixel* a_picture, int width, int height) {
			for (int row = 0; row < height; row++) {
				for (int column = 0; column < width; column++) {
					a_picture[row * width + column] = 0xff999999;
				}
			}
		}

		void make_picture_black(pixel* a_picture, int width, int height) {
			for (int row = 0; row < height; row++) {
				for (int column = 0; column < width; column++) {
					a_picture[row * width + column] = 0xff000000;
				}
			}
		}

		void make_picture_stripes(pixel* a_picture, int width, int height) {
			bool green = false;
			for (int row = 0; row < height; row++) {
				int new_row = row % 8;
				if (new_row == 0) {
					green = !green;
				}
				for (int column = 0; column < width; column++) {
					a_picture[row * width + column] = (green ? render_color : BLACK);
				}
			}
		}

		void make_picture_checkers(pixel* a_picture, int width, int height) {
			bool green = false;
			for (int row = 0; row < height; row++) {
				int new_row = row % 8;
				if (new_row == 0) {
					green = !green;
				}
				int new_column = 0;
				bool r_green = green;
				for (int column = 0; column < width; column++, new_column++) {
					if (new_column >= 8) {
						new_column = 0;
						r_green = !r_green;
					}
					a_picture[row * width + column] = (r_green ? render_color : BLACK);
				}
			}
		}

	}
}