#pragma once
#define _FILE_TOOL_

namespace file {

    class data {
        bool release;
    public:
        const size_t   size;
        const uint8_t* buffer;
        data() : data(nullptr, 0x00) { }
        data(data&& src) noexcept : data(src) { }
        data(data& src) : data(src.buffer, src.size) { src.release = false; }
        data(const uint8_t* data, const size_t size) : buffer(data), size(size), release(true) { }
        ~data() { if (release && buffer) delete[] buffer; }
    };

    template<typename T>
    class struct_data
    {
        bool release;
    public:
        const T* data;
        const uint8_t* buffer;
        const size_t size;
        struct_data() : struct_data(nullptr, 0x00) { }
        struct_data(struct_data&& src) noexcept : data(src) { }
        struct_data(struct_data<T>& src) : struct_data(src.buffer, src.size) { src.release = false; }
        struct_data(const uint8_t* data, const size_t size) : data((T*)data), buffer(data), size(size), release(true) { }
        ~struct_data() { 
            if (this->release && this->buffer) {
                delete[] this->buffer;
                this->buffer = nullptr;
                this->data = nullptr;
            }
        }
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

    template<typename T>
    inline struct_data<T>* read_as_struct_ptr(const char* path) {
        FILE* file = fopen(path, "rb");
        if (!file) return nullptr;
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);
        uint8_t* buf = new uint8_t[size];
        fread(buf, 1, size, file);
        fclose(file);
        return new struct_data<T>(buf, size);
    }
    
}