#include <windows.h>
#include <cstdlib>
#include <wchar.h>

#include "except.h"
#include "tasktray.h"
#include "winmain.h"

#include "resource.h"
#include "ssc.h"

#define _RETRY (20)
#define _INTERVAL (2000)

static HMENU _menu = NULL;
static bool _run = true;

void ssc_init(HWND hwnd, HINSTANCE inst)
{
	if ((_menu = LoadMenu(inst, MAKEINTRESOURCE(IDR_MENU_POPUP))) == NULL) {
		throw SystemException(GetLastError());
	}

	tasktray_addIcon(hwnd, inst, IDI_ICON_ON, IDI_ICON_ON, SSC_WM_NOTIFYICON, _RETRY, _INTERVAL, APPL_NAME);
}

void
ssc_terminate(HWND hwnd)
{
	if (_menu) {
		DestroyMenu(_menu);
		_menu = NULL;
	}

	tasktray_removeIcon(hwnd, IDI_ICON_ON);
}

void
ssc_popup(HWND hwnd)
{
	POINT point = {0};

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	GetCursorPos(&point);

	TrackPopupMenuEx(GetSubMenu(_menu, 0), TPM_CENTERALIGN | TPM_VCENTERALIGN | TPM_RIGHTBUTTON, point.x, point.y, hwnd, NULL);
	PostMessage(hwnd, WM_NULL, 0, 0);
}

void
ssc_stateChange(HWND hwnd, HINSTANCE inst)
{
	UINT icon = 0;

	_run = !_run;
	icon = (_run) ? IDI_ICON_ON : IDI_ICON_OFF;

	tasktray_changeIcon(hwnd, inst, IDI_ICON_ON, icon, SSC_WM_NOTIFYICON, APPL_NAME);
}

void ssc_state()
{
	INPUT input[1]{};

	if (!_run) return;

	input[0].type = INPUT_MOUSE;
	input[0].mi.dwFlags = MOUSEEVENTF_MOVE;
	input[0].mi.dx = 0;
	input[0].mi.dy = 0;
	SendInput(1, input, sizeof(input));
}

void ssc_sendMessage(const wchar_t* caption)
{
	HWND hwnd = NULL;
	DWORD target_thread = 0;
	DWORD my_thread = 0;
	INPUT input = { 0 };

	if (!_run) return;
	if ((hwnd = FindWindow(NULL, caption)) == NULL) return;
	if ((target_thread = GetWindowThreadProcessId(hwnd, NULL)) == 0) return;

	my_thread = GetCurrentThreadId();

	if (AttachThreadInput(my_thread, target_thread, TRUE) != 0) {
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.mouseData = 1;
		SendInput(1, &input, sizeof(input));
		AttachThreadInput(my_thread, target_thread, FALSE);
	}
}
