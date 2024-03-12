#pragma once

namespace gui::console::colors {
	constexpr inline int dark_blue = 1;
	constexpr inline int dark_green = 2;
	constexpr inline int dark_teal = 3;
	constexpr inline int dark_red = 4;
	constexpr inline int dark_pink = 5;
	constexpr inline int dark_yellow = 6;
	constexpr inline int dark_white = 7;
	constexpr inline int dark_gray = 8;
	constexpr inline int blue = 9;
	constexpr inline int green = 10;
	constexpr inline int teal = 11;
	constexpr inline int red = 12;
	constexpr inline int pink = 13;
	constexpr inline int yellow = 14;
	constexpr inline int white = 15;
	constexpr inline int _default = 15;
}

namespace gui::console::cp {
	constexpr inline int _default = 0;
	constexpr inline int utf_7 = 65000;
	constexpr inline int utf_8 = 65001;
	constexpr inline int sjis = 932;
	constexpr inline int gbk = 936;
}

namespace gui::console {

	extern void init(const char* name);

	extern void init(const wchar_t* name);

	extern void destroy();

	extern void write_ex(const char* fmt, int cp, int color, ...);

	extern void write(const char* text, int color = colors::_default);

	extern void write(const wchar_t* text, int color = colors::_default);

	extern void writeline(const char* text, int color = colors::_default);

	extern void writeline(const wchar_t* text, int color = colors::_default);

	extern void fmtwrite(const char* fmt_text, ...);

	extern void fmtwrite(const wchar_t* fmt_text, ...);

	extern void fmtcwrite(const char* fmt_text, int color, ...);

	extern void fmtcwrite(const wchar_t* fmt_text, int color, ...);
}

namespace gui::console::utf8 {
	extern void write(const char* text, int color = colors::_default);
	extern void writeline(const char* text, int color = colors::_default);
	extern void fmtwrite(const char* fmt_text, ...);
	extern void fmtcwrite(const char* fmt_text, int color, ...);
}

namespace gui::console::sjis {
	extern void write(const char* text, int color = colors::_default);
	extern void writeline(const char* text, int color = colors::_default);
	extern void fmtwrite(const char* fmt_text, ...);
	extern void fmtcwrite(const char* fmt_text, int color, ...);
}

namespace gui::console::gbk {
	extern void write(const char* text, int color = colors::_default);
	extern void writeline(const char* text, int color = colors::_default);
	extern void fmtwrite(const char* fmt_text, ...);
	extern void fmtcwrite(const char* fmt_text, int color, ...);
}

#ifdef _DEBUG
/********************* 基础函数 ***********************/

#define logd_init(...) gui::console::init     (__VA_ARGS__)
#define LOGD_INIT(...) gui::console::init     (__VA_ARGS__)
#define logd(...)      gui::console::write    (__VA_ARGS__)
#define LOGD(...)      gui::console::write    (__VA_ARGS__)
#define logd_ex(...)   gui::console::write_ex (__VA_ARGS__)
#define LOGD_EX(...)   gui::console::write_ex (__VA_ARGS__)
#define logd_ln(...)   gui::console::writeline(__VA_ARGS__)
#define LOGD_LN(...)   gui::console::writeline(__VA_ARGS__)
#define logd_fmt(...)  gui::console::fmtwrite (__VA_ARGS__)
#define LOGD_FMT(...)  gui::console::fmtwrite (__VA_ARGS__)
#define logd_fmtc(...) gui::console::fmtcwrite(__VA_ARGS__)
#define LOGD_FMTC(...) gui::console::fmtcwrite(__VA_ARGS__)
#define logd_detr	   gui::console::destroy
#define LOGD_DETR	   gui::console::destroy

/******************* UTF8编码拓展 *********************/

#define logd_utf8(...)      gui::console::utf8::write    (__VA_ARGS__)
#define LOGD_UTF8(...)      gui::console::utf8::write    (__VA_ARGS__)
#define logd_ln_utf8(...)   gui::console::utf8::writeline(__VA_ARGS__)
#define LOGD_LN_UTF8(...)   gui::console::utf8::writeline(__VA_ARGS__)
#define logd_fmt_utf8(...)  gui::console::utf8::fmtwrite (__VA_ARGS__)
#define LOGD_FMT_UTF8(...)  gui::console::utf8::fmtwrite (__VA_ARGS__)
#define logd_fmtc_utf8(...) gui::console::utf8::fmtcwrite(__VA_ARGS__)
#define LOGD_FMTC_UTF8(...) gui::console::utf8::fmtcwrite(__VA_ARGS__)

/***************** SHIFT-JIS编码拓展 ******************/

#define logd_sjis(...)      gui::console::sjis::write    (__VA_ARGS__)
#define LOGD_SJIS(...)      gui::console::sjis::write    (__VA_ARGS__)
#define logd_ln_sjis(...)   gui::console::sjis::writeline(__VA_ARGS__)
#define LOGD_LN_SJIS(...)   gui::console::sjis::writeline(__VA_ARGS__)
#define logd_fmt_sjis(...)  gui::console::sjis::fmtwrite (__VA_ARGS__)
#define LOGD_FMT_SJIS(...)  gui::console::sjis::fmtwrite (__VA_ARGS__)
#define logd_fmtc_sjis(...) gui::console::sjis::fmtcwrite(__VA_ARGS__)
#define LOGD_FMTC_SJIS(...) gui::console::sjis::fmtcwrite(__VA_ARGS__)

/******************** GBK编码拓展 *********************/

#define logd_gbk(...)       gui::console::gbk::write    (__VA_ARGS__)
#define LOGD_GBK(...)       gui::console::gbk::write    (__VA_ARGS__)
#define logd_ln_gbk(...)    gui::console::gbk::writeline(__VA_ARGS__)
#define LOGD_LN_GBK(...)    gui::console::gbk::writeline (__VA_ARGS__)
#define logd_fmt_gbk(...)   gui::console::gbk::fmtwrite (__VA_ARGS__)
#define LOGD_FMT_GBK(...)   gui::console::gbk::fmtwrite (__VA_ARGS__)
#define logd_fmtc_gbk(...)  gui::console::gbk::fmtcwrite(__VA_ARGS__)
#define LOGD_FMTC_GBK(...)  gui::console::gbk::fmtcwrite(__VA_ARGS__)

#else 
/********* 非Debug环境下则不会调用控制台函数 ************/

#define CALLNOP(...) 
#define logd_init      CALLNOP
#define LOGD_INIT      CALLNOP
#define logd_ex        CALLNOP
#define LOGD_EX        CALLNOP
#define logd           CALLNOP
#define LOGD           CALLNOP
#define logd_ln        CALLNOP
#define logd_ln        CALLNOP
#define logd_fmt       CALLNOP
#define LOGD_FMT       CALLNOP
#define logd_fmtc      CALLNOP
#define LOGD_FMTC      CALLNOP
#define logd_detr      CALLNOP
#define LOGD_DETR      CALLNOP
#define logd_utf8	   CALLNOP   
#define LOGD_UTF8	   CALLNOP
#define logd_ln_utf8   CALLNOP
#define LOGD_LN_UTF8   CALLNOP
#define logd_fmt_utf8  CALLNOP
#define LOGD_FMT_UTF8  CALLNOP
#define logd_fmtc_utf8 CALLNOP
#define LOGD_FMTC_UTF8 CALLNOP
#define logd_sjis      CALLNOP
#define LOGD_SJIS      CALLNOP
#define logd_ln_sjis   CALLNOP
#define LOGD_LN_SJIS   CALLNOP
#define logd_fmt_sjis  CALLNOP
#define LOGD_FMT_SJIS  CALLNOP
#define logd_fmtc_sjis CALLNOP
#define LOGD_FMTC_SJIS CALLNOP
#define logd_gbk       CALLNOP
#define LOGD_GBK       CALLNOP
#define logd_ln_gbk    CALLNOP
#define LOGD_LN_SJIS   CALLNOP
#define logd_fmt_gbk   CALLNOP
#define LOGD_FMT_GBK   CALLNOP
#define logd_fmtc_gbk  CALLNOP
#define LOGD_FMTC_GBK  CALLNOP

#endif // DEBUG_OUTPUT_LOGS

#define lcolor gui::console::colors
#define LCOLOR gui::console::colors
#define _GUI_CONSOLE_