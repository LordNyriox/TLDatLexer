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

#pragma once

#ifndef UNICODE
	#define UNICODE
#endif
#ifndef _UNICODE
	#define _UNICODE
#endif

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define NTDDI_VERSION 0x05010300
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <limits>
#include <new>
#include <type_traits>

#include <PluginInterface.h>

#include <Scintilla.h>
#include <SciLexer.h>
#include <ILexer.h>
#include <LexAccessor.h>
