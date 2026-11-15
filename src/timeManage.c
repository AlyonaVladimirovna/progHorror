#include "timeManage.h"
#include "player.h"
#include <stdio.h>


void TimeSkip(PlayerInstance* ptr_Player) {
	if (ptr_Player == NULL) { printf("Указатель на PlayerInstance равен NULL\n"); return; }
	int hours;
	while (1) {
		printf("Текущее время: День %d, %d:00 \nВведите сколько ждать: ", ptr_Player->current_day, ptr_Player->current_hour);
		hours = IntInputCheck(0, 24, "Неверный ввод, введите часы от 0 до 24, вы можете прождать только до 2:00\n");
		if ((0 <= ptr_Player->current_hour && ptr_Player->current_hour < 2 && ptr_Player->current_hour + hours < 2) ||\
			(7 < ptr_Player->current_hour && ptr_Player->current_hour < 24 && ptr_Player->current_hour + hours < 26) ){
			if (ptr_Player->current_hour + hours > 23) {
				ptr_Player->current_day += 1;
				ptr_Player->current_hour = (ptr_Player->current_hour + hours) % 24;
				break;
			}
			else { ptr_Player->current_hour = ptr_Player->current_hour + hours; break; }
		}
		printf("Вы можете ждать только до 2:00! Ввести снова? (y/n)\n");
		char* answer[16];
		TextInput(&answer, 16);
		while (strcmp(answer, "y") && strcmp(answer, "n")) {
			printf("неверный ввод, попробуйте снова\n");
			TextInput(answer, 16);
		}
		if (!strcmp(answer, "y")) continue;
		else return;
		
	}
	printf("Подождал %d Часов. Текущее время: День %d, %d:00\n", hours, ptr_Player->current_day, ptr_Player->current_hour);
}
