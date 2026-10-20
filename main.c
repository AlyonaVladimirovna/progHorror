//Лаба 3, Вариант 5
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <Windows.h>
#pragma warning(disable: 4996)

//Максимальное количество предметов
#define MAX_ITEMS 10
//Максимальная длина названия предмета
#define MAX_NAME_LEN 32

//ID меню
enum MENU {
	MENU_exit = 0,
	MENU_clock = 1,
	MENU_work = 2,
	MENU_inventory = 3,
	MENU_put = 4,
	MENU_throw = 5,
	MENU_weight = 6,
	MENU_search = 7,
	MENU_diary = 8,
	MENU_logs = 9
};


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

typedef struct {
	int id;
	char name[MAX_NAME_LEN];
} InventoryItems;

//Функция для приведения id к типу enum INVENTORY
const char* INVENTORY_nameMatch(int input, const InventoryItems nameid[]) {
		for(int i = 0; i < MAX_ITEMS; i++){
			if (input == nameid[i].id) {
				return nameid[i].name;
			}
		}
};

int INVENTORY_idMatch(int input, const InventoryItems nameid[]) {
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (input == nameid[i].id) {
			return nameid[i].id;
		}
	}
};

//Ввод чисел (да он понятия не имеет как работать с текстом)
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
		else {
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			return input;
		}
	};
};

//Ввод текста (да он понятия не имеет как работать с числами(на самом деле имеет, но записывает их как символы))
void TextInput(char* text) {

	while (1) {
		if (fgets(text, MAX_NAME_LEN, stdin) == NULL) {
			printf("Неверный ввод\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
		else if (strchr(text, '\n') == NULL) {
			int c;
			while ((c = getchar()) != '\n' && c != EOF);

			printf("Имя слишком длинное\n");
		}
		else {
			text[strcspn(text, "\r\n")] = '\0';
			break;
			
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

void GenerateDiary(char* character_name, int current_day, int current_hour, const InventoryItems* inventory_array) {
	FILE* diary = fopen("files/diary.txt", "a");
	if (diary == NULL) { printf("Не удалось открыть файл дневника\n"); return; }
	fprintf(diary, "Имя: %s\nВремя: День %d, %d:00\nИнвентарь: ", character_name, current_day, current_hour);
	printf("Имя: %s\nВремя: День %d, %d:00\nИнвентарь: ", character_name, current_day, current_hour);
	for (int i = 0; i < MAX_ITEMS; i++) { 
		fprintf(diary, "[%s]", inventory_array[i].name); 
		printf("[%s]", inventory_array[i].name);
	}
	printf("\n");
	fprintf(diary, "\n___\n");
	fclose(diary);
}

// Вывод меню
void menuPrint(char* name) {
	//printf("\033[2J\033[H"); // очищение экрана
	printf("Привет, %s\n[%d] Выход \n[%d] Посмотреть на часы\n[%d] Поработать\n[%d] Посмотреть инвентарь\n\
[%d] Положить в инвентарь\n[%d] Выбросить из инвентаря\n[%d] Поиск тяжестей\n[%d]Поиск предмета\n[%d]Дневник\n[%d]Посмотреть логи\n", \
		name, MENU_exit, MENU_clock, MENU_work, MENU_inventory, MENU_put, MENU_throw, MENU_weight, MENU_search, MENU_diary, MENU_logs);
};



//Вычисление нового дня и часа после работы
void calculating_workHours(int work_hours, int* current_hour, int* current_day) {
	if (work_hours + *current_hour >= 24) {
		*current_hour = (*current_hour + work_hours) % 24;
		*current_day = *current_day + 1;
	}
	else *current_hour = (*current_hour + work_hours);
}

//Функция "Положить"
void inventory_put(InventoryItems inventory_array[], const InventoryItems Item_struct[]) {
	printf("Куда положить?\n");
	int slot_put = IntInputCheck(0, 9, "Нет такого слота");
	printf("Что положить?\n");
	int new_item = IntInputCheck(Item_struct[1].id, Item_struct[9].id, "Нет такого предмета");
	strcpy_s(
		inventory_array[slot_put].name,
		sizeof(inventory_array[slot_put].name),
		INVENTORY_nameMatch(new_item, Item_struct)
	);
	inventory_array[slot_put].id = INVENTORY_idMatch(new_item, Item_struct);
	printf("Положили %s в слот %d\n", inventory_array[slot_put].name, slot_put);
}

//Функция "Выбросить"
void inventory_throw(InventoryItems inventory_array[], const InventoryItems Item_struct[]) {
	printf("С какого слота выбросить?\n");
	int slot_throw = IntInputCheck(0, 9, "Нет такого слота");
	if (inventory_array[slot_throw].id != Item_struct[0].id) {
		printf("Выбросили %s из слота %d\n", inventory_array[slot_throw].name, slot_throw);
		inventory_array[slot_throw].id = Item_struct[0].id;
		strcpy_s(inventory_array[slot_throw].name, sizeof(inventory_array[slot_throw].name), Item_struct[0].name);
	}
	else printf("Слот и так пустой.\n");
}

// Определенно функциия
void inventory_weight(const InventoryItems inventory_array[], const InventoryItems Item_struct[]) {
	printf("Введите ID предмета\n");
	int weight_check = IntInputCheck(Item_struct[1].id, Item_struct[MAX_ITEMS-1].id, "Нет такого предмета");
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
void inventory_search(int searching_ID,  char* text_input, const InventoryItems inventory_array[], const InventoryItems Item_struct[]) {
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


void CheckLogs() {
	FILE* logs = fopen("files/input.txt", "r");
	if (logs == NULL) {
		printf("Не удалось открыть файл с логами.\n"); return;
	}
	else {
		char file_line[256];
		char mark[32];
		int line_number = 0;
		char log_text[192];
		FILE* output = fopen("files/output.txt", "w");
		if (output == NULL) { printf("Не удалось создать output.txt\n"); { if (logs != NULL) fclose(logs); return; } }
		while (fgets(file_line, sizeof(file_line), logs) != NULL) {
			sscanf_s(file_line, "%d [%31[^]]] %191[^\r\n]", &line_number, mark, (unsigned)sizeof(mark), log_text, (unsigned)sizeof(log_text));
			if (!strcmp(mark, "WARN") || !strcmp(mark, "ERROR")) {
				fprintf(output, "%d %s\n", line_number, log_text);
				printf("%d %s\n", line_number, log_text);
			}
		}
		if (output != NULL) fclose(output);
	}
	if (logs != NULL) fclose(logs);
		
}

int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, ".UTF-8");
	int current_day = 1, current_hour = 8;

	InventoryItems Item_struct[MAX_ITEMS];
	read_item_names(Item_struct);


	InventoryItems inventory_array[MAX_ITEMS];

	for (int i = 0; i < MAX_ITEMS; i++) {
		inventory_array[i] = Item_struct[rand() % 10];
	}


	printf("Введите имя: ");
	char character_name[33];
	TextInput(character_name);



	menuPrint(character_name);
	while (1) {
		printf("Выберите пункт меню\n");
		switch (IntInputCheck(0, 9, "Неверный ввод")) {

		case MENU_exit: 
			printf("\033[2J\033[H"); printf("Выход.\n"); return 0;

		case MENU_clock: printf("Текущее время: День %d, %d:00\n", current_day, current_hour); break;

		case MENU_work: 
			printf("Сколько работать?\n");
			int work_hours = IntInputCheck(0,24, "Неверный ввод, нельзя столько работать");
			calculating_workHours(work_hours, &current_hour, &current_day);
			printf("Текущее время: День %d, %d:00\n", current_day, current_hour);
			break;

		case MENU_inventory: 
			for (int i = 0; i < MAX_ITEMS; i++)
				printf("Слот [%d]: %s (%d)\n", i, inventory_array[i].name, inventory_array[i].id);
			break;

		case MENU_put:
			inventory_put(inventory_array, Item_struct);
			break;

		case MENU_throw: 
			inventory_throw(inventory_array, Item_struct);
			break;

		case MENU_weight: 
			inventory_weight(inventory_array, Item_struct);
			break;

		case MENU_search:
			printf("Введите название: ");
			char text_input[MAX_NAME_LEN];
			TextInput(&text_input);
			int searching_ID = ID_Search(text_input, Item_struct);
			inventory_search(searching_ID, text_input, inventory_array, Item_struct);
			break;

		case MENU_diary:
			GenerateDiary(character_name, current_day, current_hour, inventory_array);
			printf("Дневничок записан в diary.txt\n");
			break;

		case MENU_logs:
			CheckLogs();
			break;
		}
	}

};