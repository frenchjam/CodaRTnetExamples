#include "CommandFactory.h"

CommandFactory::CommandFactory(ResultLog& _results) :
	results(&_results)
{
}

Command* CommandFactory::Build(const std::string& name)
{
	Command* command = NewCommandInstance();
	command->SetResultLog(name, results);
	return command;
}
