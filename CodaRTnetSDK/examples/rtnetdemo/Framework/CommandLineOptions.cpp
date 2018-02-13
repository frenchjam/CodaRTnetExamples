#include "CommandLineOptions.h"

const char* CommandLineOptionsName = "CommandLineOptions";

CommandLineOptions::CommandLineOptions()
{
}

void CommandLineOptions::RegisterOption(const char* name, std::string& option, const char* defaultvalue)
{
	option = defaultvalue;
	optionregister[name] = &option;
}

void CommandLineOptions::Parse(int argc, char* argv[]) throw(TracedException)
{
	int arg_index = 1;
	while (arg_index < argc)
	{
		// get next argument
		const char* arg = argv[arg_index++];

		// options begin with --
		if (arg[0] == '-' && arg[1] == '-')
		{
			// get option string without the --
			const char* optionname = arg + 2;

			// look up
			std::map<std::string, std::string*>::iterator iter = optionregister.find(optionname);
			if (iter == optionregister.end())
			{
				STOP2(CommandLineOptionsName, "Unrecognised option", optionname);
			}

			if (arg_index < argc)
			{
				// assign option string to the command line item
				(*iter->second) = argv[arg_index++];
			}
			else
			{
				// reached end of command line and no argument supplied
				STOP2(CommandLineOptionsName, "No value given for option", optionname);
			}
		}
		else
		{
			STOP2(CommandLineOptionsName, "Unexpected argument", arg);
		}
	}
}
