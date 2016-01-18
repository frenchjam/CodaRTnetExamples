#include "../TracedException.h"
#include "Command.h"

Command::Command() :
	results(NULL)
{
}

Command::~Command()
{
}

void Command::RegisterParameter(const std::string& parametername, Parameter& parameter)
{
	parameters[parametername] = &parameter;
}

void Command::SetResultLog(const std::string& _name, ResultLog* _results)
{
	name = _name;
	results = _results;
}

void Command::Comment(const std::string& message)
{
	results->Log1(name, message);
}

void Command::Comment(std::ostrstream& formattedmessage)
{
	formattedmessage << std::ends;
	results->Log1(name, formattedmessage.str());
}

void Command::Comment1(const std::string& message, long value)
{
	std::ostrstream formattedmessage;
	formattedmessage << message << ": " << value;
	Comment(formattedmessage);
}

void Command::Comment1(const std::string& message, const std::string& value)
{
	std::ostrstream formattedmessage;
	formattedmessage << message << ": " << value;
	Comment(formattedmessage);
}

Parameter* Command::GetParameter(const std::string& str)
{
	std::map<std::string, Parameter*>::const_iterator iter = parameters.find(str);
	return (iter != parameters.end()) ? iter->second : NULL;
}

bool Command::HaveRequiredParameters() const
{
	for (std::map<std::string, Parameter*>::const_iterator iter( parameters.begin() ); iter != parameters.end(); iter++)
	{
		if (!iter->second->IsSet())
			return false;
	}
	return true;
}

