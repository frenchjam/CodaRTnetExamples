#ifndef _COMMAND_LINE_OPTIONS_H_
#define _COMMAND_LINE_OPTIONS_H_

#include <string>
#include <map>
#include "TracedException.h"

/** Register command line options to look for and parse the command line accordingly.
*/
class CommandLineOptions
{
public:

	/** Constructor (constructs with no options) */
	CommandLineOptions();

	/** Register a possible option with the default value to use if not found 
			@param name Name to find on command line (the name on the command line will be prefixed with --)
			@param option A string object to fill with the value found for this option
			@param defaultvalue The default value to put in option if no value found
	*/
	void RegisterOption(const char* name, std::string& option, const char* defaultvalue);

	/** Parse a command line
	    @param argc The argc from main
			@param argv The argv from main
		  @throws TracedException
		*/
	void Parse(int argc, char* argv[]) throw(TracedException);

	/** Get the array of all options registered 
			@return Array of options */
	const std::map<std::string, std::string*>& OptionRegister() const
	{ return optionregister; }

private:
	std::map<std::string, std::string*> optionregister;
};

#endif
