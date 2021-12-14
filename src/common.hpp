#ifndef COMMON_HPP_2021_ADA
#define COMMON_HPP_2021_ADA

#include <stddef.h>
#include <stdint.h>
#include <type_traits>

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

using I8 = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;

using USize = size_t;
using ISize = intptr_t;  // Not guaranteed I don't think but functionally true
using IPtr = intptr_t;
using UPtr = uintptr_t;

using F32 = float;
using F64 = double;

using IChar = signed char;
using UChar = unsigned char;
using UShort = unsigned short;
using UInt = unsigned;
using ULong = long;
using LL = long long;
using ULL = unsigned long long;
using UByte = UChar;
using IByte = IChar;
using Byte = UByte;

template<class T>
concept Integral = std::is_integral<T>::value;

#endif
