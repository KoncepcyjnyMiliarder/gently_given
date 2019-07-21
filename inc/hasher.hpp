#pragma once

#include <cstdint>
#include <utility>

template<typename CharT>
struct hasher
{
    static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "hasher works with char and wchar_t only");

  private:

    static constexpr CharT to_lower(CharT c)
    {
      return (c >= 'A' && c <= 'Z') ? (c + 'a' - 'A') : c;
    }

    template<uint64_t Size, uint64_t I = 1>
    struct internal_hash_calc
    {
      static constexpr uint64_t do_the_trick(const CharT(&str)[Size])
      {
        return (internal_hash_calc < Size, I + 1 >::do_the_trick(str) ^ to_lower(str[Size - I - 1])) * 16777619u;
      }
    };

    template<uint64_t Size>
    struct internal_hash_calc<Size, Size>
    {
      static constexpr uint64_t do_the_trick(const CharT(&str)[Size])
      {
        return 2166136261u;
      }
    };

  public:

    template<uint64_t Size>
    static constexpr uint64_t compile_time_hash(const CharT(&str)[Size])
    {
      return internal_hash_calc<Size>::do_the_trick(str);
    }

    static uint64_t runtime_hash(const CharT* str)
    {
      uint64_t hash = 2166136261u;
      while (*str)
      {
        hash ^= to_lower(*str++);
        hash *= 16777619u;
      }
      return hash;
    }
};