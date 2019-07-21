#pragma once

#include <structs.hpp>

unsigned find_export(const image_dos_header* base, const wchar_t* export_name);
unsigned find_export(const image_dos_header* base, const char* export_name);
unsigned find_export(const image_dos_header* base, uint64_t export_hash);