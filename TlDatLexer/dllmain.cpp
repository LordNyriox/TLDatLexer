#include "stdafx.h"
#include <PluginInterface.h>

#include "lexer.h"

BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID) { return TRUE; }

namespace {

HWND notepad = nullptr;

void __cdecl about() {
	MessageBox(
		notepad,
		L"Author: Jason Newcomb.\n"
		L"Site: http://bitbucket.org/Jarcho/tldatlexer\n\n"
		L"This plugin is free software; it comes with ABSOLUTELY NO WARRANTY.\n",
		L"TLDatLexer v0.3.0",
		MB_OK
	);
}

/*
bool isTlDatLexer() {
	int id;
	SendMessage(notepad, NPPM_GETCURRENTLANGTYPE, 0, reinterpret_cast<LPARAM>(&id));
	std::size_t nameLen = SendMessage(notepad, NPPM_GETLANGUAGENAME, id, reinterpret_cast<LPARAM>(nullptr));
	if(nameLen != 5) return false;

	wchar_t name[sizeof("TLDat")];
	SendMessage(notepad, NPPM_GETLANGUAGENAME, id, reinterpret_cast<LPARAM>(name));
	return wcscmp(name, L"TLDat") == 0;
}

sptr_t getScintillaWindow(void* handle) {
	return static_cast<sptr_t>(SendMessage(static_cast<HWND>(handle), SCI_GETDIRECTPOINTER, 0, 0));
}

SciFnDirect getScintillaFunction(void* handle) {
	return reinterpret_cast<SciFnDirect>(SendMessage(static_cast<HWND>(handle), SCI_GETDIRECTFUNCTION, 0, 0));
}
*/

}

///// NPP Exports /////

FuncItem* getFuncsArray(int* count) {
	static FuncItem funcs[] = { { L"About", &about, 0, false, nullptr } };
	
	*count = sizeof(funcs) / sizeof(FuncItem);
	return funcs;
}

BOOL isUnicode() { return TRUE; }
const wchar_t* getName() { return L"TLDatLexer"; }
void setInfo(NppData data) { notepad = data._nppHandle; }
LRESULT messageProc(UINT, WPARAM, LPARAM) { return TRUE; }
void beNotified(SCNotification*) {}

///// Scintilla Exports /////

int SCI_METHOD GetLexerCount() { return 1; }

void SCI_METHOD GetLexerName(unsigned int index, char* name, int len) {
	if(index == 0) {
		std::strncpy(name, "TLDat", len);
		name[len - 1] = '\0';
	} else
		std::memset(name, '\0', len);
}

void SCI_METHOD GetLexerStatusText(unsigned int index, wchar_t* desc, int len) {
	if(index == 0) {
		std::wcsncpy(desc, L"Torchlight data file", len);
		desc[len - 1] = L'\0';
	}else
		std::wmemset(desc, L'\0', len);
}

LexerFactoryFunction SCI_METHOD GetLexerFactory(unsigned int index) {
	return index == 0? TLDatLexer::factory: nullptr;
}