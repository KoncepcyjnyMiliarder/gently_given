#pragma once

#include <structs.hpp>

class export_table
{
    const image_dos_header* const base_;
    const image_export_directory* export_directory_;

  public:

    struct export_by_name_iterator
    {

      struct export_entry
      {
        const char* func_name;
        unsigned func_va;
      };

      unsigned raw_base_addr_;
      const image_export_directory* export_table_;
      unsigned current_export_index_;

      //construct from index and header
      export_by_name_iterator(unsigned raw_base_addr, const image_export_directory* export_table, unsigned current_export_index)
        : raw_base_addr_(raw_base_addr), export_table_(export_table), current_export_index_(current_export_index) {}
      export_by_name_iterator(const export_by_name_iterator&) = default;

      //prefix
      export_by_name_iterator& operator++()
      {
        ++current_export_index_;
        return *this;
      }

      //postfix
      export_by_name_iterator operator++(int)
      {
        return export_by_name_iterator(raw_base_addr_, export_table_, ++current_export_index_);
      }

      //dereference WOOOHOOOO
      export_entry operator*() const
      {
        export_entry res;
        const unsigned* func_rva_table = (const unsigned*)(export_table_->rva_of_functions + raw_base_addr_);
        const unsigned* name_rva_table = (const unsigned*)(export_table_->rva_of_names + raw_base_addr_);
        const unsigned short* ordinal_table = (const unsigned short*)(export_table_->rva_of_name_ordinals + raw_base_addr_);
        res.func_va = raw_base_addr_ + func_rva_table[ordinal_table[current_export_index_]];
        res.func_name = (const char*)(raw_base_addr_ + name_rva_table[current_export_index_]);
        return res;
      }

      //equality
      bool operator==(const export_by_name_iterator& rhs) const
      {
        return raw_base_addr_ == rhs.raw_base_addr_ && export_table_ == rhs.export_table_ && current_export_index_ == rhs.current_export_index_;
      }

      bool operator!=(const export_by_name_iterator& rhs) const
      {
        return !(*this == rhs);
      }
    };

    export_table(const image_dos_header* base);
    const image_export_directory* get_raw_export_directory() const;
    unsigned find_va_by_ordinal(unsigned ordinal) const;

    export_by_name_iterator begin();
    export_by_name_iterator end();
};