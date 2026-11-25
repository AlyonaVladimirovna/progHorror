#include "inventory.h"
#include "consoleinput.h"
#include <stdio.h>
#include <curses.h>
#include <string.h>



//функция сопостовления id и имени
const char* INVENTORY_nameMatch(int input_id, const ItemsDict Item_struct[]) {
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (input_id == Item_struct[i].id) {
			return Item_struct[i].name;
		}
	}
};


//ФУНКЦИЯ сопостовления id и положения в словаре
int INVENTORY_idMatch(int input, const ItemsDict Item_struct[]) {
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (input == Item_struct[i].id) {
			return i;
		}
	}
};

//Заполнение массива имен с файла, или стандартными значениями
void read_item_names(ItemsDict Item_struct[]) {
	FILE* itemlist = fopen("files/items.txt", "r");
	if (itemlist == NULL) {
		printw("Не удалось открыть files/items.txt, взяты названия по умолчанию\n");
		//Массив стандартных названий
		const char* COMMON_INVENTORY_NAMES[MAX_ITEMS] = {
			[EMPTY] = "Пусто",
			[WOOD] = "Древесина",
			[STONE] = "Камень",
			[SEEDS] = "Семена",
			[BOTTLE] = "Бутыль с водой",
			[FLOWER] = "Цветок",
			[POTATO] = "Картофель",
			[WHEAT] = "Пшеница",
			[SHOWEL] = "Лопата",
			[AXE] = "Топор",
			[MAP] = "Карта"
		};
		const int COMMON_INVENTORY_ID[MAX_ITEMS] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		for (int i = 0; i < MAX_ITEMS; i++) {
			strcpy_s(Item_struct[i].name, MAX_NAME_LEN, COMMON_INVENTORY_NAMES[i]);
			Item_struct[i].id = COMMON_INVENTORY_ID[i];
		};
	}
	else {
		char file_line[128];
		for (int i = 0; i < MAX_ITEMS; i++) {
			if (fgets(file_line, sizeof(file_line), itemlist) != NULL) {
				sscanf_s(file_line, "%d;%[^;];%d;%d;%d\n", &(Item_struct[i].id), Item_struct[i].name, (unsigned)MAX_NAME_LEN, &(Item_struct[i].isSellable),\
														&(Item_struct[i].isStackable), &(Item_struct[i].price));
			}
		};
		fclose(itemlist);
	}

}

//Функция "Положить"
void inventory_put(ItemStack inventory_array[], const ItemsDict Item_struct[], int slot_put, int new_item) {
	inventory_array[slot_put].id = Item_struct[INVENTORY_idMatch(new_item, Item_struct)].id;
	inventory_array[slot_put].count = 1;
	printw("Положили %s в слот %d\n", INVENTORY_nameMatch(inventory_array[slot_put].id, Item_struct), slot_put);
}

//Функция "Выбросить"
void inventory_throw(ItemStack inventory_array[], const ItemsDict Item_struct[], int slot_throw) {
	if (inventory_array[slot_throw].id != Item_struct[EMPTY_POSITION].id) {
		printw("Выбросили %s из слота %d\n", INVENTORY_nameMatch(inventory_array[slot_throw].id, Item_struct), slot_throw);
		inventory_array[slot_throw].id = Item_struct[EMPTY_POSITION].id;
		inventory_array[slot_throw].count = 0;
	}
	else printw("Слот и так пустой.\n");
}

// Определенно функциия
void inventory_weight(const ItemStack inventory_array[], int weight_check) {
	printw("В этих слотах ID больше: ");
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (weight_check < inventory_array[i].id)
			printw("[%d] ", i);
	}
	printw("\n");
}

// Помните его? это он
int ID_Search(char* input_name, const ItemStack items[], const ItemsDict Item_struct[]) {
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (!strcmp(input_name, INVENTORY_nameMatch(items[i].id, Item_struct))) return items[i].id;
	}
	return -1;
}
// Функция поиска в инвентаре и вывод слотов.
void inventory_search(int searching_ID, char* text_input, const ItemStack inventory_array[], const ItemsDict Item_struct[]) {
	if (searching_ID >= 0) {
		int slots[INVENTORY_SLOTS], slot_count = 0;
		int isInInventory = 0;
		for (int i = 0; i < INVENTORY_SLOTS; i++) {
			if (inventory_array[i].id == searching_ID) {
				isInInventory = 1;
				slots[slot_count++] = i;
			}
		}
		if (slot_count < INVENTORY_SLOTS) slots[slot_count++] = -1;
		if (isInInventory) {
			printw("Предмет %s есть в слотах: ", text_input);
			for (int i = 0; i < INVENTORY_SLOTS; i++) {
				if (slots[i] != -1) printw("[%d]", slots[i]);
				else break;
			}
			printw("\n");
		}
		else printw("Предмета %s нет в инвентаре.\n", text_input);
	}
	else printw("Такого предмета нет.\n");
}


int FindEmptySlot(ItemStack inventory_array[], const ItemsDict Item_struct[]) {
	for (int i = 0; i < INVENTORY_SLOTS; i++) {
		if (inventory_array[i].id == Item_struct[EMPTY_POSITION].id) return i;
	}
	return -1;
}

int FindItemSlot(int ID, ItemStack inventory_array[]) {
	for (int i = 0; i < INVENTORY_SLOTS; i++) {
		if (ID == inventory_array[i].id)
			if (inventory_array[i].count < MAX_ITEM_COUNT) return i;
			else continue;
	}
	return -1;
}

int InventorySell(ItemStack inventory_array[], const ItemsDict Item_struct[]) {
	for (int i = 0; i < INVENTORY_SLOTS; i++)
		printw("%d| %s x %d\n", i, INVENTORY_idMatch(inventory_array[i].id, Item_struct), inventory_array[i].count);
	int SellSlot;
	while (1) {
		printw("С какого слота продать?\n");
		refresh();
		SellSlot = IntInputCheck(0, INVENTORY_SLOTS, "Некорректный ввод!\n");
		if (Item_struct[INVENTORY_idMatch(inventory_array[SellSlot].id, Item_struct)].isSellable) break;
		printw("Данный предмет нельзя продать! Ввести снова? (y/n)\n");
		refresh();
		if (YesNoInput()) continue;
		else return 0;
	}
	printw("Сколько продать?  ");
	refresh();
	int quantity;
	quantity = IntInputCheck(0, inventory_array[SellSlot].count, "Введите число от 0 и не больше количества!\n");
	int gold = Item_struct[INVENTORY_idMatch(inventory_array[SellSlot].id, Item_struct)].price * quantity;
	printw("Продано: %s в количестве %d за %d золота\n", INVENTORY_nameMatch(inventory_array[SellSlot].id, Item_struct), quantity, gold);
	if(inventory_array[SellSlot].count - quantity != 0) {
		inventory_array[SellSlot].count -= quantity;
		return gold;
	}
	else {
		inventory_array[SellSlot].id = Item_struct[EMPTY_POSITION].id;
		inventory_array[SellSlot].count = 0;
		return gold;
	}

	

}