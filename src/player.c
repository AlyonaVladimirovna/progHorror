#include "player.h"
#include <stdio.h>

PlayerInstance* CreatePlayer(InventoryItems Item_struct[]) {
	PlayerInstance* Player = malloc(sizeof(PlayerInstance));
	if (Player == NULL) {
		printf("Не удалось создать персонажа\n");
		return;
	}
	printf("Введите имя: ");
	TextInput(Player->name, MAX_PLAYER_NAME);
	Player->hp = Player->max_hp =  MAX_PLAYER_HP;
	Player->stamina = Player->max_stamina = MAX_PLAYER_STAMINA;
	Player->gold = DEFAULT_GOLD;
	for (int i = 0; i < MAX_ITEMS; i++) {
		Player->inventory[i] = Item_struct[rand() % 10];
	}
	Player->current_day = START_DAY;
	Player->current_hour = START_HOUR;
	return Player;
}

void PrintPlayerCard(PlayerInstance* ptr_player) {
	if (&ptr_player == NULL) { printf("Указатель на PlayerInstance равен NULL.\n"); return; }
	printf("Имя: %s\nЗдоровье: %d/%d\nВыносливость: %d/%d\nЗолото: %d\nВремя: День %d, %d:00\n", ptr_player->name, ptr_player->hp, ptr_player->max_hp, \
		ptr_player->stamina, ptr_player->max_stamina, ptr_player->gold, ptr_player->current_day, ptr_player->current_hour);
	for (int i = 0; i < MAX_ITEMS; i++) {
		printf("[%d| %-s] ", i, ptr_player->inventory[i].name);
		if (i == (MAX_ITEMS / 2)-1) printf("\n");
	}
		
	printf("\n");
}

