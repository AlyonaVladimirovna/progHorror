#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include "inventory.h"
#include "menu.h"
#include "consoleinput.h"
#include "keyboardinput.h"
#include "map.h"
#include "player.h"
#pragma warning(disable: 4996)



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


void Journey(PlayerInstance* ptr_Player, HANDLE console) {
	int size;
	char** map = GenerateMap(&size);
	int next_x, next_y;
	int loop_count = 0;
	int isMapWithPortal = 0;
	int home_range = 0;
	GetRandomCoords(&next_x, &next_y, size, map);
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
			GetRandomCoords(&randX, &randY, size, map);
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
			GetRandomCoords(&next_x, &next_y, size, map);
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
	

	printf("Введите имя: ");
	PlayerInstance Player;
	TextInput(&Player.name, MAX_PLAYER_NAME);

	InventoryItems Item_struct[MAX_ITEMS];
	read_item_names(Item_struct);


	srand(time(NULL));
	for (int i = 0; i < MAX_ITEMS; i++) {
		Player.inventory[i] = Item_struct[rand() % 10];
	}

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
				printf("Слот [%d]: %s (%d)\n", i, Player.inventory[i].name, Player.inventory[i].id);
			MenuReturn();
			break;

		case MENU_put:
			ClearConsole(console);
			printf("Куда положить?\n");
			int slot_put = IntInputCheck(0, 9, "Нет такого слота");
			printf("Что положить?\n");
			int new_item = IntInputCheck(Item_struct[1].id, Item_struct[9].id, "Нет такого предмета");
			inventory_put(Player.inventory, Item_struct, slot_put, new_item);
			MenuReturn();
			break;

		case MENU_throw: 
			ClearConsole(console);
			printf("С какого слота выбросить?\n");
			int slot_throw = IntInputCheck(0, 9, "Нет такого слота");
			inventory_throw(Player.inventory, Item_struct, slot_throw);
			MenuReturn();
			break;

		case MENU_weight: 
			ClearConsole(console);
			printf("Введите ID предмета\n");
			int weight_check = IntInputCheck(Item_struct[1].id, Item_struct[MAX_ITEMS - 1].id, "Нет такого предмета");
			inventory_weight(Player.inventory, Item_struct, weight_check);
			MenuReturn();
			break;

		case MENU_search:
			ClearConsole(console);
			printf("Введите название: ");
			char text_input[MAX_NAME_LEN];
			TextInput(&text_input, MAX_NAME_LEN);
			int searching_ID = ID_Search(text_input, Item_struct);
			inventory_search(searching_ID, text_input, Player.inventory, Item_struct);
			MenuReturn();
			break;

		case MENU_diary:
			ClearConsole(console);
			GenerateDiary(Player.name, current_day, current_hour, Player.inventory);
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