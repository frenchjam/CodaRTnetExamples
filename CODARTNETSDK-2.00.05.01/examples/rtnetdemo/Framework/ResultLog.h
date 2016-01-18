#ifndef _RESULT_LOG_H_
#define _RESULT_LOG_H_

#include <ostream>
class TracedException;

/** Produce a formatted log */
class ResultLog
{
public:

	/** Construct log to specified stream
			@param _diag Stream to log to
		*/
	ResultLog(std::ostream& _diag);

	/** Log a message
	    @param module Module name to use in prefix to message
			@param message Message to write
		*/
	void Log1(const std::string& module, const std::string& message);

	/** Concatenate two messages and log them
	    @param module Module name to use in prefix to message
			@param message1 First message to write, will be followed by colon and space
			@param message2 Second message to write
		*/
	void Log2(const std::string& module, const std::string& message1, const std::string& message2);

	/** Log the given exception which includes source file name and line of code
	    @param module Module to use in prefix (should be the logging module, not necessarily the module which threw the exception)
			@param e Exception to log
		*/
	void LogException(const std::string& module, const TracedException& e);

protected:
	std::ostream& diag;
};


#endif

