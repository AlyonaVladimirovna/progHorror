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
#include "player.h"
#include "utils.h"
#pragma warning(disable: 4996)

int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, ".UTF-8");
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD consCoord = { 0, 0 };
	int current_day = 1, current_hour = 8;
	

	printf("Введите имя: ");
	PlayerInstance Player;
	TextInput(&Player.name, MAX_PLAYER_NAME);

	InventoryItems Item_struct[MAX_ITEMS];
	read_item_names(Item_struct);


	srand(time(NULL));
	for (int i = 0; i < MAX_ITEMS; i++) {
		Player.inventory[i] = Item_struct[rand() % 10];
	}

	int last_menuCase = 0;
	while (1) {
		ClearConsole(console);
		switch (last_menuCase = MenuSelect(console, Player.name, last_menuCase)) {

		case MENU_exit: 
			ClearConsole(console); printf("Выход.\n"); return 0;

		case MENU_clock:  ClearConsole(console); printf("Текущее время: День %d, %d:00\n", current_day, current_hour); MenuReturn(); break;

		case MENU_work:
			ClearConsole(console);
			printf("Сколько работать?\n");
			int work_hours = IntInputCheck(0,24, "Неверный ввод, нельзя столько работать");
			calculating_workHours(work_hours, &current_hour, &current_day);
			printf("Текущее время: День %d, %d:00\n", current_day, current_hour);
			MenuReturn();
			break;

		case MENU_inventory: 
			ClearConsole(console);
			for (int i = 0; i < MAX_ITEMS; i++)
				printf("Слот [%d]: %s (%d)\n", i, Player.inventory[i].name, Player.inventory[i].id);
			MenuReturn();
			break;

		case MENU_put:
			ClearConsole(console);
			printf("Куда положить?\n");
			int slot_put = IntInputCheck(0, 9, "Нет такого слота");
			printf("Что положить?\n");
			int new_item = IntInputCheck(Item_struct[1].id, Item_struct[9].id, "Нет такого предмета");
			inventory_put(Player.inventory, Item_struct, slot_put, new_item);
			MenuReturn();
			break;

		case MENU_throw: 
			ClearConsole(console);
			printf("С какого слота выбросить?\n");
			int slot_throw = IntInputCheck(0, 9, "Нет такого слота");
			inventory_throw(Player.inventory, Item_struct, slot_throw);
			MenuReturn();
			break;

		case MENU_weight: 
			ClearConsole(console);
			printf("Введите ID предмета\n");
			int weight_check = IntInputCheck(Item_struct[1].id, Item_struct[MAX_ITEMS - 1].id, "Нет такого предмета");
			inventory_weight(Player.inventory, Item_struct, weight_check);
			MenuReturn();
			break;

		case MENU_search:
			ClearConsole(console);
			printf("Введите название: ");
			char text_input[MAX_NAME_LEN];
			TextInput(&text_input, MAX_NAME_LEN);
			int searching_ID = ID_Search(text_input, Item_struct);
			inventory_search(searching_ID, text_input, Player.inventory, Item_struct);
			MenuReturn();
			break;

		case MENU_diary:
			ClearConsole(console);
			GenerateDiary(Player.name, current_day, current_hour, Player.inventory);
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
			Journey(&Player, console);
			MenuReturn();
			break;
		}

	}

};