#pragma once
#include <windows.h>

#define MENU_COUNT 11

//ID меню
enum MENU {
	MENU_exit = 0,
	MENU_clock = 1,
	MENU_work = 2,
	MENU_inventory = 3,
	MENU_put = 4,
	MENU_throw = 5,
	MENU_weight = 6,
	MENU_search = 7,
	MENU_diary = 8,
	MENU_logs = 9,
	MENU_journey = 10
};

extern const char* MENU_NAMES[MENU_COUNT];

// Вывод меню
void menuPrint(char* name);

void MenuReturn();

int MenuSelect(HANDLE console, char* name, int last_menuCase);