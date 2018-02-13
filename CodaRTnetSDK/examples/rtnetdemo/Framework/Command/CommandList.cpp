#include "../TracedException.h"
#include "CommandList.h"
#include <ios>

const char* ParseModuleName = "CommandList.Parse";

CommandList::CommandList()
{
}

CommandList::~CommandList()
{
	// delete command objects
	for (std::vector<Command*>::iterator iter( commands.begin() ); iter != commands.end(); iter++)
	{
		delete (*iter);
	}
	commands.clear();

	// delete factory
	for (std::map<std::string, CommandFactory*>::iterator iter( commandbuilders.begin() ); iter != commandbuilders.end(); iter++)
	{
		delete iter->second;
	}
	commandbuilders.clear();
}

void CommandList::RegisterCommand(const std::string& name, CommandFactory* factory)
{
	commandbuilders[name] = factory;
}

void CommandList::RunAll() const throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray)
{
	for (std::vector<Command*>::const_iterator iter ( commands.begin() ); iter != commands.end(); iter++)
	{
		(*iter)->Run();
	}
}

Command* CommandList::BuildCommand(const std::string& commandname)
{
	// find the command name in our list
	std::map<std::string, CommandFactory*>::const_iterator command_iter = commandbuilders.find(commandname);
		
	// verify that name exists
	if (command_iter == commandbuilders.end())
	{
		// command not found
		return NULL;
	}
	else
	{
		// get factory object
		CommandFactory* factory = command_iter->second;

		// build new command
		Command* command = factory->Build(commandname);

		// add to list of commands
		commands.push_back(command);

		// return result
		return command;
	}		
}
