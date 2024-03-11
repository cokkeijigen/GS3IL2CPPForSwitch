#pragma once
#ifndef _FILE_TOOL_
#include "FileTool.h"
#endif // !_FILE_TOOL_

namespace ScenarioSystem {

	class ScenarioReader {
		bool Init(file::data* file = nullptr);
	public:
		file::data* FileImage;
		int32_t	FileSize;
		int32_t FloatTblOfs;
		int32_t IntegerTblOfs;
		uint16_t LatestArg;
		int32_t LatestCur;
		uint8_t LatestId;
		uint8_t LatestType;
		int32_t StringTblOfs;
		int32_t TokenCur;
		int32_t TokenEnd;
		int32_t TokenOfs;
		int32_t TokenCnt;
		
		bool Load(const char* fileName);
		std::string GetString() const;
		bool FetchToken();
		bool IsToken(uint8_t type, uint8_t id) const;

		ScenarioReader() noexcept;
		~ScenarioReader()noexcept;
	};
}