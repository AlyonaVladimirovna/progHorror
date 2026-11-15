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
				Player = CreatePlayer(Item_struct);
			}
			else {
				printf("Вы точно хотите пересоздать персонажа? Текущий персонаж %s будет удален!\ny\\n\n", Player->name);
				char* answer[16];
				TextInput(&answer, 16);
				while (strcmp(answer, "y") && strcmp(answer, "n")) {
					printf("неверный ввод, попробуйте снова\n");
					TextInput(answer, 16);
				}
				if (!strcmp(answer, "y")) { free(Player); printf("Память по адресу %p Очищена\n", Player); Player = CreatePlayer(Item_struct); }
				else {
					MenuReturn(); continue;
				}
			}
			
			if (Player == NULL) { printf("Не удалось создать персонажа.\n"); MenuReturn(); continue; }
			printf("Персонаж %s создан!\n");
			MenuReturn();
			break;

		case MENU_PlayerCard:
			if (Player == NULL) {
				consCoord.X = 0;
				consCoord.Y = MENU_COUNT + 1;
				SetConsoleCursorPosition(console, consCoord);
				printf("Сначала создайте персонажа!\n");
				MenuReturn();
				continue;
			}
			ClearConsole(console);
			PrintPlayerCard(Player);
			MenuReturn();
			break;
		
		case MENU_time:
			ClearConsole(console);
			TimeMenuHandle(console, Player);
			MenuReturn();
			break;

		case MENU_put:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
			printf("Куда положить?\n");
			int slot_put = IntInputCheck(0, 9, "Нет такого слота");
			printf("Что положить?\n");
			int new_item = IntInputCheck(Item_struct[1].id, Item_struct[9].id, "Нет такого предмета");
			inventory_put(Player->inventory, Item_struct, slot_put, new_item);
			MenuReturn();
			break;

		case MENU_throw: 
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
			printf("С какого слота выбросить?\n");
			int slot_throw = IntInputCheck(0, 9, "Нет такого слота");
			inventory_throw(Player->inventory, Item_struct, slot_throw);
			MenuReturn();
			break;

		case MENU_weight: 
			ClearConsole(console);
			printf("Введите ID предмета\n");
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
			int weight_check = IntInputCheck(Item_struct[1].id, Item_struct[MAX_ITEMS - 1].id, "Нет такого предмета");
			inventory_weight(Player->inventory, Item_struct, weight_check);
			MenuReturn();
			break;

		case MENU_search:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
			printf("Введите название: ");
			char text_input[MAX_NAME_LEN];
			TextInput(&text_input, MAX_NAME_LEN);
			int searching_ID = ID_Search(text_input, Item_struct);
			inventory_search(searching_ID, text_input, Player->inventory, Item_struct);
			MenuReturn();
			break;

		case MENU_diary:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
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
				printf("Не зватает выносливости!\n"); MenuReturn(); break;
			}
			else if (Player->current_hour == 2) {
				printf("Уже слишком поздно!\n"); break;
			}
			Journey(Player, console);
			MenuReturn();
			break;
		}

	}

};