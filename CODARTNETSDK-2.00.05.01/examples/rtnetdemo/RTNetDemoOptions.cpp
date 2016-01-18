#include "Framework/ResultLog.h"
#include "RTNetDemoOptions.h"

RTNetDemoOptions::RTNetDemoOptions()
{
	RegisterOption("command-file", commandfile, "commands.txt");
	RegisterOption("server", server, "127.0.0.1");
	RegisterOption("data-file", datafile, "data.txt");
}

void RTNetDemoOptions::Parse(ResultLog& results, int argc, char* argv[]) throw(TracedException)
{
	// parse
	CommandLineOptions::Parse(argc, argv);

	// display all options and values
	results.Log1("RTNetDemoOptions", "Options: ");
	for (std::map<std::string, std::string*>::const_iterator iter(OptionRegister().begin()); iter != OptionRegister().end(); iter++)
	{
		std::string message("--");
		message += iter->first;
		message += ": ";
		message += *iter->second;
		results.Log1("RTNetDemoOptions", message);
	}
}