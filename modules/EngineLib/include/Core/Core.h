// Core/Core.h

#pragma once

#include <string>
#include <expected>

using int8 = char;
using uint8 = unsigned char;
using int16 = short;
using uint16 = unsigned short;
using int32 = int;
using uint32 = unsigned int;
using int64 = long long;
using uint64 = unsigned long long;

using float32 = float;
using float64 = double;

using FString = std::string;
using FStringView = std::string_view;

template<typename T, typename E>
using TResult = std::expected<T, E>;

template<typename E>
using TErr = std::unexpected<E>;
