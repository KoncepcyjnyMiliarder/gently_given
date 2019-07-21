#include <gtest/gtest.h>
#include <hasher.hpp>

TEST(hasher, null_string)
{
  EXPECT_EQ(2166136261, hasher<char>::compile_time_hash(""));
  EXPECT_EQ(2166136261, hasher<wchar_t>::compile_time_hash(L""));
}

TEST(hasher, runtime_hash_matches_compiletime)
{
  EXPECT_EQ(hasher<char>::runtime_hash("ala ma kota"), hasher<char>::compile_time_hash("ala ma kota"));
  EXPECT_EQ(hasher<wchar_t>::runtime_hash(L"mirek i wielkie qq"), hasher<wchar_t>::compile_time_hash(L"mirek i wielkie qq"));
  EXPECT_EQ(hasher<char>::runtime_hash("mirek i wielkie qq"), hasher<char>::compile_time_hash("mirek i wielkie qq"));

  using mystr = const char[14];
  constexpr mystr examples[] =
  {
    "Cx'JVp5Xh`.yq",
    ",^`8fiy$-h*[F",
    "xNWE[^rGfRwp3",
    "~$]u=%yxO_}(K",
    "e6}h)E_DtOFc}",
    "iGRakjF*UShLy",
    "e+`%!5.&Y)(]D",
    "u>+6|pNZIj*;6",
    "=zvH@},f:/(Xa",
    "/vCrZIEt7@D]x",
    "zInSr'{w,Pm8X",
    "wnbm),XXJo=G3",
    "&llz2;Zf]a'/G",
    "Y-)d#m~kYsUFA",
    "VxC_OqYTxFKnt",
    "IMw@2rlBR/$:F",
    "x'0,es,3VR':H",
    "J'U<w`s(m&Eo/",
    "OymwWPe#2`<[S"
  };
  for(const auto& str : examples)
    EXPECT_EQ(hasher<char>::runtime_hash(str), hasher<char>::compile_time_hash(str));
}

TEST(hasher, uppercase_does_not_matter)
{
  EXPECT_EQ(hasher<char>::compile_time_hash("nAd rzeCzka opOdal krzAczKa"), hasher<char>::compile_time_hash("Nad rzEczkA oPodaL krZacZka"));
}

TEST(hasher, char_hash_matches_text_as_wchar_t)
{
  EXPECT_EQ(hasher<wchar_t>::compile_time_hash(L"Ala ma kota"), hasher<char>::compile_time_hash("ala ma kota"));
  EXPECT_EQ(hasher<wchar_t>::compile_time_hash(L"ALA MA KOTA"), hasher<char>::runtime_hash("ala ma kota"));
  EXPECT_EQ(hasher<wchar_t>::runtime_hash(L"ALA MA KOTA"), hasher<char>::compile_time_hash("ala ma kota"));
  EXPECT_EQ(hasher<wchar_t>::runtime_hash(L"AlA mA kOta"), hasher<char>::runtime_hash("aLa Ma KoTA"));
}
