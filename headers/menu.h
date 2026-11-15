#pragma once
#include "player.h"
#include <windows.h>


#define MENU_COUNT 11
#define MENU_HORIZONTAL_PADDING 35
#define TIME_MENU_COUNT 4


//ID меню
enum MENU {
	MENU_exit = 0,
	MENU_createPlayer = 1,
	MENU_PlayerCard = 2,
	MENU_time = 3,
	MENU_put = 4,
	MENU_throw = 5,
	MENU_weight = 6,
	MENU_search = 7,
	MENU_diary = 8,
	MENU_logs = 9,
	MENU_journey = 10
};

enum TIME_MENU {
	TIMEMENU_exit,
	TIMEMENU_clock,
	TIMEMENU_skip,
	TIMEMENU_rest

};

extern const char* MENU_NAMES[MENU_COUNT];

extern const char* TIME_MENU_NAMES[TIME_MENU_COUNT];



void TimeMenuHandle(HANDLE console, PlayerInstance* Player);

void MenuReturn();

int MenuSelect(HANDLE console, char* names_array[], char* name, int last_menuCase, int menu_count, int padding);