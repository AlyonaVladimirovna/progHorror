#include "player.h"
#include <stdio.h>

PlayerInstance* CreatePlayer(InventoryItems Item_struct[], int doRandInventory) {
	PlayerInstance* Player = malloc(sizeof(PlayerInstance));
	if (Player == NULL) {
		printf("Не удалось создать персонажа\n");
		return;
	}
	printf("Введите имя: ");
	TextInput(Player->name, MAX_PLAYER_NAME);
	Player->hp = MAX_PLAYER_HP;
	Player->max_hp =  MAX_PLAYER_HP;
	Player->stamina = MAX_PLAYER_STAMINA;
	Player->max_stamina = MAX_PLAYER_STAMINA;
	Player->gold = DEFAULT_GOLD;
	if (doRandInventory) {
		for (int i = 0; i < INVENTORY_SLOTS; i++) {
			Player->inventory[i] = Item_struct[rand() % MAX_ITEMS];
			if (Player->inventory[i].id == Item_struct[0].id) Player->inventory[i].count = 0;
			else Player->inventory[i].count = (rand() % MAX_START_ITEM_COUNT) +1;
			
		}
	}
	else {
		for (int i = 0; i < INVENTORY_SLOTS; i++) {
			Player->inventory[i] = Item_struct[0];
			Player->inventory[i].count = 0;
		}
	}
	Player->current_day = START_DAY;
	Player->current_hour = START_HOUR;
	return Player;
}

void PrintPlayerCard(PlayerInstance* ptr_player) {
	if (&ptr_player == NULL) { printf("Указатель на PlayerInstance равен NULL.\n"); return; }
	printf("Имя: %s\nЗдоровье: %d/%d\nВыносливость: %d/%d\nЗолото: %d\nВремя: День %d, %d:00\n", ptr_player->name, ptr_player->hp, ptr_player->max_hp, \
		ptr_player->stamina, ptr_player->max_stamina, ptr_player->gold, ptr_player->current_day, ptr_player->current_hour);
	for (int i = 0; i < INVENTORY_SLOTS; i++) {
		printf("[%d| %-s x%d] ", i+1, ptr_player->inventory[i].name, ptr_player->inventory[i].count);
		if (i == (INVENTORY_SLOTS / 2)-1) printf("\n");
	}
		
	printf("\n");
}

