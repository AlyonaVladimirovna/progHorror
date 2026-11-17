#include "menu.h"
#include "keyboardinput.h"
#include "utils.h"
#include <stdio.h>
#include <conio.h>


const char* MENU_NAMES[MENU_COUNT] = {
	[MENU_exit] = "Выход",
	[MENU_createPlayer] = "Создать/пересоздать персонажа",
	[MENU_PlayerCard] = "Карточка персонажа",
	[MENU_time] = "Управление временем",
	[MENU_inventory] = "Управление инвентарём",
	[MENU_diary] = "Дневник",
	[MENU_logs] = "Посмотреть логи",
	[MENU_journey] = "Отправится в приключение"
};



void MenuReturn() {
	printf("Нажмите любую клавишу чтобы вернуться к меню.");
	_getch();
}



int MenuSelect(HANDLE console, char* names_array[], char* name, int last_menuCase, int menu_count, int padding) {
	int menuCase = last_menuCase;
	COORD consCoord = { 0, 0 };
	int key;
	int align = (name == NULL) ? (align = 0) : (align = 1);
	consCoord.Y = align + last_menuCase;
	consCoord.X = padding;
	if (name != NULL) printf("Привет, %s!\n", name);
	for (int i = 0; i < menu_count; i++)
		printf("[%d] %s\n", i, names_array[i]);
	SetConsoleCursorPosition(console, consCoord);
	printf("<-");
	while ((key = MenuKeyboardInput()) != ENTER) {
		consCoord.X = padding;
		SetConsoleCursorPosition(console, consCoord);
		menuCase = menuCase + key;
		if (menuCase < 0) menuCase = menu_count -1;
		if (menuCase > menu_count - 1) menuCase = 0;
		printf("  ");
		consCoord.Y = menuCase + align;
		SetConsoleCursorPosition(console, consCoord);
		printf("<-");
	}
	return menuCase;
}

void TimeMenuHandle(HANDLE console, PlayerInstance* Player) {
	if (Player == NULL) { printf("Указатель на PlayerInstance равен NULL\n"); return; }
	const char* TIME_MENU_NAMES[TIME_MENU_COUNT] = {
	[TIMEMENU_exit] = "Назад",
	[TIMEMENU_clock] = "Посмотреть на часы",
	[TIMEMENU_skip] = "Ждать",
	[TIMEMENU_rest] = "Отдохнуть"
	};
	int last_menuCase = 0;
	while (1) {
		ClearConsole(console);
		switch (last_menuCase = MenuSelect(console, TIME_MENU_NAMES, Player->name, last_menuCase, TIME_MENU_COUNT, MENU_HORIZONTAL_PADDING)) {
		case TIMEMENU_exit: return;

		case TIMEMENU_clock:
			ClearConsole(console);
			printf("Текущее время: День %d, %d:00\n", Player->current_day, Player->current_hour);
			MenuReturn();
			break;

		case TIMEMENU_skip:
			ClearConsole(console);
			TimeSkip(Player);
			MenuReturn();
			break;

		case TIMEMENU_rest:
			ClearConsole(console);
			if (Player == NULL) { printf("Указатель на PlayerInstance равен NULL\n"); break; }
			Player->current_day++;
			Player->current_hour = START_HOUR;
			Player->stamina = Player->max_stamina;
			Player->hp = Player->max_hp;
			printf("Вы отдохнули! Текущее время: день %d, %d:00\n", Player->current_day, Player->current_hour);
			MenuReturn();
			break;


		}
	}
}

void InventoryMenuHandle(HANDLE console, PlayerInstance* Player, const InventoryItems Item_struct[]) {
	if (Player == NULL) { printf("Указатель на PlayerInstance равен NULL\n"); return; }

	const char* INVENTORY_MENU_NAMES[INVENTORY_MENU_COUNT] = {
		[MENU_exit] = "Назад",
		[INVMENU_show] = "Посмотреть в инвентарь",
		[INVMENU_put] = "Положить",
		[INVMENU_throw] = "Выбросить",
		[INVMENU_weight] = "Сравнить вес",
		[INVMENU_search] = "Найти",
		[INVMENU_sell] = "Продать"
	};

	int last_menuCase = 0;
	while (1) {
		ClearConsole(console);
		switch (last_menuCase = MenuSelect(console, INVENTORY_MENU_NAMES, Player->name, last_menuCase, INVENTORY_MENU_COUNT, MENU_HORIZONTAL_PADDING)) {
		case MENU_exit: return;

		case INVMENU_show: 
			ClearConsole(console);
			for (int i = 0; i < INVENTORY_SLOTS; i++)
				printf("%d| %s x %d\n", Player->inventory[i].id, Player->inventory[i].name, Player->inventory[i].count);
			MenuReturn();
			break;

		case INVMENU_put:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
			printf("Куда положить?\n");
			int slot_put = IntInputCheck(0, 9, "Нет такого слота");
			printf("Что положить?\n");
			int new_item = IntInputCheck(Item_struct[1].id, Item_struct[MAX_ITEMS-1].id, "Нет такого предмета");
			inventory_put(Player->inventory, Item_struct, slot_put, new_item);
			MenuReturn();
			break;

		case INVMENU_throw:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
			printf("С какого слота выбросить?\n");
			int slot_throw = IntInputCheck(0, 9, "Нет такого слота");
			inventory_throw(Player->inventory, Item_struct, slot_throw);
			MenuReturn();
			break;

		case INVMENU_weight:
			ClearConsole(console);
			printf("Введите ID предмета\n");
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
			int weight_check = IntInputCheck(Item_struct[1].id, Item_struct[MAX_ITEMS - 1].id, "Нет такого предмета");
			inventory_weight(Player->inventory, Item_struct, weight_check);
			MenuReturn();
			break;

		case INVMENU_search:
			ClearConsole(console);
			if (Player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
			printf("Введите название: ");
			char text_input[MAX_NAME_LEN];
			TextInput(&text_input, MAX_NAME_LEN);
			int searching_ID = ID_Search(text_input, Item_struct);
			inventory_search(searching_ID, text_input, Player->inventory, Item_struct);
			MenuReturn();
			break;

		case INVMENU_sell:
			ClearConsole(console);
			Player->gold += InventorySell(Player->inventory, Item_struct);
			printf("\n Текущее золото: %d\n", Player->gold);
			MenuReturn();
			break;
		}
	}
}