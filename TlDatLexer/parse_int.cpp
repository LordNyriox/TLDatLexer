/*
 * TlDatLexer Plugin for Notepad++.
 * Copyright (C) 2013  Jason Newcomb
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.h"
#include "parse_int.h"

#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>

#undef max

namespace {

bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

template<typename T>
ParseError isValidSigned(char* begin, char* end) {
	typedef typename std::make_unsigned<T>::type U;
	assert(begin <= end);
	if(begin == end) return ParseError::badFormat;

	int negate = 0;
	if(*begin == '-') {
		negate = 1;
		++begin;
	} else if(*begin == '+')
		++begin;

	U val = 0;
	for(; begin != end; ++begin) {
		if(!isDigit(*begin)) return ParseError::badFormat;
		if(val > static_cast<U>(std::numeric_limits<T>::max()) / 10) return ParseError::outOfRange;
		val *= 10;
		if(val + *begin - '0' > static_cast<U>(std::numeric_limits<T>::max()) + negate) return ParseError::outOfRange;
		val += *begin - '0';
	}

	return ParseError::none;
}

template<typename T>
ParseError isValidUnsigned(char* begin, char* end) {
	assert(begin <= end);
	if(begin == end) return ParseError::badFormat;

	T val = 0;
	for(; begin != end; ++begin) {
		if(!isDigit(*begin)) return ParseError::badFormat;
		if(val > std::numeric_limits<T>::max() / 10) return ParseError::outOfRange;
		val *= 10;
		if(val + *begin - '0' < val) return ParseError::outOfRange;
		val += *begin - '0';
	}

	return ParseError::none;
}

}

ParseError isValidInt32(char* begin, char* end) {
	return isValidSigned<std::int32_t>(begin, end);
}

ParseError isValidInt64(char* begin, char* end) {
	return isValidSigned<std::int64_t>(begin, end);
}

ParseError isValidUInt32(char* begin, char* end) {
	return isValidUnsigned<std::uint32_t>(begin, end);
}
