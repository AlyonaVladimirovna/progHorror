#pragma once
#include "player.h"
#include <curses.h>


#define MENU_COUNT 8
#define MENU_HORIZONTAL_PADDING 35
#define TIME_MENU_COUNT 4
#define INVENTORY_MENU_COUNT 7


//ID меню
enum MENU {
	MENU_exit = 0,
	MENU_createPlayer = 1,
	MENU_PlayerCard = 2,
	MENU_time = 3,
	MENU_inventory = 4,
	MENU_diary = 5,
	MENU_logs = 6,
	MENU_journey = 7
};

enum TIME_MENU {
	TIMEMENU_exit,
	TIMEMENU_clock,
	TIMEMENU_skip,
	TIMEMENU_rest

};

enum INVENTORY_MENU {
	INVMENU_show = 1,
	INVMENU_put,
	INVMENU_throw,
	INVMENU_weight,
	INVMENU_search,
	INVMENU_sell
};

void TimeMenuHandle(WINDOW* console, PlayerInstance* Player);

void MenuReturn();

int MenuSelect(WINDOW* console, char* names_array[], char* name, int last_menuCase, int menu_count, int padding);

void InventoryMenuHandle(WINDOW* console, PlayerInstance* Player, const InventoryItems Item_struct[]);