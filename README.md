# GS3IL2CPPForSwitch
```cpp
struct ScriptHeader {
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
```
## ScenarioHelper
This tool can re import text within game scripts. (Currently only exporting text is supported)<br>
