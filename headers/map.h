#pragma once
#include "player.h"
#include "inventory.h"
#include <curses.h>
//#include <windows.h>

#define BIOME_COUNT 4
#define BIOME_NAME_LEN 64
//Сколько колец надо навернуть для того чтобы появился портал
#define MAP_LOOP_CONST 3
#define GOLD_RATE 5
#define PORTAL_STAMINA_COST 5
#define INFO_AREA_Y_SIZE 10
#define INFO_AREA_X_SIZE 30


enum BIOME_ID {
	WOODS,
	SWAMP,
	SPARSE,
	SEQUOIA
	};

enum MAPCELLS {
	CELL_default_empty = '.',
	CELL_tree = 'T',
	CELL_portal = 'P',
	CELL_campfire = '^',
	CELL_player = '@',
	CELL_swamp_empty = '~'
};


typedef struct {
	enum BIOME_ID id;
	char name[BIOME_NAME_LEN];
	int obstruction_chance;
	int stamina_rate;
	int gold_rate;
	enum MAPCELLS cell_empty;
}BiomeStruct;

typedef struct {
	char** map_array;
	int size;
	int home_range;
	int loop_count;
	int isMapWithPortal;
	BiomeStruct biome;
}MapStruct;

void RandBiome(MapStruct* map);

void update_areas(WINDOW* map_area, WINDOW* info_area);

void FillMap(MapStruct* map);

void GenerateMap(MapStruct* map);

void DeleteMap(MapStruct* map);

void PrintMap(MapStruct map, int sleep);

void GetRandomCoords(int* ptr_x, int* ptr_y, MapStruct map);

void PrintBottomBar(MapStruct map, PlayerInstance* Player);

void Journey(PlayerInstance* ptr_Player, ItemsDict Item_struct[]);