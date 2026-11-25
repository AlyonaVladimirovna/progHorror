#include "consoleinput.h"
#include <stdio.h>
#include <string.h>
#include <curses.h>
#pragma warning(disable: 4996)

//Ввод чисел (да он понятия не имеет как работать с текстом)
int IntInputCheck(int low_limit, int up_limit, const char* error_message) {
	echo();
	int input = 0;
	int error_flag = 0;
	while (1) {
		error_flag = scanw("%d", &input);
		if (error_flag == ERR || error_flag == 0 || input > up_limit || input < low_limit)
		{
			printw("%s\n", error_message);
			refresh();
			flushinp();
		}
		else {
			flushinp();
			noecho();
			return input;
		}
	};
};

//Ввод текста (да он понятия не имеет как работать с числами(на самом деле имеет, но записывает их как символы))
void TextInput(char* text, int max_len) {
	echo();
	while (1) {
		if (getnstr(text, max_len) == ERR) {
			printw("Имя слишком длинное\n"); 
			refresh();
		}
		else break;
	}
	noecho();
};


int YesNoInput() {
	echo();
	char answer[16];
	TextInput(&answer, 16);
	while (strcmp(answer, "y") && strcmp(answer, "n")) {
		printw("неверный ввод, попробуйте снова\n");
		refresh();
		TextInput(&answer, 16);
	}
	noecho();
	if (!strcmp(answer, "y")) return 1;
	else return 0;
}