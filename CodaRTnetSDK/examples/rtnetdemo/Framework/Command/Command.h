#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <string>
#include <map>
#include <strstream>
#include "Parameter.h"
#include "../ResultLog.h"
#include "../TracedException.h"

// Forward declarations
namespace codaRTNet
{
	class NetworkException;
	class DeviceStatusArray;
}

/** An abstract command which can be executed by our program.

		Summary
		The purpose of this class is to have an abstract way to describe commands
		such that a list of them can be read from a file and executed.
		Derive from this class to add scriptable commands to the program.
		Parameters can be added to the command by having member variables
		derived from Parameter which are registered using the RegisterParameter method.
	*/
class Command
{
	friend class CommandFactory;

protected:

	/** Protected empty constructor to be used by the CommandFactory */
	Command();

	/** The factory class will call this to set the log */
	void SetResultLog(const std::string& _name, ResultLog* _results);

public:

	/** Virtual destructor */
	virtual ~Command();

	/** Abstract command method to be implemented by derived classes */
	virtual void Run() throw(TracedException) = 0;

	/** Get the wrapper object for a parameter
			@param str name of parameter to find
			@return The parameter wrapper if found, NULL otherwise
		*/
	Parameter* GetParameter(const std::string& str);

	/** See if all required parameters are set
			@return true if all parameters filled, false otherwise */
	bool HaveRequiredParameters() const;

private:
	Command(const Command& src);
	const Command& operator=(const Command& src);

protected:

	/** Add message to log using this command's name as module name */
	void Comment(const std::string& message);

	/** Add message and number to log using this command's name as module name */
	void Comment1(const std::string& message, long value);

	/** Add message and string to log using this command's name as module name */
	void Comment1(const std::string& message, const std::string& value);

	/** Add contents of string stream to log using this command's name as module name */
	void Comment(std::ostrstream& formattedmessage);

	/** Register a parameter name used by this command - useful for looking up parameters in script file */
	void RegisterParameter(const std::string& parametername, Parameter& parameter);

	/** Name of this command */
	const std::string& Name() const
	{ return name; }

private:

	std::map<std::string, Parameter*> parameters;
	std::string name;
	ResultLog* results;
};

/** Call the STOP macro to throw a TracedException to log this source file and line plus this command's name
		@param _message Error message to log 
		@throws TracedException */
#define COMMAND_STOP(_message) STOP(Name(), _message)

/** Call the STOP2 macro to throw a TracedException to log this source file and line plus this command's name.
		Also like STOP2 this will concatenate message and value.
		@param _message Error message to log 
		@throws TracedException */
#define COMMAND_STOP2(_message, _value) STOP2(Name(), _message, _value)

#endif
