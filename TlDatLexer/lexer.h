#ifndef TLDATLEXER_LEXER_H
#define TLDATLEXER_LEXER_H

#include <ILexer.h>

typedef ILexer* (*LexerFactoryFunction)();

class TLDatLexer: public ILexer {
public:
	static ILexer* factory() { return new(std::nothrow) TLDatLexer; }

	int SCI_METHOD Version() const override { return 0; }
	void SCI_METHOD Release() override { delete this; }
	const char* SCI_METHOD PropertyNames() override { return ""; }
	int SCI_METHOD PropertyType(const char*) override { return 0; }
	const char* SCI_METHOD DescribeProperty(const char*) override { return ""; } 
	int SCI_METHOD PropertySet(const char*, const char*) override { return 0; }
	const char* SCI_METHOD DescribeWordListSets() override { return ""; }
	int SCI_METHOD WordListSet(int, const char*) override { return 0; }
	void* SCI_METHOD PrivateCall(int, void*) override { return 0; }

	void SCI_METHOD Lex(unsigned int, int, int, IDocument*) override;
	void SCI_METHOD Fold(unsigned int, int, int, IDocument*) override;
};

#endif