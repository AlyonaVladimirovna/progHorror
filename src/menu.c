#include "menu.h"
#include "keyboardinput.h"
#include "utils.h"



void MenuReturn() {
	printw("Нажмите любую клавишу чтобы вернуться к меню.");
	refresh();
	getch();
}



int MenuSelect(WINDOW* console, char* names_array[], char* name, int last_menuCase, int menu_count, int padding) {
	int menuCase = last_menuCase;
	int coordX, coordY;
	int key;
	int align = (name == NULL) ? (align = 0) : (align = 1);
	coordY = align + last_menuCase;
	if (name != NULL) printw("Привет, %s!\n", name);
	for (int i = 0; i < menu_count; i++)
		printw("[%d] %s\n", i, names_array[i]);
	refresh();
	mvwprintw(console, coordY, padding, "<-");
	while ((key = MenuKeyboardHandler()) != ENTER) {
		menuCase = menuCase + key;
		if (menuCase < 0) menuCase = menu_count -1;
		if (menuCase > menu_count - 1) menuCase = 0;
		if (key) {
			mvwprintw(console, coordY, padding, "  ");
			coordY = menuCase + align;
			mvwprintw(console, coordY, padding, "<-");
		}
		refresh();
	}
	return menuCase;
}

void TimeMenuHandle(WINDOW* console, PlayerInstance* Player) {
	if (Player == NULL) { printw("Указатель на PlayerInstance равен NULL\n"); return; }
	const char* TIME_MENU_NAMES[TIME_MENU_COUNT] = {
	[TIMEMENU_exit] = "Назад",
	[TIMEMENU_clock] = "Посмотреть на часы",
	[TIMEMENU_skip] = "Ждать",
	[TIMEMENU_rest] = "Отдохнуть"
	};
	int last_menuCase = 0;
	while (1) {
		EraseRefresh();
		switch (last_menuCase = MenuSelect(console, TIME_MENU_NAMES, Player->name, last_menuCase, TIME_MENU_COUNT, MENU_HORIZONTAL_PADDING)) {
		case TIMEMENU_exit: return;

		case TIMEMENU_clock:
			EraseRefresh();
			printw("Текущее время: День %d, %d:00\n", Player->current_day, Player->current_hour);
			MenuReturn();
			break;

		case TIMEMENU_skip:
			EraseRefresh();
			TimeSkip(Player);
			MenuReturn();
			break;

		case TIMEMENU_rest:
			EraseRefresh();
			if (Player == NULL) { printw("Указатель на PlayerInstance равен NULL\n"); break; }
			Player->current_day++;
			Player->current_hour = START_HOUR;
			Player->stamina = Player->max_stamina;
			Player->hp = Player->max_hp;
			printw("Вы отдохнули! Текущее время: день %d, %d:00\n", Player->current_day, Player->current_hour);
			MenuReturn();
			break;


		}
	}
}

void InventoryMenuHandle(WINDOW* console, PlayerInstance* Player, const ItemsDict Item_struct[]) {
	if (Player == NULL) { printw("Указатель на PlayerInstance равен NULL\n"); return; }

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
		EraseRefresh();
		switch (last_menuCase = MenuSelect(console, INVENTORY_MENU_NAMES, Player->name, last_menuCase, INVENTORY_MENU_COUNT, MENU_HORIZONTAL_PADDING)) {
		case MENU_exit: return;

		case INVMENU_show: 
			EraseRefresh();
			for (int i = 0; i < INVENTORY_SLOTS; i++)
				printw("%d| %s x %d\n", Player->inventory[i].id, INVENTORY_nameMatch(Player->inventory[i].id, Item_struct), Player->inventory[i].count);
			MenuReturn();
			break;

		case INVMENU_put:
			EraseRefresh();
			if (Player == NULL) { printw("Сначала создайте персонажа!\n"); break; }
			printw("Куда положить?\n");
			refresh();
			int slot_put = IntInputCheck(0, 9, "Нет такого слота");
			printw("Что положить?\n");
			refresh();
			int new_item = IntInputCheck(Item_struct[1].id, Item_struct[MAX_ITEMS-1].id, "Нет такого предмета");
			inventory_put(Player->inventory, Item_struct, slot_put, new_item);
			MenuReturn();
			break;

		case INVMENU_throw:
			EraseRefresh();
			if (Player == NULL) { printw("Сначала создайте персонажа!\n"); break; }
			printw("С какого слота выбросить?\n");
			refresh();
			int slot_throw = IntInputCheck(0, 9, "Нет такого слота");
			inventory_throw(Player->inventory, Item_struct, slot_throw);
			MenuReturn();
			break;

		case INVMENU_weight:
			EraseRefresh();
			printw("Введите ID предмета\n");
			if (Player == NULL) { printw("Сначала создайте персонажа!\n"); break; }
			int weight_check = IntInputCheck(Item_struct[1].id, Item_struct[MAX_ITEMS - 1].id, "Нет такого предмета");
			inventory_weight(Player->inventory, weight_check);
			MenuReturn();
			break;

		case INVMENU_search:
			EraseRefresh();
			if (Player == NULL) { printw("Сначала создайте персонажа!\n"); break; }
			printw("Введите название: ");
			char text_input[MAX_NAME_LEN];
			TextInput(&text_input, MAX_NAME_LEN);
			int searching_ID = ID_Search(text_input,Player->inventory, Item_struct);
			inventory_search(searching_ID, text_input, Player->inventory, Item_struct);
			MenuReturn();
			break;

		case INVMENU_sell:
			EraseRefresh();
			Player->gold += InventorySell(Player->inventory, Item_struct);
			printw("\n Текущее золото: %d\n", Player->gold);
			MenuReturn();
			break;
		}
	}
}