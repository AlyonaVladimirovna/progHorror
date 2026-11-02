#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#pragma warning(disable: 4996)

//Максимальное количество предметов
#define MAX_ITEMS 10
//Максимальная длина названия предмета
#define MAX_NAME_LEN 32
#define MAX_PLAYER_NAME 33
//Количество пунктов меню
#define MENU_COUNT 11
//Шанс появления препятствия на карте
#define OBSTRUCTION_CHANCE 15
//Сколько колец надо навернуть для того чтобы появился портал
#define MAP_LOOP_CONST 3


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
	MENU_logs = 9,
	MENU_journey= 10
};

enum KEYS {
	UP_W = 87,
	UP_A = 65,
	UP_S = 83,
	UP_D = 68,
	UP_Q = 81,
	LOW_W = 119,
	LOW_A = 97,
	LOW_S = 115,
	LOW_D = 100,
	LOW_Q =113,

	CYRILLIC_LOW_W3 = 134,
	CYRILLIC_UP_W2 = 166,
	CYRILLIC_LOW_A3 = 132,
	CYRILLIC_UP_A2 = 164,
	CYRILLIC_LOW_S3 = 139,
	CYRILLIC_UP_S2 = 171,
	CYRILLIC_LOW_D3 = 178,
	CYRILLIC_UP_D2 = 146,
	CYRILLIC_UP_Q = 153,
	CYRILLIC_LOW_Q = 185,

	CYRILLIC_PREFIX1 = 208,
	CYRILLIC_PREFIX2 = 209,
	SPEC_PREFIX = 224,
	ARROW_UP = 72,
	ARROW_DOWN = 80,
	ARROW_LEFT = 75,
	ARROW_RIGHT = 77,
	ENTER = 13,
};

enum MAPCELLS {
	CELL_empty = '.',
	CELL_tree = 'T',
	CELL_portal = 'P',
	CELL_campfire = '^',
	CELL_player = '@'
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


const char* MENU_NAMES[MENU_COUNT] = {
	[MENU_exit] = "Выход",
	[MENU_clock] = "Посмотреть на часы",
	[MENU_work] ="Поработать",
	[MENU_inventory]= "Посмотреть инвентарь",
	[MENU_put] = "Положить в инвентарь",
	[MENU_throw] = "Выбросить из инвентаря",
	[MENU_weight] = "Поиск тяжестей",
	[MENU_search] = "Поиск предмета",
 	[MENU_diary] = "Дневник",
	[MENU_logs]= "Посмотреть логи",
	[MENU_journey] = "Отправится в приключение"
};

typedef struct {
	int id;
	char name[MAX_NAME_LEN];
} InventoryItems;

typedef struct {
	char name[MAX_PLAYER_NAME];
	COORD PlayerCoord;
}PlayerInstance;

//функция сопостовления id и имени
const char* INVENTORY_nameMatch(int input, const InventoryItems nameid[]) {
		for(int i = 0; i < MAX_ITEMS; i++){
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

void ClearConsole(HANDLE console) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	DWORD written;
	COORD start = { 0, 0 };

	if (!GetConsoleScreenBufferInfo(console, &info)) return;

	DWORD size = info.dwSize.X * info.dwSize.Y;

	FillConsoleOutputCharacterA(console, ' ', size, start, &written);
	FillConsoleOutputAttribute(console, info.wAttributes, size, start, &written);
	SetConsoleCursorPosition(console, start);
}

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

int MenuKeyboardInput() {
	int key = _getch(); 
	if (key == SPEC_PREFIX) key = _getch();
	else if (key == CYRILLIC_PREFIX1 || key == CYRILLIC_PREFIX2) key = _getch();

	switch (key) {
	case ARROW_DOWN:
	case CYRILLIC_LOW_S3:
	case CYRILLIC_UP_S2:
	case LOW_S:
	case UP_S: return 1;

	case ARROW_UP:
	case CYRILLIC_LOW_W3:
	case CYRILLIC_UP_W2:
	case LOW_W:
	case UP_W: return -1;

	case ENTER: return ENTER;
	default: return 0;
	}
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
		if (menuCase < 0) menuCase = MENU_COUNT-1;
		if (menuCase > MENU_COUNT-1) menuCase = 0;
		consCoord.Y = menuCase + 1;
		SetConsoleCursorPosition(console, consCoord);
		printf("<-");
	}
	return menuCase;
}

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
	if (diary == NULL) { printf("Не удалось открыть файл дневника\n");MenuReturn(); return; }
	fprintf(diary, "Имя: %s\nВремя: День %d, %d:00\nИнвентарь: ", character_name, current_day, current_hour);
	printf("Имя: %s\nВремя: День %d, %d:00\nИнвентарь: ", character_name, current_day, current_hour);
	for (int i = 0; i < MAX_ITEMS; i++) { 
		fprintf(diary, "[%s]", inventory_array[i].name); 
		printf("[%s]", inventory_array[i].name);
	}
	printf("\n");
	fprintf(diary, "\n___\n");
	fclose(diary);
	MenuReturn();
}




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
	MenuReturn();
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
	MenuReturn();
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
	MenuReturn();
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
	MenuReturn();
}


void CheckLogs() {
	FILE* logs = fopen("files/input.txt", "r");
	if (logs == NULL) {
		printf("Не удалось открыть файл с логами.\n"); MenuReturn(); return;
	}
	else {
		char file_line[256];
		char mark[32];
		int line_number = 0;
		char log_text[192];
		FILE* output = fopen("files/output.txt", "w");
		if (output == NULL) { printf("Не удалось создать output.txt\n"); { if (logs != NULL) fclose(logs); MenuReturn(); return; } }
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
	MenuReturn();
}

char** GenerateMap(int* ptr_size) {
	*ptr_size = rand() % 25 + 5;
	char** map;
	map = (char**)malloc(*ptr_size * sizeof(char*));
	for (int i = 0; i < *ptr_size; i++) {
		map[i] = (char*)malloc(*ptr_size * sizeof(char));
	}

	for (int i = 0; i < *ptr_size; i++) {
		for (int j = 0; j < *ptr_size; j++) {
			int isObstruction = rand() % 100;
			if (isObstruction < OBSTRUCTION_CHANCE) map[i][j] = CELL_tree;
			else map[i][j] = CELL_empty;
		}
	}
	map[rand() % *ptr_size][rand() % *ptr_size] = CELL_campfire;
	return map;
}

void DeleteMap(char** map, int size) {
	for (int i = 0; i < size; i++)
		free(map[i]);
	free(map);
}

void PrintMap(char** map, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			printf("%c", map[i][j]);
		printf("\n");
	}
}

int JourneyKeyboardInput(int* ptr_nextX, int* ptr_nextY) {
	int key = _getch();
	if (key == SPEC_PREFIX) key = _getch();
	else if (key == CYRILLIC_PREFIX1 || key == CYRILLIC_PREFIX2) key = _getch();

	switch (key) {
	case ARROW_DOWN:
	case CYRILLIC_LOW_S3:
	case CYRILLIC_UP_S2:
	case LOW_S:
	case UP_S: *ptr_nextY = *ptr_nextY + 1; return 1;

	case ARROW_UP:
	case CYRILLIC_LOW_W3:
	case CYRILLIC_UP_W2:
	case LOW_W:
	case UP_W: *ptr_nextY = *ptr_nextY - 1; return 1;

	case ARROW_LEFT:
	case CYRILLIC_LOW_A3:
	case CYRILLIC_UP_A2:
	case LOW_A:
	case UP_A: *ptr_nextX = *ptr_nextX - 1; return 1;

	case ARROW_RIGHT:
	case CYRILLIC_LOW_D3:
	case CYRILLIC_UP_D2:
	case LOW_D:
	case UP_D: *ptr_nextX = *ptr_nextX + 1; return 1;


	case LOW_Q:
	case UP_Q:
	case CYRILLIC_LOW_Q:
	case CYRILLIC_UP_Q: return LOW_Q;
	default: return 1;
	};
}


void Journey(PlayerInstance* ptr_Player, HANDLE console) {
	int size;
	char** map = GenerateMap(&size);
	int next_x, next_y;
	int loop_count = 0;
	int isMapWithPortal = 0;
	int home_range = 0;
	 do {
		next_x = rand() % size,
		next_y = rand() % size;
	 } while (map[next_y][next_x] == CELL_tree);
	 ptr_Player->PlayerCoord.X = next_x;
	 ptr_Player->PlayerCoord.Y= next_y;
	PrintMap(map, size);
	printf("Для выхода нажмите Q у костра (^)");
	SetConsoleCursorPosition(console, ptr_Player->PlayerCoord);
	printf("%c", CELL_player);
	while (!(JourneyKeyboardInput(&next_x, &next_y) == LOW_Q && map[ptr_Player->PlayerCoord.Y][ptr_Player->PlayerCoord.X] == CELL_campfire)) {
		if (next_y < 0) { next_y = size - 1; loop_count++; }
		else if (next_y > size - 1) {
			next_y = 0; loop_count++;
		}
		if (next_x > size - 1) {
			next_x = 0; loop_count++;
		}
		else if (next_x < 0) { next_x = size - 1; loop_count++; }
		if (map[next_y][next_x] == CELL_tree) { 
			next_x = ptr_Player->PlayerCoord.X;
			next_y = ptr_Player->PlayerCoord.Y;
			continue; 
		}
		if (loop_count > MAP_LOOP_CONST && !isMapWithPortal) {
			int randX, randY;
			do {
				randX = rand() % size;
				randY = rand() % size;
			} while (map[randY][randX] == CELL_tree);
			map[randY][randX] = CELL_portal;
			ClearConsole(console);
			PrintMap(map, size);
			printf("Для выхода нажмите Q у костра (^)");
			isMapWithPortal = 1;
		}
		if (map[next_y][next_x] == CELL_portal) {
			home_range++;
			ClearConsole(console);
			DeleteMap(map, size);
			map = GenerateMap(&size);
			loop_count = 0;
			isMapWithPortal = 0;
			do {
				next_x = rand() % size,
				next_y = rand() % size;
			} while (map[next_y][next_x] == CELL_tree);
			ptr_Player->PlayerCoord.X = next_x;
			ptr_Player->PlayerCoord.Y = next_y;
			PrintMap(map, size);
			printf("Дальность от дома: %d\n", home_range);
			printf("Для выхода нажмите Q у костра (^)");
			SetConsoleCursorPosition(console, ptr_Player->PlayerCoord);
			printf("%c", CELL_player);
			continue;
		}

		SetConsoleCursorPosition(console, ptr_Player->PlayerCoord);
		printf("%c", map[ptr_Player->PlayerCoord.Y][ptr_Player->PlayerCoord.X]);
		ptr_Player->PlayerCoord.X = next_x;
		ptr_Player->PlayerCoord.Y = next_y;
		SetConsoleCursorPosition(console, ptr_Player->PlayerCoord);
		printf("%c", CELL_player);
	}
	ClearConsole(console);
	DeleteMap(map, size);
	printf("Прикючение закончено!\n");
	MenuReturn();
}


int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, ".UTF-8");
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD consCoord = { 0, 0 };
	int current_day = 1, current_hour = 8;
	
	InventoryItems Item_struct[MAX_ITEMS];
	read_item_names(Item_struct);


	InventoryItems inventory_array[MAX_ITEMS];
	srand(time(NULL));
	for (int i = 0; i < MAX_ITEMS; i++) {
		inventory_array[i] = Item_struct[rand() % 10];
	}


	printf("Введите имя: ");
	PlayerInstance Player;
	TextInput(&Player.name);



	int last_menuCase = 0;
	while (1) {
		ClearConsole(console);
		switch (last_menuCase = MenuSelect(console, Player.name, last_menuCase)) {

		case MENU_exit: 
			ClearConsole(console); printf("Выход.\n"); return 0;

		case MENU_clock:  ClearConsole(console); printf("Текущее время: День %d, %d:00\n", current_day, current_hour); MenuReturn(); break;

		case MENU_work:
			ClearConsole(console);
			printf("Сколько работать?\n");
			int work_hours = IntInputCheck(0,24, "Неверный ввод, нельзя столько работать");
			calculating_workHours(work_hours, &current_hour, &current_day);
			printf("Текущее время: День %d, %d:00\n", current_day, current_hour);
			MenuReturn();
			break;

		case MENU_inventory: 
			ClearConsole(console);
			for (int i = 0; i < MAX_ITEMS; i++)
				printf("Слот [%d]: %s (%d)\n", i, inventory_array[i].name, inventory_array[i].id);
			MenuReturn();
			break;

		case MENU_put:
			ClearConsole(console);
			inventory_put(inventory_array, Item_struct);
			break;

		case MENU_throw: 
			ClearConsole(console);
			inventory_throw(inventory_array, Item_struct);
			break;

		case MENU_weight: 
			ClearConsole(console);
			inventory_weight(inventory_array, Item_struct);
			break;

		case MENU_search:
			ClearConsole(console);
			printf("Введите название: ");
			char text_input[MAX_NAME_LEN];
			TextInput(&text_input);
			int searching_ID = ID_Search(text_input, Item_struct);
			inventory_search(searching_ID, text_input, inventory_array, Item_struct);
			break;

		case MENU_diary:
			ClearConsole(console);
			GenerateDiary(Player.name, current_day, current_hour, inventory_array);
			printf("Дневничок записан в diary.txt\n");
			break;

		case MENU_logs:
			ClearConsole(console);
			CheckLogs();
			break;

		case MENU_journey:
			ClearConsole(console);
			Journey(&Player, console);
			break;
		}

	}

};