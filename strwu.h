// Copyright (c) [2025] [Jovan J. E. Odassius]
//
// License: MIT (See the LICENSE file in the root directory)
// Github: https://github.com/untyper/string-width-utility

// TODO: Unix support

#ifndef STRWU_STRING_WIDTH_UTILITY_H
#define STRWU_STRING_WIDTH_UTILITY_H

// C++ includes
#include <string>
#include <cstring>
#include <cwchar>

// Windows includes
#ifdef _WIN32
#include <Windows.h>
#else
// Unix includes
// ...
#endif

namespace strwu
{
#ifdef _WIN32
  // Copy char array to wchar_t array
  template <size_t N>
  inline void copy_char_to_wchar(const char(&source)[N], wchar_t(&dest)[N])
  {
    MultiByteToWideChar(CP_UTF8, 0, source, -1, dest, N);
  }

  // Copy wchar_t array to char array
  template <size_t N>
  inline void copy_wchar_to_char(const wchar_t(&source)[N], char(&dest)[N])
  {
    WideCharToMultiByte(CP_UTF8, 0, source, -1, dest, N, NULL, NULL);
  }

  template <size_t N>
  inline void copy_string_to_char(const std::string& source, char (&dest)[N])
  {
    strncpy_s(dest, N, source.c_str(), source.size());
  }

  template <size_t N>
  inline void copy_wstring_to_wchar(const std::wstring& source, wchar_t (&dest)[N])
  {
    wcsncpy_s(dest, N, source.c_str(), source.size());
  }

  // Function to convert std::wstring to std::string
  inline std::string wstring_to_string(const std::wstring& wide_string)
  {
    // Determine the size of the resulting UTF-8 string
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wide_string.c_str(), -1, NULL, 0, NULL, NULL);

    // Create a string to hold the converted result
    std::string utf8_string(size_needed - 1, 0); // size_needed includes null terminator, so exclude it

    // Perform the conversion
    WideCharToMultiByte(CP_UTF8, 0, wide_string.c_str(), -1, &utf8_string[0], size_needed, NULL, NULL);

    return utf8_string;
  }

  // Function to convert std::string to std::wstring
  inline std::wstring string_to_wstring(const std::string& utf8_string)
  {
    // Determine the size of the resulting wide string
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, NULL, 0);

    // Create a wide string to hold the converted result
    std::wstring wide_string(size_needed - 1, 0); // size_needed includes null terminator, so exclude it

    // Perform the conversion
    MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, &wide_string[0], size_needed);

    return wide_string;
  }

  #else // Unix

  // Copy char array to wchar_t array
  template <size_t N>
  inline void copy_char_to_wchar(const char(&source)[N], wchar_t(&dest)[N])
  {
  }

  // Copy wchar_t array to char array
  template <size_t N>
  inline void copy_wchar_to_char(const wchar_t(&source)[N], char(&dest)[N])
  {
  }

  template <size_t N>
  inline void copy_string_to_char(const std::string& source, char(&dest)[N])
  {
  }

  template <size_t N>
  inline void copy_wstring_to_wchar(const std::wstring& source, wchar_t(&dest)[N])
  {
  }

  // Function to convert std::wstring to std::string
  inline std::string wstring_to_string(const std::wstring& wide_string)
  {
    return std::string("");
  }

  // Function to convert std::string to std::wstring
  inline std::wstring string_to_wstring(const std::string& utf8_string)
  {
    return std::wstring(L"");
  }

  #endif

  // Helper type for function that accepts strings to flip input string type from
  // std::string to std::wstring (and vice versa),through the parameter directly.
  template <typename T>
  class string_not_t
  {
  private:
    // Opposite type of T (std::string if T is std::wstring, and vice versa)
    using opposite_type = std::conditional_t<std::is_same_v<T, std::string>, std::wstring, std::string>;

    // Stores the opposite type
    opposite_type data;

    // Convert function to handle std::string <-> std::wstring conversions
    opposite_type convert(const T& input)
    {
      if constexpr (std::is_same_v<T, std::string>)
      {
        return string_to_wstring(input);
      }
      else
      {
        return wstring_to_string(input);
      }
    }

  public:
    // Returns the stored string in opposite type
    auto str() const -> decltype(data)
    {
      return this->data;
    }

    // Constructor accepts either std::string or std::wstring, stores opposite type
    string_not_t(const T& input)
      : data(convert(input)) {
    }
  };

} // namespace strwu

#endif // STRWU_STRING_WIDTH_UTILITY_H
