#pragma once
#include <windows.h>
#include <stdio.h>
#include <string>

#pragma once

#include <iostream>
#include <windows.h>

class Borland {

public:
	static void Initialize()
	{
//		std::system("mode con:cols=160 lines=49");
//		std::system("chcp 437");

		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 25;
		cci.bVisible = false;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	}

	static int WhereX()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.X;
	}
	static int WhereY()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}
	static void GotoXY(int x, int y)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)x, (SHORT)y });
	}
};

class ChatWindow
{
private:
    int maxLine;
    int curLine;

public:
    ChatWindow() : maxLine(16), curLine(0)
    {
		Borland::Initialize();
    } 

    void init()
    {
        system("cls");
		Borland::GotoXY(0, 17);
        printf("----------------------------------------------------------------");
		Borland::GotoXY(0, 18);
    }

    void DrawMessage(const char* inLog, ...)
    {
		Borland::GotoXY(0, curLine++);
		va_list ap;
		va_start(ap, inLog);
		vfprintf(stderr, inLog, ap);
		va_end(ap);

		if (curLine == maxLine)
		{
			for (int i = 0; i < maxLine; i++)
			{
				ClearLine(i);
			}
			curLine = 0;
		}
		Borland::GotoXY(0, maxLine + 2);
    }

	void ClearLine(int line)
	{
		Borland::GotoXY(0, line);
		printf("                                                   \r");

	}
    ~ChatWindow() {}
};