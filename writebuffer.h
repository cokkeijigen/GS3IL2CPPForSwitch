#pragma once

namespace file {

    class writebuffer {
        va_list fmtvals = NULL;
    protected:
        uint8_t* buffer;
        size_t bfsize;
        size_t length;
        size_t exsize;
        size_t initsz;
    private:
        void initialize(bool init) {
            if (buffer) delete[] buffer;
            bfsize = initsz;
            length = 0x00;
            if (!init) return;
            buffer = new uint8_t[initsz];
        }

        void expandbuffer(size_t bfsz) {
            if (bfsz == NULL) {
                bfsize = bfsize + exsize;
            }
            else {
                size_t bfnsz = length + bfsz;
                if (bfnsz < bfsize) return;
                while (bfnsz >= (bfsize += exsize));
            }
            uint8_t* nbuf = new uint8_t[bfsize];
            if (length && buffer) {
                memcpy(nbuf, buffer, length);
                delete[] buffer;
            }
            buffer = nbuf;
        }

    public:
        writebuffer() : writebuffer(1024, 500) {}
        writebuffer(size_t initsz) : writebuffer(initsz, 500) {}
        writebuffer(size_t initsz, size_t exsize) : initsz(initsz), exsize(exsize) {
            this->initialize(true);
        }
        ~writebuffer() { this->initialize(false); }

        writebuffer& write(const void* buf, size_t bfsz) {
            this->expandbuffer(bfsz);
            memcpy(&buffer[length], buf, bfsz);
            length = length + bfsz;
            return *this;
        }

        writebuffer& overwrite(size_t pos, const void* bytes, size_t bfsz) {
            if (pos + bfsz >= this->bfsize || pos + bfsz >= this->length) {
                this->length = pos;
                return this->write(bytes, bfsz);
            }
            else {
                memcpy(&this->buffer[pos], bytes, bfsz);
                return *this;
            }
        }

        writebuffer& overwrite(size_t pos, uint8_t val, size_t num) {
            if (pos + num >= this->bfsize) this->expandbuffer(num);
            if (pos + num > this->length) this->length = pos + num;
            for (size_t i = pos; i < pos + num; i++) buffer[i] = val;
            return *this;
        }

        writebuffer& overwrite(size_t start, size_t end, int8_t val) {
            return this->overwrite(start, (int8_t)val, end - start);
        }
        
        writebuffer& fmtwrite(const char* fmt, ...) {
            __crt_va_start(this->fmtvals, fmt);
            if (int strSz = std::vsnprintf(nullptr, 0, fmt, this->fmtvals); strSz > 0) {
                this->expandbuffer(strSz + 1);
                char* bufbeg = (char*)&buffer[this->length];
                std::vsnprintf(bufbeg, strSz + 1, fmt, this->fmtvals);
                this->length += strSz;
            }
            __crt_va_end(this->fmtvals);
            return *this;
        }

        uint8_t* data() const {
            return this->buffer;
        }

        size_t size() const {
            return this->length;
        }

        bool save(const char* path) {
            if (!this->length) return false;
            FILE* outfp = fopen(path, "wb");
            if (outfp == NULL) return false;
            fwrite(buffer, 1, length, outfp);
            fclose(outfp);
            return true;
        }

        writebuffer& clear() {
            if (length) this->initialize(true);
            return *this;
        }
    };
}