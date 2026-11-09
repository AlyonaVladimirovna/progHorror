#include "map.h"
#include <stdio.h>
#include <stdlib.h>

char** GenerateMap(int* ptr_size) {
	*ptr_size = rand() % 25 + 5;
	char** map;
	map = (char**)malloc(*ptr_size * sizeof(char*));
	for (int i = 0; i < *ptr_size; i++) {
		map[i] = (char*)malloc(*ptr_size * sizeof(char));
	}

	for (int i = 0; i < *ptr_size; i++) {
		for (int j = 0; j < *ptr_size; j++) {
			int isObstruction = rand() % 100;
			if (isObstruction < OBSTRUCTION_CHANCE) map[i][j] = CELL_tree;
			else map[i][j] = CELL_empty;
		}
	}
	map[rand() % *ptr_size][rand() % *ptr_size] = CELL_campfire;
	return map;
}

void DeleteMap(char** map, int size) {
	for (int i = 0; i < size; i++)
		free(map[i]);
	free(map);
}

void PrintMap(char** map, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			printf("%c", map[i][j]);
		printf("\n");
	}
}

void GetRandomCoords(int* ptr_x, int* ptr_y, int size, char** map) {
	do {
		*ptr_x = rand() % size,
			*ptr_y = rand() % size;
	} while (map[*ptr_y][*ptr_x] == CELL_tree);
}
