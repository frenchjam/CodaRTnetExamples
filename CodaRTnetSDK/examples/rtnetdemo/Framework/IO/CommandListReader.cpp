#include "CommandListReader.h"

const char* CommandListReaderModuleName = "CommandListReader";
#define READERSTOP(_message) STOP(CommandListReaderModuleName, _message);
#define READERSTOP2(_message, _value) STOP2(CommandListReaderModuleName, _message, _value);

CommandListReader::CommandListReader(std::istream& _stream) :
	TextReader(_stream)
{
}

void CommandListReader::Read(CommandList& commandlist) throw(TracedException)
{
	// should begin with a bracket
	if (NextNonSpace() != '{')
	{
		READERSTOP("Mising opening bracket");
	}

	// loop to read all commands
	while (true)
	{
		// scan command name
		std::string commandname;
		ParseQuotedString(commandname);

		// expect colon separator for parameter value
		if (NextNonSpace() != ':')
		{
			READERSTOP("Expected colon followed by parameter values");
		}

		// expect bracket to open parameter values
		if (NextNonSpace() != '{')
		{
			READERSTOP("Expected open bracket for parameter values");
		}

		// create new object so we have access to its parameter list
		Command* command = commandlist.BuildCommand(commandname);
		if (command == NULL)
		{
			// command name not found
			READERSTOP2("Unrecognised command", commandname);
		}
		
		// populate it
		for (int parametercount = 0; ; parametercount++)
		{
			// a bracket ends params
			SkipSpace();
			if (stream.peek() == '}')
			{
				stream.get();
				break;
			}

			// expect comma-separator between commands
			if (parametercount > 0)
			{
				if (NextNonSpace() != ',')
				{
					READERSTOP("Expected } for end-of-parameters or comma to separate multiple parameters");
				}
			}

			// get parameter name
			std::string parametername;
			std::string valuestring;
			ParseKeyValue(parametername, valuestring);

			// check that this parameter exists
			Parameter* parameter = command->GetParameter(parametername);
			if (parameter == NULL)
			{
				READERSTOP2("Unrecognised parameter", parametername);
			}

			// get param value
			parameter->Parse(valuestring);
			if (!parameter->IsSet())
			{
				READERSTOP2("Failed to parse parameter value", valuestring);
			}
		}

		// verify that all mandafory parameters are set
		if (!command->HaveRequiredParameters())
		{
			READERSTOP2("Missing required parameters for command", commandname);
		}

		// look for command separator or end of sequence
		int term = NextNonSpace();
		if (term == '}')
		{
			break;
		}
		else if (term != ',')
		{
			READERSTOP2("Unexpected character following command", commandname);
		}
	}
}
