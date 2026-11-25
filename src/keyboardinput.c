#include "keyboardinput.h"
#include <curses.h>

int MenuKeyboardHandler() {
	int key = getch();
	if (key == ERR) return NULL;

	switch (key) {
	case KEY_DOWN:
	case CYRILLIC_LOW_S3:
	case CYRILLIC_UP_S2:
	case LOW_S:
	case UP_S: return 1;

	case KEY_UP:
	case CYRILLIC_LOW_W3:
	case CYRILLIC_UP_W2:
	case LOW_W:
	case UP_W: return -1;

	case KEY_ENTER:
	case '\n':
	case '\r': return ENTER;
	default: return 0;
	}
}



int JourneyKeyboardHandler(int* ptr_nextX, int* ptr_nextY) {
	int key = getch();
	if (key == ERR) return NULL;

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

	case LOW_M: return LOW_M;

	default: return 1;
	};
}