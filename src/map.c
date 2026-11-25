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
	.obstruction_chance = 10,
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

void update_areas(WINDOW* map_area, WINDOW* info_area) {
	wnoutrefresh(map_area);
	wnoutrefresh(info_area);
	doupdate();
}

void FillMap(MapStruct* map) {
	if (map->biome.id < WOODS || map->biome.id > BIOME_COUNT-1) {
		printw("Биом не задан!\n"); refresh(); napms(1000); return;
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
					if (i + 1 < map->size && j + 1 < map->size) map->map_array[i + 1][j + 1] = CELL_tree;
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
	if (map == NULL) { printw("Указатель на MapStruct равен NULL"); refresh(); return; }
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
		wprintw(stdscr, "Память по адресу %p освобождена\n", &map->map_array[i]);
		wrefresh(stdscr);
		napms(100);
	}
	free(map->map_array);
	wprintw(stdscr, "Память по адресу %p освобождена\n", &map->map_array);
	wrefresh(stdscr);
}

void PrintMap(MapStruct map, int sleep, WINDOW* map_area) {
	for (int i = 0; i < map.size; i++) {
		for (int j = 0; j < map.size; j++)
			mvwaddch(map_area, i, j, map.map_array[i][j]);
		wrefresh(map_area);
	}
}

void GetRandomCoords(int* ptr_x, int* ptr_y, MapStruct map) {
	do {
		*ptr_x = rand() % map.size,
			*ptr_y = rand() % map.size;
	} while (map.map_array[*ptr_y][*ptr_x] == CELL_tree);
}

void PrintBottomBar(MapStruct map, PlayerInstance Player, WINDOW* info_area, int isMapSaved) {	
	W_EraseRefresh(info_area);
	wprintw(info_area, "Здоровье: %d/%d\nВыносливость: %d/%d\nЗолото: %d\nВремя: %d:00\nДальность от дома: %d\nБиом: %s\n", \
		Player.hp, Player.max_hp, Player.stamina, Player.max_stamina, Player.gold, Player.current_hour, map.home_range, map.biome.name);
	if(isMapSaved) wprintw(info_area, "Карта записана!\n");
	else wprintw(info_area, "M - Записать карту\n");
	wprintw(info_area, "Q - Выход у костра (^)");
}


void Journey(PlayerInstance* ptr_Player, const InventoryItems Item_struct[]) {
	if (ptr_Player == NULL) { printw("Указатель на PlayerInstance равен NULL"); return; }
	nodelay(stdscr, TRUE);
	MapStruct map;
	GenerateMap(&map);
	if (&map == NULL) { printw("Указатель на MapStruct равен NULL"); return; }

	WINDOW* info_area = newwin(INFO_AREA_Y_SIZE, INFO_AREA_X_SIZE, 0, map.size + 3);
	WINDOW* map_area = newwin(map.size, map.size, 0, 0);
	map.isMapWithPortal = 0;
	map.loop_count = 0;
	map.home_range = 0;
	int next_x, next_y, key;
	int isMapSavedForLoop = 0;
	GetRandomCoords(&next_x, &next_y, map);
	ptr_Player->coordX = next_x;
	ptr_Player->coordY = next_y;

	PrintMap(map, 50, map_area);
	PrintBottomBar(map, *ptr_Player, info_area, isMapSavedForLoop);
	mvwprintw(map_area, ptr_Player->coordY, ptr_Player->coordX, "%c", CELL_player);
	update_areas(map_area, info_area);
	while (!((key = JourneyKeyboardHandler(&next_x, &next_y) )== LOW_Q && map.map_array[ptr_Player->coordY][ptr_Player->coordX] == CELL_campfire)) {

		if (ptr_Player->stamina <= 0 || ptr_Player->current_hour == 2) {
			delwin(map_area);
			delwin(info_area);
			EraseRefresh();
			ptr_Player->gold = ptr_Player->gold / 2;
			printw("Вы упали в обморок! Вас спасли, но забрали половину золота (%d)\n", ptr_Player->gold);
			ptr_Player->stamina = ptr_Player->max_stamina;
			ptr_Player->hp = ptr_Player->max_hp;
			DeleteMap(&map);
			return;
		}


		if (key == NULL) { wrefresh(map_area); napms(50); continue; } //!!!ВЕСЬ РЕНДЕР ВЫШЕ!!!

		if (next_y < 0) { next_y = map.size - 1; map.loop_count++; }
		else if (next_y > map.size - 1) {
			next_y = 0; map.loop_count++;
		}
		if (next_x > map.size - 1) {
			next_x = 0; map.loop_count++;
		}
		else if (next_x < 0) { next_x = map.size - 1; map.loop_count++; }
		if (map.map_array[next_y][next_x] == CELL_tree) {
			next_x = ptr_Player->coordX;
			next_y = ptr_Player->coordY;
			continue;
		}
		if (map.loop_count > MAP_LOOP_CONST && !map.isMapWithPortal) {
			int randX, randY;
			GetRandomCoords(&randX, &randY, map);
			map.map_array[randY][randX] = CELL_portal;
			W_EraseRefresh(map_area);
			PrintMap(map, 0, map_area);
			PrintBottomBar(map, *ptr_Player, info_area, isMapSavedForLoop);
			update_areas(map_area, info_area);
			map.isMapWithPortal = 1;
		}
		if (map.map_array[next_y][next_x] == CELL_portal) {
			map.home_range++;
			isMapSavedForLoop = 0;
			ptr_Player->stamina -= PORTAL_STAMINA_COST;
			ptr_Player->current_hour += 1;
			ptr_Player->gold += GOLD_RATE * map.home_range * map.biome.gold_rate;
			W_EraseRefresh(map_area);
			W_EraseRefresh(info_area);
			DeleteMap(&map);
			wprintw(stdscr, "Создание новой карты"); wrefresh(stdscr);
			for (int i = 0; i < 4; i++) {
				Sleep(500);
				wprintw(stdscr, "."); wrefresh(stdscr);
			}
			W_EraseRefresh(stdscr);
			delwin(map_area);
			delwin(info_area);
			GenerateMap(&map);
			map.loop_count = 0;
			map.isMapWithPortal = 0;
			GetRandomCoords(&next_x, &next_y, map);
			ptr_Player->coordX = next_x;
			ptr_Player->coordY = next_y;
			info_area = newwin(INFO_AREA_Y_SIZE, INFO_AREA_X_SIZE, 0, map.size + 3);
			map_area = newwin(map.size, map.size, 0, 0);
			PrintMap(map, 50, map_area); 
			PrintBottomBar(map, *ptr_Player, info_area, isMapSavedForLoop); 
			mvwprintw(map_area, ptr_Player->coordY, ptr_Player->coordX, "%c", CELL_player);
			update_areas(map_area, info_area);
			continue;
		}
		if(key == LOW_M) {
			PrintBottomBar(map, *ptr_Player, info_area, isMapSavedForLoop);
			if (isMapSavedForLoop) mvwprintw(info_area, 8, 0, "Вы уже записали карту для данной местности\n");
			else {
				isMapSavedForLoop = 1;
				int map_slot = FindItemSlot(Item_struct[MAP].id, ptr_Player->inventory);
				if (map_slot == -1) {
					map_slot = FindEmptySlot(ptr_Player->inventory, Item_struct);
					if ( map_slot == -1) {
						mvwprintw(info_area, 8, 0, "Нет места в инвентаре!");
					}
					else {
						ptr_Player->inventory[map_slot] = Item_struct[MAP];
						ptr_Player->inventory[map_slot].count += 1;
						
					}
				}
				else { ptr_Player->inventory[map_slot].count += 1; }
				wrefresh(info_area);
				continue;
			}
		}
		if (key != NULL) {
			mvwprintw(map_area, ptr_Player->coordY, ptr_Player->coordX, "%c", map.map_array[ptr_Player->coordY][ptr_Player->coordX]);
			ptr_Player->coordX = next_x;
			ptr_Player->coordY = next_y;
			mvwprintw(map_area, ptr_Player->coordY, ptr_Player->coordX, "%c", CELL_player);
			ptr_Player->stamina -= 1 * map.biome.stamina_rate;
			PrintBottomBar(map, *ptr_Player, info_area, isMapSavedForLoop);
			update_areas(map_area, info_area);
		}
		napms(50);
	}
	EraseRefresh();
	delwin(map_area);
	delwin(info_area);
	nodelay(stdscr, FALSE);
	DeleteMap(&map);
	printw("Прикючение закончено!\n");

}

