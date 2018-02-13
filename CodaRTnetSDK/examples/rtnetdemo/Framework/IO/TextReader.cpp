#include "TextReader.h"
#include <stdlib.h>

const char* TextReaderModuleName = "TextReader";

TextReader::TextReader(std::istream& _stream) :
	stream(_stream)
{
}

void TextReader::SkipSpace()
{
	while (isspace(stream.peek()))
		stream.get();
}

int TextReader::NextNonSpace()
{
	SkipSpace();
	return stream.get();
}

void TextReader::ParseQuotedString(std::string& str) throw(TracedException)
{
	// should be open-quotes
	if (NextNonSpace() != '\"')
	{
		STOP(TextReaderModuleName, "Expected quotes");
	}

	// scan until next quotes
	int c(0);
	while ((c = stream.get()) != '\"')
		str += c;
}

void TextReader::ParseValue(std::string& value) throw(TracedException)
{
	SkipSpace();

	// value may be quoted or not
	if (stream.peek() == '\"')
	{
		ParseQuotedString(value);
	}
	else
	{
		int c = 0;
		while (!isspace((c = stream.peek())) && (c != ',') && (c != '}') && (c != ']'))
			value += stream.get();
	}
}

void TextReader::ParseKeyValue(std::string& key, std::string& value) throw(TracedException)
{
	ParseQuotedString(key);

	// must have colon to separate parameter value
	if (NextNonSpace() != ':')
	{
		STOP(TextReaderModuleName, "Expected colon to separate parameter value");
	}

	// parse value irrespective of quotes or not
	ParseValue(value);
}
