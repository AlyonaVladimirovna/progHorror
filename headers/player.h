#pragma once
#include "inventory.h"
#include "consoleinput.h"
#include <Windows.h>

#define MAX_PLAYER_NAME 33
#define MAX_PLAYER_HP 100
#define MAX_PLAYER_STAMINA 100

typedef struct {
	char name[MAX_PLAYER_NAME];
	COORD PlayerCoord;
	InventoryItems inventory[MAX_ITEMS];
	int hp, max_hp;
	int stamina, max_stamina;
	int gold;
}PlayerInstance;

PlayerInstance* CreatePlayer();
