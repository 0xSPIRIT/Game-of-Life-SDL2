#define SDL_MAIN_HANDLED

#define SIMULATION_STEP 8

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "grid.h"

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	// Rectangle around the borders of the usable area.
	SDL_Rect rect = {1, 1, GRID_SIZE * CELL_SIZE - 1, GRID_SIZE * CELL_SIZE - 1};

	// Add a simple glider by default.
	grid[160][179] = 1;
	grid[161][180] = 1;
	grid[162][180] = 1;
	grid[162][179] = 1;
	grid[162][178] = 1;

	int running = 1;
	int paused = 1;

	// Set camera to the middle of the usable area.
	camera_x = (GRID_SIZE / 2) * CELL_SIZE;
	camera_y = (GRID_SIZE / 2) * CELL_SIZE;
	camera_spd = 9;

	int frames = SIMULATION_STEP;
	while (running) {
		SDL_Event event;
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_MOUSEBUTTONDOWN: {
					// Get the mouse position.
					int mouse_x, mouse_y;
					SDL_GetMouseState(&mouse_x, &mouse_y);
					// Convert it to world coordinates.
					mouse_x += camera_x;
					mouse_y += camera_y;

					// Convert mouse position to grid position.
					int mx = floor(mouse_x / (float) CELL_SIZE) + GRID_OFFSET;
					int my = floor(mouse_y / (float) CELL_SIZE) + GRID_OFFSET;

					// Set the cell to alive or dead based on mouse input.
					if (event.button.button == SDL_BUTTON_LEFT) {
						grid[my][mx] = 1;
					} else if (event.button.button == SDL_BUTTON_RIGHT) {
						grid[my][mx] = 0;
					}
					break;
				}
				case SDL_KEYDOWN: {
					if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) {
						// Pause the simulation, make sure there's no delay to playing the sim, by setting frames = 0
						paused = !paused;
						frames = 0;
					} else if (event.key.keysym.sym == SDLK_r) {
						// Reset the simulation- set camera to center of usable area, and reset grid to be empty.
						camera_x = camera_y = (GRID_SIZE / 2) * CELL_SIZE;
						int nothing[GRID_SIZE][GRID_SIZE] = {0};
						copy_grid(nothing, grid);
					}
					break;	
				}
			}
		}

		// Clear the screen with a black colour.
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw outline around usable area.

		rect.x = -camera_x;
		rect.y = -camera_y;
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &rect);

		// Don't simulate if paused.
		
		if (!paused) {
			if (frames == 0) {
				update_cells();		
				frames = SIMULATION_STEP;
			} else {
				frames--;
			}
		}

		// Move the camera based on WASD.
		
		const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
		if (keyboard_state[SDL_SCANCODE_D]) {
			camera_x += camera_spd;
		}
		if (keyboard_state[SDL_SCANCODE_A]) {
			camera_x -= camera_spd;
		}
		if (keyboard_state[SDL_SCANCODE_S]) {
			camera_y += camera_spd;
		}
		if (keyboard_state[SDL_SCANCODE_W]) {
			camera_y -= camera_spd;
		}

		// Constrain the camera to the usable area.
		if (camera_y < 0) {
			camera_y = 0;
		}
		if (camera_x < 0) {
			camera_x = 0;
		}
		if (camera_x > GRID_SIZE * CELL_SIZE - WINDOW_WIDTH) {
			camera_x = GRID_SIZE * CELL_SIZE - WINDOW_WIDTH;
		}
		if (camera_y > GRID_SIZE * CELL_SIZE - WINDOW_HEIGHT) {
			camera_y = GRID_SIZE * CELL_SIZE - WINDOW_HEIGHT;
		}

		
		render_cells(renderer, paused);

		// Render everything to the screen.
		SDL_RenderPresent(renderer);
	}

	// Free stuff.
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}