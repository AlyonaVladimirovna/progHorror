//Лаба 2, Вариант 6
#include <stdio.h>
#include <locale.h>
#pragma warning(disable: 4996)


enum MENU {
	MENU_exit = 0,
	MENU_clock = 1,
	MENU_work = 2,
	MENU_inventory = 3,
	MENU_put = 4,
	MENU_throw = 5,
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

const char* INVENTORY_NAMES[10] = {
	[INVENTORY_empty] = "Пусто",
	[INVENTORY_wood] = "Древесина",
	[INVENTORY_stone] = "Камень",
	[INVENTORY_seed] = "Семена",
	[INVENTORY_water] = "Бутыль с водой",
	[INVENTORY_flower] = "Цветок", 
	[INVENTORY_potato] = "Картофель",
	[INVENTORY_wheat] = "Пшеница",
	[INVENTORY_showel] = "Лопата",
	[INVENTORY_axe] = "Топор"
};

int INVENTORY_idMatch(int input) {
	switch (input) {
		case INVENTORY_empty: return INVENTORY_empty;
		case INVENTORY_wood: return INVENTORY_wood;
		case INVENTORY_stone: return INVENTORY_stone;
		case INVENTORY_seed: return INVENTORY_seed;
		case INVENTORY_water: return INVENTORY_water;
		case INVENTORY_flower: return INVENTORY_flower;
		case INVENTORY_potato: return INVENTORY_potato;
		case INVENTORY_wheat: return INVENTORY_wheat;
		case INVENTORY_showel: return INVENTORY_showel;
		case INVENTORY_axe: return INVENTORY_axe;
		default: return -1;

	}
};

int IntInputCheck(int low_limit, int up_limit, const char *error_message) {
	int input = 0;
	int error_flag = 0;
	while (1) {
		error_flag = scanf("%d", &input);
		if (error_flag != 1 || input > up_limit || input < low_limit)
		{
			printf("%s\n", error_message);
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
		else return input;
	};
};



void menuPrint() {
	printf("\033[2J\033[H"); // очищение экрана
	printf("[%d] Выход \n[%d] Посмотреть на часы\n[%d] Поработать\n[%d] Посмотреть инвентарь\n[%d] Положить в инвентарь\n[%d] Выбросить из инвентаря\n[%d] Поиск тяжестей\n", \
		MENU_exit, MENU_clock, MENU_work, MENU_inventory, MENU_put, MENU_throw, MENU_weight);
};

int main() {
	setlocale(LC_ALL, ".UTF-8");
	int current_day = 1, current_hour = 8;

	int inventory_array[10] = { INVENTORY_empty, INVENTORY_empty, INVENTORY_axe, INVENTORY_wood, INVENTORY_wood, \
						INVENTORY_flower, INVENTORY_empty, INVENTORY_seed, INVENTORY_empty, INVENTORY_wood };

	menuPrint();
	while (1) {
		printf("Выберите пункт меню\n");
		switch (IntInputCheck(0, 6, "Неверный ввод")) {

		case MENU_exit: printf("\033[2J\033[H"); printf("Выход.\n"); return 0;

		case MENU_clock: printf("Текущее время: День %d, %d:00\n", current_day, current_hour); break;

		case MENU_work: 
			printf("Сколько работать?\n");
			int work_hours = IntInputCheck(0,24, "Неверный ввод, нельзя столько работать");
			if (work_hours + current_hour >= 24) {
				current_hour = (current_hour + work_hours) % 24;
				current_day++;
			}
			else current_hour = (current_hour + work_hours);
			printf("Текущее время: День %d, %d:00\n", current_day, current_hour);
			break;

		case MENU_inventory: 
			for (int i = 0; i < sizeof(inventory_array)/ sizeof(*inventory_array); i++)
				printf("Слот [%d]: %s\n", i+1, INVENTORY_NAMES[inventory_array[i]]);
			break;


		case MENU_put:
			printf("Куда положить?\n");
			int slot = IntInputCheck(0, 9, "Нет такого слота");
			printf("Что положить?\n");
			int new_item = IntInputCheck(1, 9, "Нет такого предмета");
			inventory_array[slot] = INVENTORY_idMatch(new_item);
			printf("Положили %s в слот %d\n", INVENTORY_NAMES[inventory_array[slot]], slot);
			break;
		case MENU_throw: printf("Тут будет функция throw\n"); break;
		case MENU_weight:printf("Тут будет функция weight\n"); break;
		}
	}

};