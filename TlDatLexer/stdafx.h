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
#include <cctype>
#include <cstring>
#include <cwchar>
#include <new>

#include <Scintilla.h>
#include <SciLexer.h>
#include <ILexer.h>
#include <LexAccessor.h>
