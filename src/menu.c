#include "menu.h"
#include "keyboardinput.h"
#include <stdio.h>
#include <conio.h>


const char* MENU_NAMES[MENU_COUNT] = {
	[MENU_exit] = "Выход",
	[MENU_clock] = "Посмотреть на часы",
	[MENU_work] = "Поработать",
	[MENU_inventory] = "Посмотреть инвентарь",
	[MENU_put] = "Положить в инвентарь",
	[MENU_throw] = "Выбросить из инвентаря",
	[MENU_weight] = "Поиск тяжестей",
	[MENU_search] = "Поиск предмета",
	[MENU_diary] = "Дневник",
	[MENU_logs] = "Посмотреть логи",
	[MENU_journey] = "Отправится в приключение"
};

// Вывод меню
void menuPrint(char* name) {
	printf("Привет, %s!\n", name);
	for (int i = 0; i < MENU_COUNT; i++)
		printf("[%d] %s\n", i, MENU_NAMES[i]);
}

void MenuReturn() {
	printf("Нажмите любую клавишу чтобы вернуться к меню.");
	_getch();
}



int MenuSelect(HANDLE console, char* name, int last_menuCase) {
	int menuCase = last_menuCase;
	COORD consCoord = { 0, 0 };
	int key;
	menuPrint(name);
	consCoord.Y = 1 + last_menuCase;
	consCoord.X = 30;
	SetConsoleCursorPosition(console, consCoord);
	printf("<-");
	while ((key = MenuKeyboardInput()) != ENTER) {
		ClearConsole(console);
		menuPrint(name);
		consCoord.X = 30;
		SetConsoleCursorPosition(console, consCoord);
		menuCase = menuCase + key;
		if (menuCase < 0) menuCase = MENU_COUNT - 1;
		if (menuCase > MENU_COUNT - 1) menuCase = 0;
		consCoord.Y = menuCase + 1;
		SetConsoleCursorPosition(console, consCoord);
		printf("<-");
	}
	return menuCase;
}