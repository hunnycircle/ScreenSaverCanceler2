#pragma once

#include <windows.h>

class SystemException
{
public:
	DWORD error_no;

public:
	SystemException(DWORD no) {error_no = no;};
};
