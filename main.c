//Лаба 2, Вариант 6
#include <stdio.h>
#include <locale.h>


enum MENU {
	MENU_exit = 0,
	MENU_clock = 1,
	MENU_work = 2,
	MENU_inventory = 3,
	MENU_put = 4,
	MENU_trow = 5,
	MENU_weight = 6
};

enum INVENTORY {
	INVENTORY_empty = 0,
	INVENTORY_wood = 1,
	INVENTORY_stone = 2,
	INVENTORY_seed = 3,
	INVENTORY_water = 4,
	INVENTORY_flower = 5,
	INVENTORY_potato = 6,
	INVENTORY_wheat = 7, 
	INVENTORY_showel = 8,
	INVENTORY_axe = 9
};

int menuSelect() {

};

void menuPrint() {
	printf("[%d] Выход \n[%d] Посмотреть на часы\n[%d] Поработать\n[%d] Посмотреть инвентарь\n[%d] Положить в инвентарь\n[%d] Выбросить из инвентаря\n[%d] Поиск тяжестей", \
		MENU_exit, MENU_clock, MENU_work, MENU_inventory, MENU_put, MENU_trow, MENU_weight);
};

int main() {
	setlocale(LC_ALL, ".UTF-8");
	int current_day = 1, current_hour = 8;

	int inventory[10] = { INVENTORY_empty, INVENTORY_empty, INVENTORY_axe, INVENTORY_wood, INVENTORY_wood, \
						INVENTORY_flower, INVENTORY_empty, INVENTORY_seed, INVENTORY_empty, INVENTORY_wood };

	menuPrint();



};