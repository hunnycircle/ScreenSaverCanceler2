#pragma once

#include <windows.h>
#include <wchar.h>

#define SSC_WM_NOTIFYICON (WM_USER + 100)

void ssc_init(HWND, HINSTANCE);
void ssc_terminate(HWND);
void ssc_popup(HWND);
void ssc_stateChange(HWND, HINSTANCE);
void ssc_state(void);
void ssc_sendMessage(const wchar_t*);
