#include "stdafx.h"

#include "lexer.h"

namespace {

namespace TextStyle {
	enum: int {
		default_,
		openTag,
		closeTag,
		fieldType,
		fieldName,
		fieldValue,
		fieldValueBool,
		fieldValueNumber
	};
}

enum class ValType {
	unknown,
	int32,
	int64,
	uint32,
	bin32,
	bin64,
	bool_,
	string
};

bool nameEquals(unsigned int start, LexAccessor& doc, const char* val) {
	for(; *val != '\0'; ++start, ++val)
		if(std::toupper(doc[start]) != *val) return false;
	return true;
}

ValType getType(unsigned int start, unsigned int end, LexAccessor& doc) {
	switch(end - start)  {
	case sizeof("BOOL") - 1:
		return nameEquals(start, doc, "BOOL")? ValType::bool_: ValType::unknown; 
	case sizeof("FLOAT") - 1:
		return nameEquals(start, doc, "FLOAT")? ValType::bin32: ValType::unknown;
	case sizeof("DOUBLE") - 1:
		return
			nameEquals(start, doc, "DOUBLE")? ValType::bin64:
			nameEquals(start, doc, "STRING")? ValType::string:
			ValType::unknown;
	case sizeof("INTEGER") - 1:
		return nameEquals(start, doc, "INTEGER")? ValType::int32: ValType::unknown;
	case sizeof("INTEGER64") - 1:
		return
			nameEquals(start, doc, "INTEGER64")? ValType::int64:
			nameEquals(start, doc, "TRANSLATE")? ValType::string:
			ValType::unknown;
	case sizeof("UNSIGNED INT") - 1:
		return nameEquals(start, doc, "UNSIGNED INT")? ValType::uint32: ValType::unknown;
	}

	return ValType::unknown;
}

bool isWhitespace(char c) { return c == ' ' || c == '\t'; }
bool isEol(char c) { return c == '\n' || c == '\r'; }
bool isNumeric(char c) { return c >= '0' && c <= '9'; }
bool isSign(char c) { return c == '-' || c == '+'; }
bool isExponent(char c) { return c == 'e' || c == 'E'; }

bool find(char c, unsigned int& pos, LexAccessor& doc) {
	for(; ; ++pos) {
		char cur = doc.SafeGetCharAt(pos, '\n');
		if(isEol(cur)) return false;
		if(cur == c) return true; 
	}
}

unsigned int findEol(unsigned int start, LexAccessor& doc) {
	for(;; ++start)
		if(isEol(doc.SafeGetCharAt(start, '\n'))) return start;
}

bool ReadInt(unsigned int& pos, unsigned int end, LexAccessor& doc) {
	if(pos == end) return false;
	if(!isNumeric(doc[pos])) return false;

	while(++pos != end && isNumeric(doc[pos])) {}
	return true;
}

void LexLine(unsigned int start, unsigned int end, int line, LexAccessor& doc) {
	char c;
	
	for(;;) {
		c = doc[start++];
		if(start == end) return;
		if(!isWhitespace(c)) break;
		doc.ColourTo(start - 1, TextStyle::default_);
	}
	
	if(c == '[') {
		bool isCloseTag = false;
		if(doc[start] == '/') isCloseTag = true;

		if(!find(']', start, doc)) goto MarkDefaultLine;

		if(isCloseTag) {
			doc.ColourTo(start, TextStyle::closeTag);
			doc.SetLineState(line, TextStyle::closeTag);
		} else {
			doc.ColourTo(start, TextStyle::openTag);
			doc.SetLineState(line, TextStyle::openTag);
		}

		if(++start != end) {
			do {
				if(!isWhitespace(doc[start])) {
					unsigned int indicEnd = end;
					for(; --indicEnd != start && isWhitespace(doc[indicEnd]);) {}

					doc.IndicatorFill(start, indicEnd + 1, 0, 1);
					break;
				}
			} while(++start != end);
			doc.ColourTo(end - 1, TextStyle::default_);
		}

		return;
	} else if(c == '<') {
		unsigned int old = start;
		if(!find('>', start, doc)) goto MarkDefaultLine;

		ValType type = getType(old, start, doc);
		doc.ColourTo(start++, TextStyle::fieldType);
		
		if(type == ValType::unknown)
			doc.IndicatorFill(old - 1, start, 0, 1); 

		if(start == end) return;
		if(!find(':', start, doc)) goto MarkDefaultLine;
		doc.ColourTo(start, TextStyle::fieldName);

		if(++start == end) return;
		if(type == ValType::string || type == ValType::unknown) {
			doc.ColourTo(end - 1, TextStyle::fieldValue);
			return;
		} else if(type == ValType::bool_)
			doc.ColourTo(end - 1, TextStyle::fieldValueBool);
		else
			doc.ColourTo(end - 1, TextStyle::fieldValueNumber);

		for(;;) {
			if(!isWhitespace(doc[start])) break;
			if(++start == end) return;
		}

		old = start;
		while(++start != end && !isWhitespace(doc[start])) {}

		if(start != end) {
			for(++start; end-- != start;)
				if(!isWhitespace(doc[end])) {
					doc.IndicatorFill(old, end + 1, 0, 1);
					return;
				}
		}
		start = old;

		if(type == ValType::bool_) {
			switch(end - start) {
			default: goto MarkInvalidValue;
			case 1:
				if(doc[start] != '1' && doc[start] != '0') goto MarkInvalidValue;
			return;
			case 4:
				if(!nameEquals(start, doc, "TRUE")) goto MarkInvalidValue;
			return;
			case 5:
				if(!nameEquals(start, doc, "FALSE")) goto MarkInvalidValue;
			return;
			}
		}

		if(isSign(doc[start])) {
			if(type == ValType::uint32) goto MarkInvalidValue;
			++start;
		}

		if(!ReadInt(start, end, doc)) goto MarkInvalidValue;
		if(start == end) return;
		if(type == ValType::int32 || type == ValType::int64 || type == ValType::uint32) goto MarkInvalidValue;

		if(doc[start] == '.') {
			if(!ReadInt(++start, end, doc)) goto MarkInvalidValue;
			if(start == end) return;
		}

		if(isExponent(doc[start])) {
			if(++start == end) goto MarkInvalidValue;
			if(isSign(doc[start])) ++start;
			if(!ReadInt(start, end, doc)) goto MarkInvalidValue;
		}

		if(start == end) return;

	MarkInvalidValue:
		doc.IndicatorFill(old, end, 0, 1);
		return;
	}
	
MarkDefaultLine:
	doc.ColourTo(end - 1, TextStyle::default_);
}

}

void SCI_METHOD TLDatLexer::Lex(unsigned int start, int length, int, IDocument* pDoc) {
	try {
		LexAccessor doc(pDoc);
		doc.StartAt(start);
		doc.StartSegment(start);

		pDoc->DecorationSetCurrentIndicator(0);

		for(unsigned int end = start + static_cast<unsigned int>(length);;) {
			unsigned int eol = findEol(start, doc);
			int line = doc.GetLine(start);
			doc.SetLineState(line, TextStyle::default_);

			if(start != eol) {
				pDoc->DecorationFillRange(start, 0, end - start);
				LexLine(start, eol, line, doc);
			}
			
			if(eol == end) break;
			if(doc[eol] == '\r' && eol + 1 != end && doc[eol + 1] == '\n')
				++eol;
			doc.ColourTo(eol, TextStyle::default_);
			if(++eol == end) break;
			start = eol;
		}
		
		doc.Flush();
	} catch (...) {
		pDoc->SetErrorStatus(SC_STATUS_FAILURE);
	}
}

void SCI_METHOD TLDatLexer::Fold(unsigned int start, int length, int, IDocument* doc) {
	unsigned int end = start + length;
	int line = doc->LineFromPosition(start);
	int level = SC_FOLDLEVELBASE;

	if(line > 0) {
		level = doc->GetLevel(line - 1);
		if(doc->GetLineState(line - 1) == TextStyle::openTag)
			level = (level & ~SC_FOLDLEVELHEADERFLAG) + 1;
	}

	for(; static_cast<unsigned int>(doc->LineStart(line)) < end; ++line) {
		int state = doc->GetLineState(line);
		if(state == TextStyle::closeTag && level > SC_FOLDLEVELBASE)
			doc->SetLevel(line, --level);
		else if(state == TextStyle::openTag) {
			doc->SetLevel(line, level | SC_FOLDLEVELHEADERFLAG);
			level++;
		} else
			doc->SetLevel(line, level);
	}
}