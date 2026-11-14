#include "player.h"
#include <stdio.h>

PlayerInstance* CreatePlayer() {
	PlayerInstance* Player = malloc(sizeof(PlayerInstance));
	if (Player == NULL) {
		printf("Не удалось создать персонажа\n");
		return;
	}
	printf("Введите имя: ");
	TextInput(Player->name, MAX_PLAYER_NAME);
	Player->hp = Player->max_hp =  MAX_PLAYER_HP;
	Player->stamina = Player->max_stamina = MAX_PLAYER_STAMINA;

	return Player;
}


