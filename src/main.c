#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include "inventory.h"
#include "menu.h"
#include "consoleinput.h"
#include "keyboardinput.h"
#include "map.h"
#include "Player.h"
#include "utils.h"
#include <curses.h>
#pragma warning(disable: 4996)

int main() {

	WINDOW* stdscr = initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, FALSE);

	setlocale(LC_ALL, ".UTF-8");

	InventoryItems Item_struct[MAX_ITEMS];
	read_item_names(Item_struct);

	PlayerInstance* Player = NULL;

	srand(time(NULL));

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


	int last_menuCase = 0;
	while (1) {
		EraseRefresh();
		switch (last_menuCase = MenuSelect(stdscr, MENU_NAMES, Player->name, last_menuCase, MENU_COUNT, MENU_HORIZONTAL_PADDING)) {

		case MENU_exit:
			EraseRefresh();
			if (Player != NULL) {
				free(Player);
				printw("память по адресу %p очищена\n", Player);
			}
			printw("Выход.\n"); 
			refresh();
			napms(500);
			return 0;

		case MENU_createPlayer:
			EraseRefresh();
			if (Player == NULL) {
				Player = CreatePlayer(Item_struct, 1);
			}
			else {
				printw("Вы точно хотите пересоздать персонажа? Текущий персонаж %s будет удален!\ny\\n\n", Player->name); refresh();
				if (YesNoInput()) { free(Player); printw("Память по адресу %p Очищена\n", Player); Player = CreatePlayer(Item_struct, 1); }
				else {
					MenuReturn(); continue;
				}
			}
			break;

			if (Player == NULL) { printw("Не удалось создать персонажа.\n"); MenuReturn(); continue; }
			printw("Персонаж %s создан!\n");
			MenuReturn();
			break;

		case MENU_PlayerCard:
			EraseRefresh();
			if (Player == NULL) { printw("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			PrintPlayerCard(Player);
			MenuReturn();
			break;

		case MENU_time:
			EraseRefresh();
			if (Player == NULL) { printw("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			TimeMenuHandle(stdscr, Player);
			break;

		case MENU_inventory:
			EraseRefresh();
			if (Player == NULL) { printw("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			InventoryMenuHandle(stdscr, Player, Item_struct);
			break;


		case MENU_diary:
			EraseRefresh();
			if (Player == NULL) { printw("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			GenerateDiary(Player->name, Player->current_day, Player->current_hour, Player->inventory);
			printw("Дневничок записан в diary.txt\n");
			MenuReturn();
			break;

		case MENU_logs:
			EraseRefresh();
			CheckLogs();
			MenuReturn();
			break;

		case MENU_journey:
			EraseRefresh();
			if (Player == NULL) { printw("Сначала создайте персонажа!\n"); MenuReturn(); break; }
			if (Player->stamina == 0) {
				printw("Не хватает выносливости!\n"); MenuReturn(); break;
			}
			else if (Player->current_hour == 2) {
				printw("Уже слишком поздно!\n"); break;
			}
			Journey(Player, Item_struct);
			MenuReturn();
			break;
		}

	}

}