#include <afx.h>
#include <afxwin.h>
#include <cstdlib>

#include "msgbox.h"

void msgbox_showSystemMessage(DWORD error_no)
{
	LPVOID msg = NULL;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errno, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&msg, 0, NULL) != 0) {
		AfxMessageBox((LPCTSTR)msg);
		LocalFree(msg);
	}
}
