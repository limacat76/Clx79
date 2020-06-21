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
#include "Renderer.h"

namespace Host {
	namespace Renderer {

		void Target::set_auto_continue(bool var) {
			auto_continue = var;
		}

		void Headless::loop(bool &quit, int &frames, bool &allClosed) {
			if (allClosed) {
				quit = true;
			}
		}

		void Headless::stop() {
			;
		}


		SDLTarget::SDLTarget(uint32_t* in_pixels, int in_width, int in_height, bool full_screen) : pixels(in_pixels), width(in_width), height(in_height), full_screen(full_screen) {
			SDL_Init(SDL_INIT_VIDEO);
			window = SDL_CreateWindow("Clx79", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
			if (full_screen) {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			}
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
		}

		SDLTarget::~SDLTarget() {

		};

		void SDLTarget::stop() {
			SDL_DestroyTexture(texture);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		void SDLTarget::loop(bool &quit, int &frames, bool &allClosed) {
			if (copy) {
				SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(uint32_t));
			}

			if (!allClosed || remainingFrames > 0) {
				if (allClosed) {
					remainingFrames--;
					if (allClosed && auto_continue) {
						quit = true;
					}
				}
				while (SDL_PollEvent(&event) != 0) {
					switch (event.type) {
					case SDL_QUIT:
						quit = true;
						break;
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) {
							quit = true;
						}
						else if (event.key.keysym.sym == SDLK_x) {
							copy = !copy;
						}
						break;
					}
				}
				frames++;
			}
			else {
				SDL_WaitEvent(&event);
				switch (event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						quit = true;
					}
					else if (event.key.keysym.sym == SDLK_x) {
						copy = !copy;
					}
					break;
				}
			}
			if (copy) {
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);
			}
		}

	}
}