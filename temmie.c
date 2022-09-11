#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include "./temmie.h"
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define strerr strerror(errno)
uint32_t img_w;
uint32_t img_h;
SDL_Renderer *renderer;
SDL_Texture *texture;
uint32_t *pixels;
void display() {
	for (uint32_t x = 0; x < img_w; ++x)
	for (uint32_t y = 0; y < img_h; ++y) {
		size_t j = y*img_w+x;
		size_t i = j*8+16;
		pixels[j] = ((uint32_t)data_temmie[i+6] << 24) | ((uint32_t)data_temmie[i+0] << 16) | ((uint32_t)data_temmie[i+2] << 8) | ((uint32_t)data_temmie[i+4]);
		if (data_temmie[i+6] > 0x7f) {
		} else {
			pixels[j] = 0;
		}
	}
	SDL_UpdateTexture(texture, NULL, pixels, img_w*4);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
bool quitted = 0;
void quit() {
	if (quitted) return;
	quitted = 1;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	free(pixels);
	SDL_Quit();
}
int main(int argc, char* argv[]) {
	if (memcmp(data_temmie, "farbfeld", 8) != 0) return 1;
	img_w = (data_temmie[010] << 030) | (data_temmie[011] << 020) | (data_temmie[012] << 010) | data_temmie[013];
	img_h = (data_temmie[014] << 030) | (data_temmie[015] << 020) | (data_temmie[016] << 010) | data_temmie[017];
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		eprintf("error initialising SDL: %s\n", SDL_GetError()); return 1;
	}
	SDL_Window *win = SDL_CreateWindow("Temmie", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, img_w, img_h, 0);
	bool quit_ = 0;
   	renderer = SDL_CreateRenderer(win, -1, 0);
   	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, img_w, img_h);
	pixels = malloc(img_w * img_h * 4);
	if (!pixels) {
		eprintf("malloc: %s\n", strerr); return errno;
	}
	atexit(quit);
	display();
	SDL_Event event;
	while (!quit_) {
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
				quit_ = 1;
				break;
			default:
				display();
				break;
		}
	}
	return 0;
}
