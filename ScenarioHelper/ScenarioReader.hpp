#pragma once
#ifndef _FILE_TOOL_
#include "FileTool.h"
#endif // !_FILE_TOOL_
#ifndef _FUNCTIONAL_
#include <functional>
#endif

namespace ScenarioSystem {

	struct ScenHdr {
		int32_t TokenOfs;
		int32_t IntegerTblOfs;
		int32_t FloatTblOfs;
		int32_t StringTblOfs;
		struct Token {
			uint8_t Type, Id, v1, v2;
			inline auto Arg() -> uint16_t&& { return v2 | (v1 << 8); }
			inline auto Arg(uint16_t arg)   { v1 = uint8_t(arg >> 8), v2 = uint8_t(arg & 0x00FF); }
		} TokenTbl[0];
	};

	class ScenarioReader {
		bool Init(file::data* file = nullptr);
	public:
		file::data* FileImage;
		ScenHdr* ScenHdr;
		uint16_t LatestArg;
		int32_t LatestCur;
		uint8_t LatestId;
		uint8_t LatestType;
		int32_t TokenCur;
		int32_t TokenEnd;
		int32_t TokenCnt;
		
		bool Load(std::string fileName);
		std::string GetString() const;
		bool FetchToken();
		bool IsToken(uint8_t type, uint8_t id) const;

		void Foreach(std::function<void(int type, int id, int arg, int cur)> callback);

		ScenarioReader() noexcept;
		~ScenarioReader()noexcept;
	};
}