#include "signature.h"

const char *ChimeraSignature::name() {
    return this->i_name.data();
}

unsigned char *ChimeraSignature::address() {
    return this->i_found_address;
}

void ChimeraSignature::undo() {
    if(this->i_found_address == NULL) return;
    DWORD a;
    DWORD b;
    size_t length = this->size();
    if(VirtualProtect(this->i_found_address, length, PAGE_READWRITE, &a) == false) return;
    for(int i=0;i<length;i++) {
        this->i_found_address[i] = this->i_original_code[i];
    }
    VirtualProtect(this->i_found_address, length, a, &b);
}

unsigned char *ChimeraSignature::signature() {
    return &this->i_original_code[0];
}

size_t ChimeraSignature::size() {
    return this->i_original_code.size();
}

ChimeraSignature::ChimeraSignature(const char *name, const short *signature, size_t signature_length) :
    i_name(name) {
    auto *code = reinterpret_cast<unsigned char *>(FindCode(GetModuleHandle(0), signature, signature_length));
    if(code) {
        this->i_found_address = code;
        this->i_original_code.clear();
        for(uint32_t i=0;i<signature_length;i++) {
            this->i_original_code.push_back(code[i]);
        }
    }
    else {
        throw std::exception();
    }
}

bool write_code(unsigned char *where, const short *what, size_t length) {
    DWORD old_protect = 0;
    DWORD old_protect_b = 0;
    if(VirtualProtect(where, length, PAGE_READWRITE, &old_protect) == false) return false;
    for(int i=0;i<length;i++) {
        short x = what[i];
        if(x == -1) continue;
        *(where + i) = static_cast<unsigned char>(x);
    }
    VirtualProtect(where, length, old_protect, &old_protect_b);
    return true;
}

bool write_code(unsigned char *where, const unsigned char *what, size_t length) {
    auto *code = new short[length];
    for(size_t i=0;i<length;i++) code[i] = what[i];
    auto r = write_code(where, code, length);
    delete[] code;
    return r;
}
