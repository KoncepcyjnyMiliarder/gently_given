#include <find_export.hpp>
#include <hasher.hpp>
#include <cassert>
#include <export_table.hpp>

unsigned find_export(const image_dos_header* base, const wchar_t* export_name)
{
  return find_export(base, hasher<wchar_t>::runtime_hash(export_name));
}

unsigned find_export(const image_dos_header* base, const char* export_name)
{
  return find_export(base, hasher<char>::runtime_hash(export_name));
}

unsigned find_export(const image_dos_header* base, uint64_t export_hash)
{
  export_table exptbl(base);
  for (const auto e : exptbl)
  {
    if (hasher<char>::runtime_hash(e.func_name) == export_hash)
      return e.func_va;
  }
  return 0;
}
