#ifndef _COMMAND_FACTORY_H_
#define _COMMAND_FACTORY_H_

#include "../ResultLog.h"
#include "Command.h"

/** A factory object for building commands of a specified type
		Derive from this to implement factories for specific commands */
class CommandFactory
{
public:
	/** Construct
			@param _results Results log to give to the command objects we make */
	CommandFactory(ResultLog& _results);

public:
	/** Build a new command object */
	virtual Command* Build(const std::string& name);

protected:
	/** Override in derived classes to build commands of a particular type */
	virtual Command* NewCommandInstance() = 0;

private:
	CommandFactory(const CommandFactory& src);
	const CommandFactory& operator=(const CommandFactory& src);

private:
	ResultLog* results;
};

#endif
