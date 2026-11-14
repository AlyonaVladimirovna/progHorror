#pragma once
#include "player.h"
#include <windows.h>

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

typedef struct {
	char** map_array;
	int size;
	int home_range;
	int loop_count;
	int isMapWithPortal;
}MapStruct;

void GenerateMap(MapStruct* map);

void DeleteMap(MapStruct* map);

void PrintMap(MapStruct* map);

void GetRandomCoords(int* ptr_x, int* ptr_y, MapStruct* map);

void Journey(PlayerInstance* ptr_Player, HANDLE console);