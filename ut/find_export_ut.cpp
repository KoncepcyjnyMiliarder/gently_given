#include <gtest/gtest.h>
#include <find_export.hpp>
#include <find_module.hpp>
#include <hasher.hpp>
#include <export_table.hpp>
#include <Windows.h> //LoadLibrary

TEST(find_export, nonexistent)
{
  EXPECT_EQ(0, find_export(find_module("kernel32.dll"), "dupa dupa"));
}

TEST(find_export, some_existent)
{
  EXPECT_NE(0, find_export(find_module(hasher<char>::compile_time_hash("kernel32.dll")), hasher<char>::compile_time_hash("WriteFileEx")));
}

TEST(find_export, ws2_32_ordinals)
{
  const image_dos_header* ws_base = (const image_dos_header*)LoadLibraryA("ws2_32.dll");
  ASSERT_EQ(ws_base, find_module("ws2_32.dll"));

  export_table ws_exports(ws_base);
  EXPECT_EQ(0, ws_exports.find_va_by_ordinal(44444));
  EXPECT_EQ(0, ws_exports.find_va_by_ordinal(0));
  EXPECT_EQ(find_export(ws_base, "accept"), ws_exports.find_va_by_ordinal(1));
  EXPECT_EQ(find_export(ws_base, "connect"), ws_exports.find_va_by_ordinal(4));
  EXPECT_EQ(find_export(ws_base, "send"), ws_exports.find_va_by_ordinal(19));
  EXPECT_EQ(find_export(ws_base, "WEP"), ws_exports.find_va_by_ordinal(500));
  EXPECT_EQ(find_export(ws_base, "__WSAFDIsSet"), ws_exports.find_va_by_ordinal(151));
  EXPECT_EQ(0, ws_exports.find_va_by_ordinal(ws_exports.get_raw_export_directory()->num_of_functions + 1));
  FreeLibrary((HMODULE)ws_base);
}