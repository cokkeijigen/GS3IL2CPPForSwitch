#pragma once
#define _FILE_TOOL_

namespace file {

    class data {
        bool release;
    public:
        size_t size;
        uint8_t* buffer;
        data() : data(nullptr, 0x00) { }
        data(data&& src) noexcept : data(src) { }
        data(data& src) : data(src.buffer, src.size) { src.release = false; }
        data(uint8_t* data, size_t size) : buffer(data), size(size), release(true) { }
        ~data() { if (release && buffer) delete[] buffer; }
    };
}

namespace file::tool {

    inline data read(const char* path) {
        FILE* file = fopen(path, "rb");
        if (!file) return data();
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);
        uint8_t* buf = new uint8_t[size];
        fread(buf, 1, size, file);
        fclose(file);
        return data(buf, size);
    }

    inline data* read_as_ptr(const char* path) {
        FILE* file = fopen(path, "rb");
        if (!file) return nullptr;
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);
        uint8_t* buf = new uint8_t[size];
        fread(buf, 1, size, file);
        fclose(file);
        return new data(buf, size);
    }
}