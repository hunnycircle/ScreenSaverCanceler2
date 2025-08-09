#include <afx.h>
#include <afxwin.h>
#include <atlstr.h>
#include <cstdlib>
#include <cstring>
#include <wchar.h>

#include "except.h"
#include "msgbox.h"
#include "ssc.h"

#include "resource.h"
#include "winmain.h"

#define _MAX_CAPTION (256)

static void loadProperties();
static ATOM registFrameWindow(HINSTANCE);
static HWND initFrameWindow(HINSTANCE);
static LRESULT mainWindowProc(HWND, UINT, WPARAM, LPARAM);

struct {
	UINT interval = 0;
	bool send_message = false;
	CString caption;
} _props;

static void loadProperties()
{
	TCHAR filepath[_MAX_PATH + 1]{};
	DWORD size = 0;

	GetModuleFileName(NULL, filepath, _MAX_PATH);
	PathRemoveFileSpec(filepath);

	wcscat_s(filepath, _MAX_PATH, L"\\ssc2.ini");

	_props.interval = GetPrivateProfileInt(L"ssc2", L"interval", 2, filepath);
	_props.send_message = (GetPrivateProfileInt(L"ssc2", L"send_message", 0, filepath) != 0);
	size = GetPrivateProfileString(L"ssc2", L"caption", L"", _props.caption.GetBuffer(_MAX_CAPTION), _MAX_CAPTION, filepath);
	_props.caption.ReleaseBuffer(size);
}

static ATOM registFrameWindow(HINSTANCE inst)
{
	WNDCLASS wc = {0};

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)mainWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = inst;
	wc.hIcon = LoadIcon(inst, (LPCWSTR)IDI_ICON_APPL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;

	return RegisterClass(&wc);
}

static HWND initFrameWindow(HINSTANCE inst)
{
	return CreateWindow(
		CLASS_NAME, APPL_NAME,
//		WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU,
		0,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
		NULL,
		NULL,
		inst,
		NULL
	);
}

static LRESULT mainWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
		case SSC_WM_NOTIFYICON:
		{
			if (lparam == WM_LBUTTONUP) {
				ssc_stateChange(hwnd, GetModuleHandle(0));
			} else if (lparam == WM_RBUTTONUP) {
				ssc_popup(hwnd);
			}
		}
		break;

		case WM_TIMER:
		{
			ssc_state();
			if (_props.send_message) ssc_sendMessage(_props.caption);
		}
		break;

		case WM_COMMAND:
		{
			if (LOWORD(wparam) == IDM_EXIT) {
				DestroyWindow(hwnd);
			}
		}
		return 0;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prev_inst, _In_ LPSTR cmdline, _In_ int cmdshow)
{
	int retval = 0;
	HWND hwnd = NULL;
	MSG msg = {0};
	UINT_PTR timer = NULL;

	try {
		AfxSetResourceHandle(inst);

		if (registFrameWindow(inst) == (ATOM)0) {
			throw SystemException(GetLastError());
		}
		if ((hwnd = initFrameWindow(inst)) == NULL) {
			throw SystemException(GetLastError());
		}

		loadProperties();

		ssc_init(hwnd, inst);

		if ((timer = SetTimer(hwnd, NULL, (_props.interval * 60 * 1000), NULL)) == NULL) {
			throw SystemException(GetLastError());
		}

		while (!0) {
			if ((retval = GetMessage(&msg, NULL, 0, 0)) >= 0) {
				if (msg.message != WM_QUIT) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				} else {
					retval = (int)msg.wParam;
					break;
				}
			} else {
				break;
			}
		}
	} catch (SystemException se) {
		msgbox_showSystemMessage(se.error_no);
		retval = se.error_no;
	}

	if (hwnd && timer) KillTimer(hwnd, timer);

	ssc_terminate(hwnd);

	return retval;
}
