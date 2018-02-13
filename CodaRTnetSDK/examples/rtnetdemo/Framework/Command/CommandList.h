#ifndef _COMMAND_LIST_
#define _COMMAND_LIST_

#include <string>
#include <map>
#include <vector>

#include "CommandFactory.h"

/** A list of command factories and the ability to manufacture commands by name and keep them in a local list */
class CommandList
{
public:
	
	/** Construct empty */
	CommandList();

	/** Destructor */
	virtual ~CommandList();

	/** Register a command factory by name 
			@param name Name to register
			@param factory Corresponding factory */
	void RegisterCommand(const std::string& name, CommandFactory* factory);

	/** Run all commands in local list (will have been made using BuildCommand) 
			@throws TracedException if a command fails */
	void RunAll() const throw(TracedException);

	/** Look up factory with specified command name, make new command and store in list 
			@param commandname Name to find */
	Command* BuildCommand(const std::string& commandname);

private:
	CommandList(const CommandList& src);
	const CommandList& operator=(const CommandList& src);

protected:
	std::map<std::string, CommandFactory*> commandbuilders;
	std::vector<Command*> commands;
};

#endif
