#ifndef _COMMAND_LIST_READER_H

#include "../Command/CommandList.h"
#include "TextReader.h"

/** Read list of commands from structured text file */
class CommandListReader : protected TextReader
{
public:
	/** Construct to read from specified stream
			@param _stream Stream to read */
	CommandListReader(std::istream& _stream);

	/** Parse according to registered command factories and build command instances
			@param commandlist Command list to populate 
			@throws TracedException if structured text format was not as expected */
	void Read(CommandList& commandlist) throw(TracedException);

};

#endif