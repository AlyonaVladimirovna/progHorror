#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include "inventory.h"
#include "menu.h"
#include "consoleinput.h"
#include "keyboardinput.h"
#include "map.h"
#include "Player.h"
#include "utils.h"
#pragma warning(disable: 4996)

int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, ".UTF-8");
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD consCoord = { 0, 0 };

	InventoryItems Item_struct[MAX_ITEMS];
	read_item_names(Item_struct);

	PlayerInstance* Player = NULL;


	srand(time(NULL));


	int last_menuCase = 0;
	while (1) {
		ClearConsole(console);
		switch (last_menuCase = MenuSelect(console, MENU_NAMES, Player->name, last_menuCase, MENU_COUNT, MENU_HORIZONTAL_PADDING)) {

		case MENU_exit:
			ClearConsole(console);
			free(Player);
			printf("память по адресу %p очищена\n", Player);
			printf("Выход.\n"); return 0;

		case MENU_createPlayer:
			ClearConsole(console);
			if (Player == NULL) {
				Player = CreatePlayer(Item_struct, 1);
			}
			else {
				printf("Вы точно хотите пересоздать персонажа? Текущий персонаж %s будет удален!\ny\\n\n", Player->name);
				if (YesNoInput()) { free(Player); printf("Память по адресу %p Очищена\n", Player); Player = CreatePlayer(Item_struct, 1); }
				else {
					MenuReturn(); continue;
				}
			}

			if (Player == NULL) { printf("Не удалось создать персонажа.\n"); MenuReturn(); continue; }
			printf("Персонаж %s создан!\n");
			MenuReturn();
			break;

		case MENU_PlayerCard:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			PrintPlayerCard(Player);
			MenuReturn();
			break;

		case MENU_time:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			TimeMenuHandle(console, Player);
			break;

		case MENU_inventory:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			InventoryMenuHandle(console, Player, Item_struct);
			break;


		case MENU_diary:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			GenerateDiary(Player->name, Player->current_day, Player->current_hour, Player->inventory);
			printf("Дневничок записан в diary.txt\n");
			MenuReturn();
			break;

		case MENU_logs:
			ClearConsole(console);
			CheckLogs();
			MenuReturn();
			break;

		case MENU_journey:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			if (Player->stamina == 0) {
				printf("Не хватает выносливости!\n"); MenuReturn(); break;
			}
			else if (Player->current_hour == 2) {
				printf("Уже слишком поздно!\n"); break;
			}
			Journey(Player, console, Item_struct);
			MenuReturn();
			break;
		}

	}

}