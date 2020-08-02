#include "grid.h"

#include <stdio.h>

// For rand() and srand(time(NULL));
#include <stdlib.h>
#include <time.h>

void randomize_cells() {
	srand(time(NULL));
	for (int y = 0; y < GRID_SIZE; ++y) {
		for (int x = 0; x < GRID_SIZE; ++x) {
			int r = rand();
			grid[y][x] = r > RAND_MAX/2;
		}		
	}
}

void update_cells() {
	int grid_temp[GRID_SIZE][GRID_SIZE] = { 0 };

	copy_grid(grid, grid_temp);

	for (int y = 0; y < GRID_SIZE; ++y) {
		for (int x = 0; x < GRID_SIZE; ++x) {
			if (x == 0 || y == 0) continue;
			
			int neighbours = get_neighbours(x, y);

			if (grid[y][x] == 1 && (neighbours < 2 || neighbours > 3)) {
				grid_temp[y][x] = 0;
			} else if (grid[y][x] == 0 && neighbours == 3) {
				grid_temp[y][x] = 1;
			}
		}	
	}

	copy_grid(grid_temp, grid);
}

void render_cells(SDL_Renderer *renderer, int paused) {
	for (int y = 0; y < GRID_SIZE; ++y) {
		if (y * CELL_SIZE < camera_y || y * CELL_SIZE > camera_y + 1200) continue; // Culling
		for (int x = 0; x < GRID_SIZE; ++x) {
			if (x * CELL_SIZE < camera_x || x * CELL_SIZE > camera_x + 1500) continue; // Culling
		
			if (grid[y][x] == 1) {
				if (!paused) {
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				} else {
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				}
				SDL_Rect rect = {(x - GRID_OFFSET) * CELL_SIZE + 1 - camera_x, (y - GRID_OFFSET) * CELL_SIZE + 1 - camera_y, CELL_SIZE - 1, CELL_SIZE - 1};
				SDL_RenderFillRect(renderer, &rect);
			}
		}		
	}
}

int get_neighbours(int x, int y) {
	int neighbours = 0;

	for (int sy = y - 1; sy < y+2; sy++) {
		for (int sx = x - 1; sx < x+2; sx++) {
			if (sx == x && sy == y) continue;

			if (grid[sy][sx] == 1) neighbours++;
		}
	}

	return neighbours;
}

void copy_grid(int src[GRID_SIZE][GRID_SIZE], int dest[GRID_SIZE][GRID_SIZE]) {
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			dest[i][j] = src[i][j];
		}	
	}
}