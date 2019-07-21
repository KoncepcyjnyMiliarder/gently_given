#include <gtest/gtest.h>
#include <find_module.hpp>
#include <hasher.hpp>

TEST(find_module, find_nonexistent_returns_null)
{
  EXPECT_EQ(nullptr, find_module("ala ma kota"));
}

TEST(find_module, find_kernel32_dll)
{
  EXPECT_NE(nullptr, find_module(L"kernel32.dll"));
}

TEST(find_module, find_ntdll_dll)
{
  EXPECT_NE(nullptr, find_module("ntdll.dll"));
}

TEST(find_module, different_modules_cannot_share_base)
{
  EXPECT_NE(find_module("kernel32.dll"), find_module("ntdll.dll"));
}

TEST(find_module, uppercase_does_not_matter)
{
  EXPECT_EQ(find_module("NtDlL.DlL"), find_module("nTdLl.dll"));
  EXPECT_EQ(find_module("ntdll.dll"), find_module("NTDLL.DLL"));
}

TEST(find_module, charsize_does_not_matter)
{
  EXPECT_EQ(find_module(L"NtDlL.DlL"), find_module("nTdLl.dll"));
  EXPECT_EQ(find_module("ntdll.dll"), find_module(L"NTDLL.DLL"));
}

TEST(find_module, find_by_precomputed_hash)
{
  auto hash = hasher<wchar_t>::compile_time_hash(L"NtDlL.dLL");
  EXPECT_NE(nullptr, find_module(hash));
}