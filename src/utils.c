#include "utils.h"
#include "menu.h"
#include <stdio.h>

//я гребанный ~~архитектор~~ программист
void EraseRefresh() {
	erase();
	refresh();
}

//я гребанный ~~архитектор~~ программист мать твою 2
void W_EraseRefresh(WINDOW* window) {
	werase(window);
	wrefresh(window);
}


void GenerateDiary(char* character_name, int current_day, int current_hour, const ItemStack* inventory_array, const ItemsDict Item_struct[]) {
	FILE* diary = fopen("files/diary.txt", "a");
	if (diary == NULL) { printw("Не удалось открыть файл дневника\n");MenuReturn(); return; }
	fprintf(diary, "Имя: %s\nВремя: День %d, %d:00\nИнвентарь: ", character_name, current_day, current_hour);
	printw("Имя: %s\nВремя: День %d, %d:00\nИнвентарь: ", character_name, current_day, current_hour);
	for (int i = 0; i < MAX_ITEMS; i++) {
		fprintf(diary, "[%s]", INVENTORY_nameMatch(inventory_array[i].id, Item_struct));
		printw("[%s]", INVENTORY_nameMatch(inventory_array[i].id, Item_struct));
	}
	printw("\n"); refresh();
	fprintf(diary, "\n___\n");
	fclose(diary);
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
		printw("Не удалось открыть файл с логами.\n"); MenuReturn(); return;
	}
	else {
		char file_line[256];
		char mark[32];
		int line_number = 0;
		char log_text[192];
		FILE* output = fopen("files/output.txt", "w");
		if (output == NULL) { printw("Не удалось создать output.txt\n"); { if (logs != NULL) fclose(logs); MenuReturn(); return; } }
		while (fgets(file_line, sizeof(file_line), logs) != NULL) {
			sscanf_s(file_line, "%d [%31[^]]] %191[^\r\n]", &line_number, mark, (unsigned)sizeof(mark), log_text, (unsigned)sizeof(log_text));
			if (!strcmp(mark, "WARN") || !strcmp(mark, "ERROR")) {
				fprintf(output, "%d %s\n", line_number, log_text);
				printw("%d %s\n", line_number, log_text);
			}
		}
		refresh();
		if (output != NULL) fclose(output);
	}
	if (logs != NULL) fclose(logs);

}

void TimeSkip(PlayerInstance* ptr_Player) {
	if (ptr_Player == NULL) { printw("Указатель на PlayerInstance равен NULL\n"); return; }
	int hours;
	while (1) {
		printw("Текущее время: День %d, %d:00 \nВведите сколько ждать: ", ptr_Player->current_day, ptr_Player->current_hour);
		refresh();
		hours = IntInputCheck(0, 24, "Неверный ввод, введите часы от 0 до 24, вы можете прождать только до 2:00\n");
		if ((0 <= ptr_Player->current_hour && ptr_Player->current_hour < 2 && ptr_Player->current_hour + hours < 2) || \
			(7 < ptr_Player->current_hour && ptr_Player->current_hour < 24 && ptr_Player->current_hour + hours < 26)) {
			if (ptr_Player->current_hour + hours > 23) {
				ptr_Player->current_day += 1;
				ptr_Player->current_hour = (ptr_Player->current_hour + hours) % 24;
				break;
			}
			else { ptr_Player->current_hour = ptr_Player->current_hour + hours; break; }
		}
		if (YesNoInput()) continue;
		else return;
	}
	printw("Подождал %d Часов. Текущее время: День %d, %d:00\n", hours, ptr_Player->current_day, ptr_Player->current_hour);
	refresh();
}