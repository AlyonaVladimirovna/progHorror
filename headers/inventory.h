#pragma once

//Максимальное количество предметов
#define MAX_ITEMS 11
#define INVENTORY_SLOTS 10
//Максимальная длина названия предмета
#define MAX_NAME_LEN 32
#define MAX_ITEM_COUNT 99
#define MAX_START_ITEM_COUNT 15
#define MAP_PRICE 10

typedef struct {
	int id;
	char name[MAX_NAME_LEN];
	int count;
} InventoryItems;

enum ITEMSTRUCT_ORD {
	EMPTY,
	WOOD,
	STONE,
	SEEDS,
	BOTTLE,
	FLOWER,
	POTATO,
	WHEAT,
	SHOWEL,
	AXE,
	MAP
};

extern const char* COMMON_INVENTORY_NAMES[MAX_ITEMS];
extern const int COMMON_INVENTORY_ID[MAX_ITEMS];

//функция сопостовления id и имени
const char* INVENTORY_nameMatch(int input, const InventoryItems nameid[]);

//ФУНКЦИЯ сопостовления id и id
int INVENTORY_idMatch(int input, const InventoryItems nameid[]);

//Заполнение массива имен с файла, или стандартными значениями
void read_item_names(InventoryItems Item_struct[]);

//Функция "Положить"
void inventory_put(InventoryItems inventory_array[], const InventoryItems Item_struct[], int slot_put, int new_item);

// Функция "Выбросить"
void inventory_throw(InventoryItems inventory_array[], const InventoryItems Item_struct[], int slot_throw);

// Определенно функциия
void inventory_weight(const InventoryItems inventory_array[], const InventoryItems Item_struct[], int weight_check);

// Помните его? это он
int ID_Search(char* input_name, const InventoryItems* items);

// Функция поиска в инвентаре и вывод слотов.
void inventory_search(int searching_ID, char* text_input, const InventoryItems inventory_array[], const InventoryItems Item_struct[]);

int FindEmptySlot(InventoryItems inventory_array[], const InventoryItems Item_struct[]);

int FindItemSlot(int ID, InventoryItems inventory_array[]);

int InventorySell(InventoryItems inventory_array[], const InventoryItems Item_struct[]);