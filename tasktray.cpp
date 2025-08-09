#include <windows.h>
#include <wchar.h>
#include <shellapi.h>

#include "except.h"

#include "tasktray.h"

void tasktray_addIcon(const HWND hwnd, const HINSTANCE inst, UINT id, UINT icon, UINT msgid, int retry, int interval, const wchar_t* tips)
{
	int i = 0;
	NOTIFYICONDATA nid = {0};

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = id;
	nid.uFlags = (NIF_MESSAGE | NIF_ICON | NIF_TIP);
	nid.uCallbackMessage = msgid;
	nid.hIcon = LoadIcon(inst, MAKEINTRESOURCE(icon));
	wcsncpy_s(nid.szTip, _countof(nid.szTip), tips, _TRUNCATE);

	for (i = 0; i < retry; i++) {
		if (Shell_NotifyIcon(NIM_ADD, &nid) == TRUE) {
			break;
		}
		else {
			Sleep(interval);
		}
	}

	if (i >= retry) {
		throw SystemException(GetLastError());
	}
}

void tasktray_removeIcon(const HWND hwnd, UINT id)
{
	NOTIFYICONDATA nid = {0};

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = id;

	Shell_NotifyIcon(NIM_DELETE, &nid);

	if (nid.hIcon) {
		DestroyIcon(nid.hIcon);
	}
}

void tasktray_changeIcon(const HWND hwnd, const HINSTANCE inst, UINT iD, UINT icon, UINT msgid, const wchar_t* tips)
{
	NOTIFYICONDATA		nid;

	ZeroMemory(&nid, sizeof(NOTIFYICONDATA));

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = iD;
	nid.uFlags = (NIF_MESSAGE | NIF_ICON | NIF_TIP);
	nid.uCallbackMessage = msgid;
	nid.hIcon = LoadIcon(inst, MAKEINTRESOURCE(icon));
	wcsncpy_s(nid.szTip, (sizeof(nid.szTip) / sizeof(wchar_t)), tips, _TRUNCATE);

	Shell_NotifyIcon(NIM_MODIFY, &nid);
}
