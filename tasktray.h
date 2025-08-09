#pragma once

#include <windows.h>
#include <wchar.h>

void tasktray_addIcon(const HWND, const HINSTANCE, UINT, UINT, UINT, int, int, const wchar_t*);
void tasktray_removeIcon(const HWND, UINT);
void tasktray_changeIcon(const HWND, const HINSTANCE, UINT, UINT, UINT, const wchar_t*);
