#pragma once
#include "inventory.h"
#include "player.h"
#include <curses.h>

void EraseRefresh();

void W_EraseRefresh(WINDOW* window);

void GenerateDiary(char* character_name, int current_day, int current_hour, const ItemStack* inventory_array);

void calculating_workHours(int work_hours, int* current_hour, int* current_day);

void CheckLogs();

void TimeSkip(PlayerInstance* ptr_Player);