# GS3IL2CPPForSwitch
```cpp
	struct ScriptHeader {
		int32_t TokenOfs;
		int32_t IntegerTblOfs;
		int32_t FloatTblOfs;
		int32_t StringTblOfs;
		struct Token {
			uint8_t Type, Id, v1, v2;
			inline uint16_t&& Arg() {
				return v2 | (v1 << 8);
			}
		} TokenTbl[0];
	};
}
```
## ScenarioHelper
This tool can re import text within game scripts. (Currently only exporting text is supported)<br>
