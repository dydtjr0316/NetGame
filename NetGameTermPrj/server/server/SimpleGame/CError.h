#pragma once

#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <iostream>

class CError
{
public:
	CError();
	~CError();

public:
	void err_quit(const char* msg);
	void err_display(const char* msg);
};