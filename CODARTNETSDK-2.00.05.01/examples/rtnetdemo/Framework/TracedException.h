#ifndef _TEST_FAILURE_EXCEPTION_
#define _TEST_FAILURE_EXCEPTION_

#include <string>
#include <iostream>

/** 
		Summary
		An exception class which can log the source file and source code line at which the exception was thrown,
    plus module name and error message.
		
		Description
		This class is not to be thrown directly but rather via the STOP and STOP2 macros which use the C
		preprocessor to log the source file and source line at which they are used.
*/
class TracedException
{
public:
	/** Constructor to be called from STOP and STOP2 */
	TracedException(const std::string& _module, const char* _sourcefile, unsigned long _sourceline, const std::string& _message);

	/** Copy constructor 
	    @param src Instance to copy */
	TracedException(const TracedException& src);

	/** Print this exception to the given stream
	    @param report The stream to print to 
	 */
	void Report(std::ostream& report) const;

private:
	const TracedException& operator=(const TracedException& src);

private:
	std::string module;
	std::string sourcefile;
	unsigned long sourceline;
	std::string message;
};

/** Throw a TracedException tagged with this line of source code and source file name.
    @param _module Module name to put in the exception
		@param _message Error message to put in the exception
 */
#define STOP(_module, _message) throw TracedException(_module, __FILE__, __LINE__, _message)

/** Throw a TracedException tagged with this line of source code and source file name,
    and two message strings which are concatenated and separated by a colon.
    @param _module Module name to put in the exception
		@param _message1 Error message to put in the exception
		@param _value Value to append to error message following a colon
 */
#define STOP2(_module, _message1, _value) STOP(_module, std::string(_message1) + std::string(": ") + _value)

// Disable the following warning in MSVC:
// warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning ( disable : 4290 )

#endif
