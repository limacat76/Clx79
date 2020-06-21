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
#include <SDL.h>

namespace Host {
	namespace Renderer {

		class Target {
		public:
			virtual void loop(bool &quit, int &frames, bool &allClosed) = 0;

			virtual void stop() = 0;

			void set_auto_continue(bool var);

		protected:
			bool auto_continue = false;
		};

		class Headless : public Target {
		public:
			void loop(bool &quit, int &frames, bool &allClosed);

			void stop();
		};

		class SDLTarget : public Target {
		protected:
			SDL_Window* window;
			SDL_Renderer* renderer;
			SDL_Texture* texture;
			SDL_Event event;
			int remainingFrames = 60;
			bool copy = true;
			uint32_t* pixels;
			int width;
			int height;
			bool full_screen;

		public:
			SDLTarget(uint32_t* pixels, int width, int height, bool full_screen);

			~SDLTarget();

			void loop(bool &quit, int &frames, bool &allClosed);

			void stop();
		};

	}
}