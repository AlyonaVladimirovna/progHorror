#include "map.h"
#include "keyboardinput.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void GenerateMap(MapStruct* map) {
	map->size = rand() % 25 + 5;
	map->map_array = (char**)malloc(map->size * sizeof(char*));
	for (int i = 0; i < map->size; i++) {
		map->map_array[i] = (char*)malloc(map->size * sizeof(char));
	}

	for (int i = 0; i < map->size; i++) {
		for (int j = 0; j < map->size; j++) {
			int isObstruction = rand() % 100;
			if (isObstruction < OBSTRUCTION_CHANCE) map->map_array[i][j] = CELL_tree;
			else map->map_array[i][j] = CELL_empty;
		}
	}
	map->map_array[rand() % map->size][rand() % map->size] = CELL_campfire;
}

void DeleteMap(MapStruct* map) {
	for (int i = 0; i < map->size; i++)
		free(map->map_array[i]);
	free(map->map_array);
}

void PrintMap(MapStruct map) {
	for (int i = 0; i < map.size; i++) {
		for (int j = 0; j < map.size; j++)
			printf("%c", map.map_array[i][j]);
		printf("\n");
	}
}

void GetRandomCoords(int* ptr_x, int* ptr_y, MapStruct map) {
	do {
		*ptr_x = rand() % map.size,
			*ptr_y = rand() % map.size;
	} while (map.map_array[*ptr_y][*ptr_x] == CELL_tree);
}



void Journey(PlayerInstance* ptr_Player, HANDLE console) {
	MapStruct map;
	GenerateMap(&map);
	map.isMapWithPortal = 0;
	map.loop_count = 0;
	map.home_range = 0;
	int next_x, next_y;
	GetRandomCoords(&next_x, &next_y, map);
	ptr_Player->PlayerCoord.X = next_x;
	ptr_Player->PlayerCoord.Y = next_y;
	PrintMap(map);
	printf("Для выхода нажмите Q у костра (^)");
	SetConsoleCursorPosition(console, ptr_Player->PlayerCoord);
	printf("%c", CELL_player);
	while (!(JourneyKeyboardInput(&next_x, &next_y) == LOW_Q && map.map_array[ptr_Player->PlayerCoord.Y][ptr_Player->PlayerCoord.X] == CELL_campfire)) {
		if (next_y < 0) { next_y = map.size - 1; map.loop_count++; }
		else if (next_y > map.size - 1) {
			next_y = 0; map.loop_count++;
		}
		if (next_x > map.size - 1) {
			next_x = 0; map.loop_count++;
		}
		else if (next_x < 0) { next_x = map.size - 1; map.loop_count++; }
		if (map.map_array[next_y][next_x] == CELL_tree) {
			next_x = ptr_Player->PlayerCoord.X;
			next_y = ptr_Player->PlayerCoord.Y;
			continue;
		}
		if (map.loop_count > MAP_LOOP_CONST && !map.isMapWithPortal) {
			int randX, randY;
			GetRandomCoords(&randX, &randY, map);
			map.map_array[randY][randX] = CELL_portal;
			ClearConsole(console);
			PrintMap(map);
			printf("Для выхода нажмите Q у костра (^)");
			map.isMapWithPortal = 1;
		}
		if (map.map_array[next_y][next_x] == CELL_portal) {
			map.home_range++;
			ClearConsole(console);
			DeleteMap(&map);
			GenerateMap(&map);
			map.loop_count = 0;
			map.isMapWithPortal = 0;
			GetRandomCoords(&next_x, &next_y, map);
			ptr_Player->PlayerCoord.X = next_x;
			ptr_Player->PlayerCoord.Y = next_y;
			PrintMap(map);
			printf("Дальность от дома: %d\n", map.home_range);
			printf("Для выхода нажмите Q у костра (^)");
			SetConsoleCursorPosition(console, ptr_Player->PlayerCoord);
			printf("%c", CELL_player);
			continue;
		}

		SetConsoleCursorPosition(console, ptr_Player->PlayerCoord);
		printf("%c", map.map_array[ptr_Player->PlayerCoord.Y][ptr_Player->PlayerCoord.X]);
		ptr_Player->PlayerCoord.X = next_x;
		ptr_Player->PlayerCoord.Y = next_y;
		SetConsoleCursorPosition(console, ptr_Player->PlayerCoord);
		printf("%c", CELL_player);
	}
	ClearConsole(console);
	DeleteMap(&map);
	printf("Прикючение закончено!\n");

}

