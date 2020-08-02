#pragma once

#define GRID_OFFSET 12 // Offset to the left and top of the screen.

#define GRID_SIZE 256
#define CELL_SIZE 16

#include <SDL2/SDL.h>

int grid[GRID_SIZE][GRID_SIZE];
int camera_x, camera_y, camera_spd;

void update_cells();
void render_cells(SDL_Renderer *renderer, int paused);

int get_neighbours();

void copy_grid(int src[GRID_SIZE][GRID_SIZE], int dest[GRID_SIZE][GRID_SIZE]);