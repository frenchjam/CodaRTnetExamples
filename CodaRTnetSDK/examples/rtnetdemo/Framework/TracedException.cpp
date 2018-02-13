#include "TracedException.h"

TracedException::TracedException(const std::string& _module, const char* _sourcefile, unsigned long _sourceline, const std::string& _message) :
	module(_module),
	sourcefile(_sourcefile),
	sourceline(_sourceline),
	message(_message)
{
}

TracedException::TracedException(const TracedException& src) :
	module(src.module),
	sourcefile(src.sourcefile),
	sourceline(src.sourceline),
	message(src.message)
{
}

void TracedException::Report(std::ostream& report) const
{
	report 
		<< sourcefile << "(" << sourceline  << "): "
		<< "FAIL [" << module << "]: "
		<< message;
}
