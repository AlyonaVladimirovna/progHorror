#pragma once

//Шанс появления препятствия на карте
#define OBSTRUCTION_CHANCE 15
//Сколько колец надо навернуть для того чтобы появился портал
#define MAP_LOOP_CONST 3

enum MAPCELLS {
	CELL_empty = '.',
	CELL_tree = 'T',
	CELL_portal = 'P',
	CELL_campfire = '^',
	CELL_player = '@'
};


char** GenerateMap(int* ptr_size);

void DeleteMap(char** map, int size);

void PrintMap(char** map, int size);

void GetRandomCoords(int* ptr_x, int* ptr_y, int size, char** map);

