#include "menu.h"
#include "keyboardinput.h"
#include "timeManage.h"
#include "utils.h"
#include <stdio.h>
#include <conio.h>


const char* MENU_NAMES[MENU_COUNT] = {
	[MENU_exit] = "Выход",
	[MENU_createPlayer] = "Создать/пересоздать персонажа",
	[MENU_PlayerCard] = "Карточка персонажа",
	[MENU_time] = "Управление временем",
	[MENU_put] = "Положить в инвентарь",
	[MENU_throw] = "Выбросить из инвентаря",
	[MENU_weight] = "Поиск тяжестей",
	[MENU_search] = "Поиск предмета",
	[MENU_diary] = "Дневник",
	[MENU_logs] = "Посмотреть логи",
	[MENU_journey] = "Отправится в приключение"
};

const char* TIME_MENU_NAMES[TIME_MENU_COUNT] = {
	[TIMEMENU_exit] = "Назад",
	[TIMEMENU_clock] = "Посмотреть на часы",
	[TIMEMENU_skip] = "Ждать",
	[TIMEMENU_rest] = "Отдохнуть"
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