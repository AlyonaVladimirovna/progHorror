#pragma once
#include <windows.h>
#include "inventory.h"
#include "player.h"

void ClearConsole(HANDLE console);

void GenerateDiary(char* character_name, int current_day, int current_hour, const InventoryItems* inventory_array);

void calculating_workHours(int work_hours, int* current_hour, int* current_day);

void CheckLogs();

void TimeSkip(PlayerInstance* ptr_Player);