#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "ScenarioReader.hpp"

namespace ScenarioSystem {

	struct scen_hdr {
		int32_t TokenOfs;
		int32_t IntegerTblOfs;
		int32_t FloatTblOfs;
		int32_t StringTblOfs;
	};

	ScenarioReader::ScenarioReader() noexcept {
		(void)this->Init();
	}

	ScenarioReader::~ScenarioReader() noexcept {
		(void)this->Init();
	}

	bool ScenarioReader::Init(file::data* file) {
		if (this->FileImage) delete this->FileImage;
		this->FileImage  = file;
		this->FileSize   = 0x00;
		this->LatestType = 0x00;
		this->LatestArg  = 0x00;
		this->LatestCur  = 0x00;
		this->LatestId   = 0x00;
		this->TokenCur = 0x00;
		this->TokenEnd = 0x00;
		this->TokenOfs = 0x00;
		this->TokenCnt = 0x00;
		this->FloatTblOfs   = 0x00;
		this->IntegerTblOfs = 0x00;
		this->StringTblOfs  = 0x00;
		return file && file->buffer && file->size;
	}

	bool ScenarioReader::Load(std::string file) {
		if (this->Init(file::tool::read_as_ptr(file.c_str()))) {
			this->FileSize = int32_t(this->FileImage->size);
			scen_hdr* hdr_field = (scen_hdr*)this->FileImage->buffer;
			if (hdr_field->TokenOfs != 0x10) return false;
			this->TokenOfs = hdr_field->TokenOfs;
			this->IntegerTblOfs = hdr_field->IntegerTblOfs;
			this->FloatTblOfs = hdr_field->FloatTblOfs;
			this->StringTblOfs = hdr_field->StringTblOfs;
			if (this->IntegerTblOfs - 0x10 >= 0) {
				this->TokenEnd = (this->IntegerTblOfs - 0x10) >> 2;
			}
			else {
				this->TokenEnd = (this->IntegerTblOfs - 0x0D) >> 2;
			}
			return true;
		}
		return false;
		
	}

	bool ScenarioReader::IsToken(uint8_t type, uint8_t id) const {
		return this->LatestType == type && this->LatestId == id;
	}

	bool ScenarioReader::FetchToken() {
		int32_t TokenEnd = this->TokenEnd;
		int32_t TokenCur = this->TokenCur;
		if (TokenCur < TokenEnd && this->FileImage) {
			uint32_t offset = this->TokenOfs + (TokenCur * 0x04);
			uint32_t length = this->FileImage->size;
			uint8_t* buffer = this->FileImage->buffer;
			if (offset + 2 < length && offset + 3 < length) {
				this->LatestType = *(uint8_t*)(buffer + offset + 0);
				this->LatestId = *(uint8_t*)(buffer + offset + 1);
				uint8_t v6 = *(uint8_t*)(buffer + offset + 2);
				uint8_t v7 = *(uint8_t*)(buffer + offset + 3);
				this->LatestArg = v7 | (v6 << 8);
				++this->TokenCur;
			}
		}
		this->LatestCur = TokenCur;
		return TokenCur < TokenEnd;
	}

	std::string ScenarioReader::GetString() const {
		uint8_t* buffer = this->FileImage->buffer;
		uint32_t length = this->FileImage->size;
		uint32_t offset = this->StringTblOfs + this->LatestArg;
		if (this->IsToken(0x05, 0x03) && offset < length) {
			if (std::string text((char*)(buffer + offset)); !text.empty()) {
				uint8_t arg = uint8_t(this->LatestArg), val;
				char* enc_str = (char*)text.c_str();
				while (*enc_str)
				{
					if (val = *enc_str ^ (arg | 0x3C)) {
						*enc_str = char(val);
					}
					enc_str++;
				}
				return text;
			}
		}
		return std::string();
	}

	void ScenarioReader::Foreach(std::function<void(int type, int id, int arg, int cur)> callback) {
		while (this->FetchToken()) callback(this->LatestType, this->LatestId, this->LatestArg, this->LatestCur);
	}
}
