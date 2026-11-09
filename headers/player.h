#pragma once
#include "inventory.h"
#include <Windows.h>

#define MAX_PLAYER_NAME 33
//Количество пунктов меню

typedef struct {
	char name[MAX_PLAYER_NAME];
	COORD PlayerCoord;
	InventoryItems inventory[MAX_ITEMS];
	int hp, max_hp;
	int stamina, max_stamina;
	int gold;
}PlayerInstance;

