#pragma once

#include <cstdint>

struct unicode_string
{
  unsigned short len;
  unsigned short max_len;
  wchar_t* buf;
};

struct list_entry
{
  list_entry* flink;
  list_entry* blink;
};

struct ldr_data_entry
{
  list_entry in_memory_order_module_list;
  void* base_address;
  void* entrypoint;
  unsigned long size_of_image;
  unicode_string full_dll_name;
  unicode_string base_dll_name;
  unsigned long flags;
  short load_count;
  short tls_index;
  list_entry hash_table_entry;
  unsigned long time_datestamp;
};

struct image_data_directory
{
  unsigned rva;
  unsigned size;
};

struct image_optional_header_32
{
  unsigned short magic;
  unsigned char major_linker_version;
  unsigned char minor_linker_version;
  unsigned size_of_code;
  unsigned size_of_initialized_data;
  unsigned size_of_uninitialized_data;
  unsigned address_of_entrypoint;
  unsigned base_of_code;
  unsigned base_of_data;
  unsigned imagebase;
  unsigned section_alignment;
  unsigned file_alignment;
  unsigned short major_os_version;
  unsigned short minor_os_version;
  unsigned short major_image_version;
  unsigned short minor_image_version;
  unsigned short major_subsystem_version;
  unsigned short minor_subsystem_version;
  unsigned win32_version_value;
  unsigned size_of_image;
  unsigned size_of_headers;
  unsigned checksum;
  unsigned short subsystem;
  unsigned short dll_characteristics;
  unsigned size_of_stack_reserve;
  unsigned size_of_stack_commit;
  unsigned size_of_heap_reserve;
  unsigned size_of_heap_commit;
  unsigned loader_flags;
  unsigned number_of_rva_and_sizes;
  image_data_directory data_directory[16];
};

struct image_dos_header
{
  unsigned short e_magic; // Magic number
  unsigned short e_cblp; // Bytes on last page of file
  unsigned short e_cp; // Pages in file
  unsigned short e_crlc; // Relocations
  unsigned short e_cparhdr; // Size of header in paragraphs
  unsigned short e_minalloc; // Minimum extra paragraphs needed
  unsigned short e_maxalloc; // Maximum extra paragraphs needed
  unsigned short e_ss; // Initial (relative) SS value
  unsigned short e_sp; // Initial SP value
  unsigned short e_csum; // Checksum
  unsigned short e_ip; // Initial IP value
  unsigned short e_cs; // Initial (relative) CS value
  unsigned short e_lfarlc; // File address of relocation table
  unsigned short e_ovno; // Overlay number
  unsigned short e_res[4]; // Reserved unsigned shorts
  unsigned short e_oemid; // OEM identifier (for e_oeminfo)
  unsigned short e_oeminfo; // OEM information; e_oemid specific
  unsigned short e_res2[10]; // Reserved unsigned shorts
  long e_lfanew; // File address of new exe header
};

struct image_file_header
{
  unsigned short machine;
  unsigned short number_of_sections;
  unsigned time_datestamp;
  unsigned pointer_to_symbol_table;
  unsigned number_of_symbols;
  unsigned short size_of_optional_header;
  unsigned short characteristics;
};

struct image_nt_headers
{
  unsigned pe_signature;
  image_file_header file_header;
  image_optional_header_32 optional_header;
};

struct image_export_directory
{
  unsigned characteristics;
  unsigned time_datestamp;
  unsigned short major_version;
  unsigned short minor_version;
  unsigned name;
  unsigned base;
  unsigned num_of_functions;
  unsigned num_of_names;
  unsigned rva_of_functions;
  unsigned rva_of_names;
  unsigned rva_of_name_ordinals;
};