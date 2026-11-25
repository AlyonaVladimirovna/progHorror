#pragma once

#define MAX_ITEMS 11
#define INVENTORY_SLOTS 10
#define MAX_NAME_LEN 32
#define MAX_ITEM_COUNT 99
#define MAX_START_ITEM_COUNT 15
#define MAP_PRICE 10
#define EMPTY_POSITION 0

typedef struct {
	int id;
	char name[MAX_NAME_LEN];
	int isSellable, isStackable;
	int price;
} ItemsDict;

typedef struct {
	int id, count;
} ItemStack;


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

//функция сопостовления id и имени
const char* INVENTORY_nameMatch(int input, const ItemsDict nameid[]);

//ФУНКЦИЯ сопостовления id и положения в словаре
int INVENTORY_idMatch(int input_id, const ItemsDict nameid[]);

//Заполнение массива имен с файла, или стандартными значениями
void read_item_names(ItemsDict Item_struct[]);

//Функция "Положить"
void inventory_put(ItemStack inventory_array[], const ItemsDict Item_struct[], int slot_put, int new_item);

// Функция "Выбросить"
void inventory_throw(ItemStack inventory_array[], const ItemsDict Item_struct[], int slot_throw);

// Определенно функциия
void inventory_weight(const ItemStack inventory_array[], int weight_check);

// Помните его? это он
int ID_Search(char* input_name, const ItemStack items[], const ItemsDict Item_struct[]);

// Функция поиска в инвентаре и вывод слотов.
void inventory_search(int searching_ID, char* text_input, const ItemStack inventory_array[], const ItemsDict Item_struct[]);

int FindEmptySlot(ItemStack inventory_array[], const ItemsDict Item_struct[]);

int FindItemSlot(int ID, ItemStack inventory_array[]);

int InventorySell(ItemStack inventory_array[], const ItemsDict Item_struct[]);