#include <find_module.hpp>
#include <hasher.hpp>

ldr_data_entry* first_ldr_data_entry()
{
  ldr_data_entry* result;
  __asm
  {
    mov eax, fs:[0x30]
    mov eax, [eax + 0x0C]
    mov eax, [eax + 0x1C]
    mov[result], eax //todo naked inline?
  }
}

const image_dos_header* find_module(const wchar_t* name)
{
  return find_module(hasher<wchar_t>::runtime_hash(name));
}

const image_dos_header* find_module(const char* name)
{
  return find_module(hasher<char>::runtime_hash(name));
}

const image_dos_header* find_module(uint64_t hash_val)
{
  ldr_data_entry* cursor = first_ldr_data_entry();
  while (cursor->base_address)
  {
    auto current_module_hash = hasher<wchar_t>::runtime_hash(cursor->base_dll_name.buf);
    if (current_module_hash == hash_val)
      return (const image_dos_header*)cursor->base_address;
    cursor = (ldr_data_entry*)cursor->in_memory_order_module_list.flink;
  }
  return nullptr;
}

