#include "player.h"
#include <stdio.h>
#include <curses.h>

PlayerInstance* CreatePlayer(ItemsDict Item_struct[], int doRandInventory) {
	PlayerInstance* Player = malloc(sizeof(PlayerInstance));
	if (Player == NULL) {
		printw("Не выделить память\n");
		return NULL;
	}
	printw("Введите имя: ");
	refresh();
	TextInput(Player->name, MAX_PLAYER_NAME);
	Player->hp = MAX_PLAYER_HP;
	Player->max_hp =  MAX_PLAYER_HP;
	Player->stamina = MAX_PLAYER_STAMINA;
	Player->max_stamina = MAX_PLAYER_STAMINA;
	Player->gold = DEFAULT_GOLD;
	if (doRandInventory) {
		for (int i = 0; i < INVENTORY_SLOTS; i++) {
			Player->inventory[i].id = Item_struct[rand() % MAX_ITEMS].id;
			if (Player->inventory[i].id == Item_struct[EMPTY_POSITION].id) Player->inventory[i].count = 0;
			else if (Item_struct[INVENTORY_idMatch(Player->inventory[i].id, Item_struct)].isStackable != 0) Player->inventory[i].count = (rand() % MAX_START_ITEM_COUNT) + 1;
			else if (Item_struct[INVENTORY_idMatch(Player->inventory[i].id, Item_struct)].isStackable == 0)  Player->inventory[i].count = 1;
			
		}
	}
	else {
		for (int i = 0; i < INVENTORY_SLOTS; i++) {
			Player->inventory[i].id = Item_struct[EMPTY_POSITION].id;
			Player->inventory[i].count = 0;
		}
	}
	Player->current_day = START_DAY;
	Player->current_hour = START_HOUR;
	return Player;
}

void PrintPlayerCard(PlayerInstance* ptr_player, const ItemsDict Item_struct[]) {
	if (&ptr_player == NULL) { printw("Указатель на PlayerInstance равен NULL.\n"); return; }
	printw("Имя: %s\nЗдоровье: %d/%d\nВыносливость: %d/%d\nЗолото: %d\nВремя: День %d, %d:00\n", ptr_player->name, ptr_player->hp, ptr_player->max_hp, \
		ptr_player->stamina, ptr_player->max_stamina, ptr_player->gold, ptr_player->current_day, ptr_player->current_hour);
	for (int i = 0; i < INVENTORY_SLOTS; i++) {
		printw("[%d| %-s x%d] ", i+1, INVENTORY_nameMatch(ptr_player->inventory[i].id, Item_struct), ptr_player->inventory[i].count);
		if (i == (INVENTORY_SLOTS / 2)-1) printw("\n");
	}
		
	printw("\n");
	refresh();
}

