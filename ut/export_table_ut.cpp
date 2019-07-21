#include <gtest/gtest.h>
#include <export_table.hpp>
#include <find_module.hpp>

TEST(export_table, basic)
{
  EXPECT_NE(nullptr, export_table(find_module(L"kernel32.dll")).get_raw_export_directory());
}

TEST(export_table, iterating_from_begin_to_end)
{
  export_table kernel32(find_module(L"kernel32.dll"));
  auto start = kernel32.begin();
  auto end = kernel32.end();
  EXPECT_NE(start, end);
  auto num_of_funcs = kernel32.get_raw_export_directory()->num_of_functions;
  EXPECT_NE(0, num_of_funcs);
  while (num_of_funcs--)
    ++start;
  EXPECT_EQ(start, end);
}

TEST(export_table, iterator_dereference)
{
  export_table kernel32(find_module(L"kernel32.dll"));
  auto start = kernel32.begin();
  auto end = kernel32.end();
  auto num_of_funcs = kernel32.get_raw_export_directory()->num_of_functions;
  while (num_of_funcs--)
  {
    auto current = *start;
    EXPECT_STRNE("", current.func_name);
    EXPECT_NE(0, current.func_va);
    ++start;
  }
}