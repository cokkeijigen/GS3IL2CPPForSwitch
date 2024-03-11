#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include "gui_console.hpp"

namespace gui::console {

	static HANDLE HdConsole = NULL;
	static HWND ConsoleHWND = NULL;
	static va_list FmtVargs = NULL;
	static struct { char* buf; int size; } acharBuffer { nullptr, 0 };
	static struct { wchar_t* buf; int size; } wcharBuffer { nullptr, 0 };

	inline static bool check() {
		return console::ConsoleHWND && console::HdConsole;
	}

	static void __Init_Console_Ex() {
		(void)AllocConsole();
		console::HdConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		console::ConsoleHWND = GetConsoleWindow();
		(void)ShowWindow(console::ConsoleHWND, SW_SHOW);
	}

	static void __Clear_Console_Output(CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 }, DWORD cellsWritten = 0) {
		(void)GetConsoleScreenBufferInfo(console::HdConsole, &csbi);
		(void)SetConsoleCursorPosition(console::HdConsole, { 0, 0 });
		(void)FillConsoleOutputCharacter(console::HdConsole, ' ',
			csbi.dwSize.X * csbi.dwSize.Y, { 0, 0 }, &cellsWritten
		);
		(void)FillConsoleOutputAttribute(console::HdConsole, csbi.wAttributes,
			csbi.dwSize.X * csbi.dwSize.Y, { 0, 0 }, &cellsWritten
		);
	}

	void console::init(const char* ConsoleName) {
		console::__Init_Console_Ex();
		(void)SetConsoleTitleA(ConsoleName);
		console::__Clear_Console_Output();
	}

	void console::init(const wchar_t* ConsoleName) {
		if (!console::check()) console::__Init_Console_Ex();
		(void)SetConsoleTitleW(ConsoleName);
		console::__Clear_Console_Output();
	}

	void console::destroy() {
		if (console::ConsoleHWND) {
			(void)DestroyWindow(console::ConsoleHWND);
			console::ConsoleHWND = NULL;
		}
		if (console::HdConsole) {
			(void)FreeConsole();
			console::HdConsole = NULL;
		}
		if (console::acharBuffer.buf) {
			delete[] console::acharBuffer.buf;
			console::acharBuffer = { nullptr, 0 };
		}
		if (console::wcharBuffer.buf) {
			delete[] console::wcharBuffer.buf;
			console::wcharBuffer = { nullptr, 0 };
		}
	}

	static void __fmt_write_asEx(const char* fmt, int n_CodePageID, int color) {
		if (!console::check() || !console::FmtVargs) return;
		if (int strSz = std::vsnprintf(nullptr, 0, fmt, console::FmtVargs); strSz > 0) {
			if (!console::acharBuffer.buf || console::acharBuffer.size <= strSz) {
				if (console::acharBuffer.buf) delete[] console::acharBuffer.buf;
				console::acharBuffer = { new char[strSz + 1], strSz + 1 };
			}
			(void)std::vsnprintf(console::acharBuffer.buf, console::acharBuffer.size, fmt, console::FmtVargs);
			SetConsoleTextAttribute(console::HdConsole, color);
			UINT o_CodePageID = GetConsoleCP();
			SetConsoleOutputCP(n_CodePageID);
			WriteConsoleA(console::HdConsole, console::acharBuffer.buf, strSz, NULL, NULL);
			SetConsoleTextAttribute(console::HdConsole, colors::_default);
			SetConsoleOutputCP(o_CodePageID);
		}
		console::FmtVargs = NULL;
	}

	static void __fmt_write_asEx(const wchar_t* fmt, int color) {
		if (!console::check() || !console::FmtVargs) return;
		if (int strSz = std::vswprintf(nullptr, 0, fmt, console::FmtVargs); strSz > 0) {
			if (!console::wcharBuffer.buf || console::wcharBuffer.size <= strSz) {
				if (console::wcharBuffer.buf) delete[] console::wcharBuffer.buf;
				console::wcharBuffer = { new wchar_t[strSz + 1], strSz + 1 };
			}
			(void)std::vswprintf(console::wcharBuffer.buf, console::wcharBuffer.size, fmt, console::FmtVargs);
			SetConsoleTextAttribute(console::HdConsole, color);
			WriteConsoleW(console::HdConsole, console::wcharBuffer.buf, strSz, NULL, NULL);
			SetConsoleTextAttribute(console::HdConsole, colors::_default);
		}
		console::FmtVargs = NULL;
	}

	void write_ex(const char* fmt, int cp, int color, ...) {
		__crt_va_start(console::FmtVargs, color);
		__fmt_write_asEx(fmt, cp, color);
		__crt_va_end(console::FmtVargs);
	}

	void console::fmtwrite(const char* fmt_text, ...) {
		__crt_va_start(console::FmtVargs, fmt_text);
		__fmt_write_asEx(fmt_text, console::cp::_default, colors::_default);
		__crt_va_end(console::FmtVargs);
	}

	void console::fmtcwrite(const char* fmt_text, int color, ...) {
		__crt_va_start(console::FmtVargs, color);
		__fmt_write_asEx(fmt_text, console::cp::_default, color);
		__crt_va_end(console::FmtVargs);
	}

	void console::write(const char* text, int color) {
		console::fmtcwrite("%s", color, text);
	}

	void console::write(const wchar_t* text, int color) {
		console::fmtcwrite(L"%s", color, text);
	}

	void console::writeline(const char* text, int color) {
		console::fmtcwrite("%s\n", color, text);
	}

	void console::writeline(const wchar_t* text, int color) {
		console::fmtcwrite(L"%s\n", color, text);
	}

	void console::fmtwrite(const wchar_t* fmt_text, ...) {
		__crt_va_start(console::FmtVargs, fmt_text);
		__fmt_write_asEx(fmt_text, colors::_default);
		__crt_va_end(console::FmtVargs);
	}

	void console::fmtcwrite(const wchar_t* fmt_text, int color, ...) {
		__crt_va_start(console::FmtVargs, color);
		__fmt_write_asEx(fmt_text, color);
		__crt_va_end(console::FmtVargs);
	}
}

namespace gui::console::utf8 {

	void write(const char* text, int color) {
		console::utf8::fmtcwrite("%s", color, text);
	}

	void writeline(const char* text, int color) {
		console::utf8::fmtcwrite("%s\n", color, text);
	}

	void fmtwrite(const char* fmt_text, ...) {
		__crt_va_start(console::FmtVargs, fmt_text);
		console::__fmt_write_asEx(
			fmt_text, console::cp::utf_8, colors::_default
		);
		__crt_va_end(console::FmtVargs);
	}

	void fmtcwrite(const char* fmt_text, int color, ...) {
		__crt_va_start(console::FmtVargs, color);
		console::__fmt_write_asEx(
			fmt_text, console::cp::utf_8, color
		);
		__crt_va_end(console::FmtVargs);
	}
}

namespace gui::console::sjis {

	void write(const char* text, int color) {
		console::sjis::fmtcwrite("%s", color, text);
	}

	void writeline(const char* text, int color) {
		console::sjis::fmtcwrite("%s\n", color, text);
	}

	void fmtwrite(const char* fmt_text, ...) {
		__crt_va_start(console::FmtVargs, fmt_text);
		console::__fmt_write_asEx(
			fmt_text, console::cp::sjis, colors::_default
		);
		__crt_va_end(console::FmtVargs);
	}

	void fmtcwrite(const char* fmt_text, int color, ...) {
		__crt_va_start(console::FmtVargs, color);
		console::__fmt_write_asEx(
			fmt_text, console::cp::sjis, color
		);
		__crt_va_end(console::FmtVargs);
	}
}

namespace gui::console::gbk {

	void write(const char* text, int color) {
		console::gbk::fmtcwrite("%s", color, text);
	}

	void writeline(const char* text, int color) {
		console::gbk::fmtcwrite("%s\n", color, text);
	}

	void fmtwrite(const char* fmt_text, ...) {
		__crt_va_start(console::FmtVargs, fmt_text);
		console::__fmt_write_asEx(
			fmt_text, console::cp::gbk, colors::_default
		);
		__crt_va_end(console::FmtVargs);
	}

	void fmtcwrite(const char* fmt_text, int color, ...) {
		__crt_va_start(console::FmtVargs, color);
		console::__fmt_write_asEx(
			fmt_text, console::cp::gbk, color
		);
		__crt_va_end(console::FmtVargs);
	}
}
