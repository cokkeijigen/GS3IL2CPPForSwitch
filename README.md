# GS3IL2CPPForSwitch
```cpp
struct ScriptHeader {
  int32_t TokenOfs;
  int32_t IntegerTblOfs;
  int32_t FloatTblOfs;
  int32_t StringTblOfs;
  struct {
    uint8_t type;
    uint8_t id;
    int16_t offset; // Little Endian
  }* Tokens;
}
```
在写了在写了...
