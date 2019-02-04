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

#include <Windows.h>

#include <Scintilla.h>

const wchar_t* name = L"TlDatLexer";

HWND nppHandle;
HWND scintillaHandle1;
HWND scintillaHandle2;

HWND getCurrentScintilla();
bool isTlDatLexerActive();
sptr_t getDirectScintillaPtr(void* handle);
SciFnDirect getDirectScintillaFunc(void* handle);
