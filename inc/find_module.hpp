#pragma once

#include <structs.hpp>

const image_dos_header* find_module(const wchar_t* name);
const image_dos_header* find_module(const char* name);
const image_dos_header* find_module(uint64_t hash_val);