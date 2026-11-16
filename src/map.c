#include "map.h"
#include "keyboardinput.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>



void RandBiome(MapStruct* map) {
	BiomeStruct Woods = {
	.id = WOODS,
	.name = "Лес",
	.obstruction_chance = 20,
	.stamina_rate = 1,
	.gold_rate = 1,
	.cell_empty = CELL_default_empty
	};

	BiomeStruct Swamp = {
	.id = SWAMP,
	.name = "Топь",
	.obstruction_chance = 25,
	.stamina_rate = 2,
	.gold_rate = 2,
	.cell_empty = CELL_swamp_empty
	};

	BiomeStruct Sparse = {
	.id = SPARSE,
	.name = "Редколесье",
	.obstruction_chance = 10,
	.stamina_rate = 1,
	.gold_rate = 1, 
	.cell_empty = CELL_default_empty
	};

	const BiomeStruct Sequoia = {
	.id = SEQUOIA,
	.name = "Секвойи",
	.obstruction_chance = 15,
	.stamina_rate = 1,
	.gold_rate = 2,
	.cell_empty = CELL_default_empty
	};
	int instance = rand() % BIOME_COUNT ;
	switch (instance) {
	case WOODS: map->biome = Woods; return;
	case SWAMP: map->biome = Swamp; return;
	case SPARSE: map->biome = Sparse; return;
	case SEQUOIA: map->biome = Sequoia; return;
	default: map->biome = Woods; 
	}
}

void FillMap(MapStruct* map) {
	if (map->biome.id < WOODS || map->biome.id > BIOME_COUNT-1) {
		printf("Биом не задан!\n"); Sleep(1000); return;
	}

	if (map->biome.id == SEQUOIA) {
		for (int i = 0; i < map->size; i++) {
			for (int j = 0; j < map->size; j++) {
				if (map->map_array[i][j] == CELL_tree) continue;
				int isObstruction = rand() % 100;
				if (isObstruction < map->biome.obstruction_chance) {
					map->map_array[i][j] = CELL_tree;
					if (i + 1 < map->size) map->map_array[i + 1][j] = CELL_tree;
					if (j + 1 < map->size) map->map_array[i][j + 1] = CELL_tree;
					if (i + 1 < map->size && i + 1 < map->size) map->map_array[i + 1][j + 1] = CELL_tree;
				}
				else map->map_array[i][j] = map->biome.cell_empty;
			}
		}
	}

	else {
		for (int i = 0; i < map->size; i++) {
			for (int j = 0; j < map->size; j++) {
				int isObstruction = rand() % 100;
				if (isObstruction < map->biome.obstruction_chance) map->map_array[i][j] = CELL_tree;
				else map->map_array[i][j] = map->biome.cell_empty;
			}
		}
	}
	map->map_array[rand() % map->size][rand() % map->size] = CELL_campfire;
}



void GenerateMap(MapStruct* map) {
	if (map == NULL) { printf("Указатель на MapStruct равен NULL"); return; }
	map->size = rand() % 25 + 5;
	map->map_array = (char**)malloc(map->size * sizeof(char*));
	for (int i = 0; i < map->size; i++) {
		map->map_array[i] = (char*)malloc(map->size * sizeof(char));
	}
	RandBiome(map);
	FillMap(map);
}

void DeleteMap(MapStruct* map) {
	for (int i = 0; i < map->size; i++) {
		free(map->map_array[i]);
		printf("Память по адресу %p освобождена\n", &map->map_array[i]);
		Sleep(100);
	}
	free(map->map_array);
	printf("Память по адресу %p освобождена\n", &map->map_array);
}

void PrintMap(MapStruct map, DWORD sleep) {
	for (int i = 0; i < map.size; i++) {
		for (int j = 0; j < map.size; j++)
			printf("%c", map.map_array[i][j]);
		Sleep(sleep);
		printf("\n");
	}
}

void GetRandomCoords(int* ptr_x, int* ptr_y, MapStruct map) {
	do {
		*ptr_x = rand() % map.size,
			*ptr_y = rand() % map.size;
	} while (map.map_array[*ptr_y][*ptr_x] == CELL_tree);
}

void PrintBottomBar(MapStruct map, PlayerInstance Player, HANDLE console) {
	COORD bottomBarCoords = { 0, map.size + 1 };
	SetConsoleCursorPosition(console, bottomBarCoords);
	printf("Здоровье: %d/%d | Выноосливость: %d/%d | Золото: %d | Время: %d | Дальность от дома: %d |Биом: %s\nДля выхода нажмите Q у костра(^)", \
		Player.hp, Player.max_hp, Player.stamina, Player.max_stamina, Player.gold, Player.current_hour, map.home_range, map.biome.name);
}



void Journey(PlayerInstance* ptr_Player, HANDLE console) {
	if (ptr_Player == NULL) { printf("Указатель на PlayerInstance равен NULL"); return; }
	COORD buffer = { 120, 40 };
	SetConsoleScreenBufferSize(console, buffer);
	MapStruct map;
	GenerateMap(&map);
	if (&map == NULL) { printf("Указатель на MapStruct равен NULL"); return; }
	map.isMapWithPortal = 0;
	map.loop_count = 0;
	map.home_range = 0;
	int next_x, next_y;
	GetRandomCoords(&next_x, &next_y, map);
	ptr_Player->PlayerCoord.X = next_x;
	ptr_Player->PlayerCoord.Y = next_y;
	PrintMap(map, 50);
	PrintBottomBar(map, *ptr_Player, console);
	SetConsoleCursorPosition(console, ptr_Player->PlayerCoord);
	printf("%c", CELL_player);
	while (!(JourneyKeyboardInput(&next_x, &next_y) == LOW_Q && map.map_array[ptr_Player->PlayerCoord.Y][ptr_Player->PlayerCoord.X] == CELL_campfire)) {
		if (ptr_Player->stamina == 0 || ptr_Player->current_hour == 2) {
			ClearConsole(console);
			ptr_Player->gold = ptr_Player->gold / 2;
			printf("Вы упали в обморок! Вас спасли, но забрали половину золота (%d)\n", ptr_Player->gold);
			ptr_Player->stamina = ptr_Player->max_stamina;
			ptr_Player->hp = ptr_Player->max_hp;
			DeleteMap(&map);
			return;
		}

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
			PrintMap(map, 0);
			PrintBottomBar(map, *ptr_Player, console);
			map.isMapWithPortal = 1;
		}
		if (map.map_array[next_y][next_x] == CELL_portal) {
			map.home_range++;
			ptr_Player->stamina -= 5;
			ptr_Player->current_hour += 1;
			ptr_Player->gold += GOLD_RATE * map.home_range * map.biome.gold_rate;
			ClearConsole(console);
			DeleteMap(&map);
			printf("Создание новой карты");
			for (int i = 0; i < 4; i++) {
				Sleep(500);
				printf(".");
			}
			ClearConsole(console);
			GenerateMap(&map);
			map.loop_count = 0;
			map.isMapWithPortal = 0;
			GetRandomCoords(&next_x, &next_y, map);
			ptr_Player->PlayerCoord.X = next_x;
			ptr_Player->PlayerCoord.Y = next_y;
			PrintMap(map, 50);
			PrintBottomBar(map, *ptr_Player, console);
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
		ptr_Player->stamina -= 1 * map.biome.stamina_rate;
		PrintBottomBar(map, *ptr_Player, console);

	}
	ClearConsole(console);
	DeleteMap(&map);
	printf("Прикючение закончено!\n");

}

