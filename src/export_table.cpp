#include <export_table.hpp>
#include <cassert>

export_table::export_table(const image_dos_header* base)
  : base_(base),
    export_directory_(nullptr)
{
  assert(base);
  auto raw_base = (unsigned)base_;
  const image_nt_headers* nt_header = (const image_nt_headers*)(base_->e_lfanew + raw_base);

  assert(nt_header->pe_signature == 0x00004550); //"PE"
  auto data_directory_rva = nt_header->optional_header.data_directory[0].rva;
  auto data_directory_size = nt_header->optional_header.data_directory[0].size;
  if (data_directory_rva != 0 && data_directory_size != 0)
    export_directory_ = (const image_export_directory*)(data_directory_rva + raw_base);
}

const image_export_directory* export_table::get_raw_export_directory() const
{
  return export_directory_;
}

unsigned export_table::find_va_by_ordinal(unsigned ordinal) const
{
  unsigned biased_index = ordinal - export_directory_->base;
  if(biased_index >= export_directory_->num_of_functions)
    return 0;

  auto raw_base = (unsigned)base_;
  const unsigned* func_rva_table = (const unsigned*)(export_directory_->rva_of_functions + raw_base);
  return raw_base + func_rva_table[biased_index];
}

export_table::export_by_name_iterator export_table::begin()
{
  return export_by_name_iterator((unsigned)base_, export_directory_, 0);
}

export_table::export_by_name_iterator export_table::end()
{
  return export_by_name_iterator((unsigned)base_, export_directory_, export_directory_->num_of_names);
}
