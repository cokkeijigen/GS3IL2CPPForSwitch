#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "FileTool.h"
#include "ScenarioReader.hpp"
#include "gui_console.hpp"
#define TestPath "F:\\COKEZIGE\\StudioWork\\gs3_switch\\gs3_scenario\\scenario\\f\\TextAsset\\"



int main(int, char**) {
	gui::console::init("ScenarioReader Test");
	if (ScenarioSystem::ScenarioReader sr; sr.Load(TestPath"f01_01_000.bytes")) {
		while (sr.FetchToken())
		{
			if (!sr.IsToken(5, 3)) continue;
			if (std::string text = sr.GetString(); !text.empty()) {
				if (!(*(text.c_str()) & char(128))) continue;
				gui::console::utf8::fmtwrite(
					"Index: 0x%04X Text: %s\n", sr.LatestCur, text.c_str()
				);
			}
		}
	}
	gui::console::destroy();
	return 0;
}