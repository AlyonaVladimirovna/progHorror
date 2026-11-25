#pragma once
#include "inventory.h"
#include "consoleinput.h"
#include <Windows.h>

#define MAX_PLAYER_NAME 33
#define MAX_PLAYER_HP 100
#define MAX_PLAYER_STAMINA 100
#define DEFAULT_GOLD 10
#define START_DAY 1
#define START_HOUR 8

typedef struct {
	char name[MAX_PLAYER_NAME];
	int coordX, coordY;
	InventoryItems inventory[INVENTORY_SLOTS];
	int hp, max_hp;
	int stamina, max_stamina;
	int gold;
	int current_day;
	int current_hour;
}PlayerInstance;

PlayerInstance* CreatePlayer(InventoryItems Item_struct[], int DoRandInventory);

void PrintPlayerCard(PlayerInstance* ptr_player);
