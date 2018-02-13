#ifndef _TEXT_READER_H_
#define _TEXT_READER_H_

#include <istream>
#include <string>

#include "../TracedException.h"

/** Summary
		Read structured text files

		Description
		This is designed to read parts of the structured text files which are output from DataWriter.
		They use a subset of the JSON format ( http://www.json.org/ ).
	*/
class TextReader
{
public:
	/** Construct to read from given stream
			@param _stream Stream to read
		*/
	TextReader(std::istream& _stream);

	/** Skip space */
	void SkipSpace();

	/** Skip to next non-space and read it
			@return The value of next non-space character
		*/
	int NextNonSpace();

	/** Read from quote character until and including the next quote character and
	    extract the string in between.
			@param str Filled with the string between the quotes
			@throws TracedException if the stream is not positioned just before a quote
	*/
	void ParseQuotedString(std::string& str) throw(TracedException);

	/** Summary
			Parse a value.

			Description
			If the next nons-space character is a quote, this will read up to and including the next
			quote and return the string in between (like ParseQuotedString).  But if
			the next nons-space character is not a quote then it will read up until any
			space or control character is found, and so can read numbers which are not
			generally enclosed in quotes

			@param str Filled with the value read
			@throws TracedException		
		*/
	void ParseValue(std::string& value) throw(TracedException);

	/** Summary
			Parse a key and value separated by colon and arbitary numbers of space characters

			Description
			Parse a key-value pair. Supports quoted and non-quoted values.
			For example:
				
				"mykey" : "myvalue"

			Or:

				"some number" : 3


			@param key Filled with key read
			@param value Filled with value
			@throws TracedException if stream is not positioned before such a pair
			*/
	void ParseKeyValue(std::string& key, std::string& value) throw(TracedException);

protected:
	std::istream& stream;
};

#endif
