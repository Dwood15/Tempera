#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include "hacclient/codefinder.h"

class ChimeraSignature {
private:
    std::string i_name;
    std::vector<unsigned char> i_original_code;
    unsigned char *i_found_address = nullptr;
public:
    const char *name();
    unsigned char *address();
    void undo();
    bool sig_search(const short *signature, size_t signature_length);
    unsigned char *signature();
    size_t size();

    ChimeraSignature(const char *name, const short *signature, size_t signature_length);
};

#define sig_scan_s(signature) FindCode(GetModuleHandle(0), signature, sizeof(signature) / sizeof(short))

bool write_code(unsigned char *where, const short *what, size_t length);
#define write_code_s(where,what) write_code(where,what,sizeof(what)/sizeof(short))

bool write_code(unsigned char *where, const unsigned char *what, size_t length);
#define write_code_c(where,what) write_code(where,what,sizeof(what))

template<class T>
bool write_code_any_array(unsigned char *where, T *what, size_t length) {
    return write_code(where, reinterpret_cast<const unsigned char *>(what), length);
}
#define write_code_any_array_c(where,what) write_code_any(where,what,sizeof(what))

template<class T>
bool write_code_any_value(unsigned char *where, T what) {
    return write_code(where, reinterpret_cast<const unsigned char *>(&what), sizeof(what));
}
