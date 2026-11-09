#include "inventory.h"
#include <stdio.h>
#include <string.h>



//Массив стандартных названий
const char* COMMON_INVENTORY_NAMES[MAX_ITEMS] = {
	[0] = "Пусто",
	[1] = "Древесина",
	[2] = "Камень",
	[3] = "Семена",
	[4] = "Бутыль с водой",
	[5] = "Цветок",
	[6] = "Картофель",
	[7] = "Пшеница",
	[8] = "Лопата",
	[9] = "Топор"
};


const int COMMON_INVENTORY_ID[MAX_ITEMS] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

//функция сопостовления id и имени
const char* INVENTORY_nameMatch(int input, const InventoryItems nameid[]) {
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (input == nameid[i].id) {
			return nameid[i].name;
		}
	}
};


//ФУНКЦИЯ сопостовления id и id
int INVENTORY_idMatch(int input, const InventoryItems nameid[]) {
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (input == nameid[i].id) {
			return nameid[i].id;
		}
	}
};

//Заполнение массива имен с файла, или стандартными значениями
void read_item_names(InventoryItems Item_struct[]) {
	FILE* itemlist = fopen("files/items.txt", "r");
	if (itemlist == NULL) {
		printf("Не удалось открыть files/items.txt, взяты названия по умолчанию\n");
		for (int i = 0; i < MAX_ITEMS; i++) {
			strcpy_s(Item_struct[i].name, MAX_NAME_LEN, COMMON_INVENTORY_NAMES[i]);
			Item_struct[i].id = COMMON_INVENTORY_ID[i];
		};
	}
	else {
		char file_line[MAX_NAME_LEN];
		for (int i = 0; i < MAX_ITEMS; i++) {
			if (fgets(file_line, sizeof(file_line), itemlist) != NULL)
				sscanf_s(file_line, "%d %32[^%n\n]", &(Item_struct[i].id), Item_struct[i].name, (unsigned)MAX_NAME_LEN);
		};
		fclose(itemlist);
	}

}

//Функция "Положить"
void inventory_put(InventoryItems inventory_array[], const InventoryItems Item_struct[], int slot_put, int new_item) {
	strcpy_s(
		inventory_array[slot_put].name,
		sizeof(inventory_array[slot_put].name),
		INVENTORY_nameMatch(new_item, Item_struct)
	);
	inventory_array[slot_put].id = INVENTORY_idMatch(new_item, Item_struct);
	printf("Положили %s в слот %d\n", inventory_array[slot_put].name, slot_put);
}

//Функция "Выбросить"
void inventory_throw(InventoryItems inventory_array[], const InventoryItems Item_struct[], int slot_throw) {
	if (inventory_array[slot_throw].id != Item_struct[0].id) {
		printf("Выбросили %s из слота %d\n", inventory_array[slot_throw].name, slot_throw);
		inventory_array[slot_throw].id = Item_struct[0].id;
		strcpy_s(inventory_array[slot_throw].name, sizeof(inventory_array[slot_throw].name), Item_struct[0].name);
	}
	else printf("Слот и так пустой.\n");
}

// Определенно функциия
void inventory_weight(const InventoryItems inventory_array[], const InventoryItems Item_struct[], int weight_check) {
	printf("В этих слотах ID больше: ");
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (weight_check < inventory_array[i].id)
			printf("[%d] ", i);
	}
	printf("\n");
}

// Помните его? это он
int ID_Search(char* input_name, const InventoryItems* items) {
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (!strcmp(input_name, items[i].name)) return items[i].id;
	}
	return -1;
}
// Функция поиска в инвентаре и вывод слотов.
void inventory_search(int searching_ID, char* text_input, const InventoryItems inventory_array[], const InventoryItems Item_struct[]) {
	if (searching_ID >= 0) {
		int slots[MAX_ITEMS], slot_count = 0;
		int isInInventory = 0;
		for (int i = 0; i < MAX_ITEMS; i++) {
			if (inventory_array[i].id == searching_ID) {
				isInInventory = 1;
				slots[slot_count++] = i;
			}
		}
		if (slot_count < MAX_ITEMS) slots[slot_count++] = -1;
		if (isInInventory) {
			printf("Предмет %s есть в слотах: ", text_input);
			for (int i = 0; i < MAX_ITEMS; i++) {
				if (slots[i] != -1) printf("[%d]", slots[i]);
				else break;
			}
			printf("\n");
		}
		else printf("Предмета %s нет в инвентаре.\n", text_input);
	}
	else printf("Такого предмета нет.\n");
}
