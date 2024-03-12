#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <filesystem>
#include "writebuffer.h"
#include "readbuffer.h"
#include "gui_console.hpp"
#include "ScenarioReader.hpp"

namespace fsys {

	using namespace std::filesystem;
	using dir_ety = directory_entry;
	using dir_ite = directory_iterator;

	static std::string change(const path& p, std::string ext) noexcept {
		return p.stem().string().append(ext);
	}

	static bool make(const path& p) noexcept {
		return exists(p) || create_directory(p);
	}

	static std::string pcat(path p1, path p2) noexcept {
		return (p1 /= p2).string();
	}

	static void pcat(path p1, path p2, std::string& out) noexcept {
		out.assign(fsys::pcat(p1, p2));
	}

	static std::string extension(const path& p) noexcept {
		std::string ext(p.extension().string());
		transform(ext.begin(), ext.end(), ext.begin(), tolower);
		return ext;
	}

	static std::string parent(const path& p) noexcept {
		return p.parent_path().string();
	}

	static bool del(const path& p) noexcept {
		return !exists(p) || remove_all(p);
	}

	static std::string name(const path& p) noexcept {
		return p.filename().string();
	}
}

namespace worker {

	static fsys::path OUT_PATH, IN_PATH, BIN_PATH;
	static file::writebuffer writebuffer;
	static std::string config(".config");

	inline static bool read_config_if_exists() {
		std::string conf = fsys::pcat(IN_PATH, config);
		if (!fsys::exists(conf)) return false;
		file::textrbfr textrbfr(conf.c_str(), false);
		textrbfr.foreach([&](const char* line) {
			if (fsys::exists(line)) {
				worker::BIN_PATH.assign(line);
			}
			return worker::BIN_PATH.empty();
		});
		return true;
	}

	inline static void exports_as_afile(fsys::path path, int count = 1) {
		if (ScenarioSystem::ScenarioReader sr; sr.Load(path.string())) {
			if (!fsys::make(OUT_PATH)) {
				throw std::exception("Failed to make output directory.");
			}
			worker::writebuffer.clear();
			sr.Foreach([&](int, int, int arg, int cur) noexcept -> void {
				if (std::string text = sr.GetString(); !text.empty() && (*(text.c_str()) & char(128))) {
					writebuffer.fmtwrite("#0x%04X:0x%04X:0x%02X\n", arg, cur, int(text.size() + 1));
					writebuffer.fmtwrite(u8"¡ï¡ò  %03d  ¡ò¡ï//%s\n", count, text.c_str());
					writebuffer.fmtwrite(u8"¡ï¡ò  %03d  ¡ò¡ï%s\n\n", count, text.c_str());
					count++;
				}
			});
			std::string name = fsys::change(path, ".txt");
			std::string out  = fsys::pcat(OUT_PATH, name);
			if (writebuffer.size() && writebuffer.save(out.c_str())) {
				gui::console::fmtcwrite(
					"[+] Saved: %s\n", lcolor::yellow, name.c_str()
				);
			}
			else {
				gui::console::fmtcwrite(
					"[-] Failed: %s\n", lcolor::red, name.c_str()
				);
			}
		}
	}

	inline static void exports_as_multifile() {
		for (const fsys::dir_ety& file : fsys::dir_ite(IN_PATH)) {
			exports_as_afile(file.path());
		}
	}

	inline static void run_text_imports() {
		gui::console::writeline("This feature has not been written yet...", lcolor::yellow);
	}

	static void start(fsys::path path1, fsys::path path2) try {
		OUT_PATH.assign(path1.parent_path()), IN_PATH.assign(path2);
		if (worker::read_config_if_exists()) {
			gui::console::writeline("\n===== [Import Mode] =====\n");
			if (worker::BIN_PATH.empty()) {
				throw std::exception(
					"The binary file (.bytes) directory in the .config file does not exist."
				);
			}
			fsys::del(OUT_PATH.append("OutputFiles"));
			worker::run_text_imports();
		}
		else {
			gui::console::writeline("\n===== [Export Mode] =====\n");
			fsys::del(OUT_PATH.append("OutputText"));
			if (fsys::is_directory(IN_PATH)) {
				worker::exports_as_multifile();
			}
			else if (fsys::exists(IN_PATH)) {
				worker::exports_as_afile(IN_PATH);
				IN_PATH.assign(IN_PATH.parent_path());
			}
			else {
				throw std::exception("The input path does not exist.");
			}
			if (fsys::exists(OUT_PATH)) {
				worker::config.assign(fsys::pcat(OUT_PATH, config));
				writebuffer.clear().fmtwrite("%s", IN_PATH.string().c_str());
				writebuffer.save(worker::config.c_str());
			}
		}
	}
	catch (std::exception& ex) {
		gui::console::fmtcwrite(
			"[Error] %s\n", lcolor::red, ex.what()
		);
	}
}

int main(int argc, char* argv[]) {
	gui::console::init("ScenarioHelper Beta 0.5 By iTsukezigen");
	if (argc == 2) {
		worker::start(argv[0], argv[1]);
	}
	else {
		gui::console::write("\n[Error] Illegal parameter!!!\n", lcolor::red);
	}
	gui::console::write("\n========= [End] =========\n");
	std::cin.ignore();
	gui::console::destroy();
	return 0;
}