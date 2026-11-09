#include "consoleinput.h"
#include <stdio.h>
#include <string.h>
#pragma warning(disable: 4996)

//Ввод чисел (да он понятия не имеет как работать с текстом)
int IntInputCheck(int low_limit, int up_limit, const char* error_message) {
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
void TextInput(char* text, int max_len) {

	while (1) {
		if (fgets(text, max_len, stdin) == NULL) {
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