#include "ResultLog.h"
#include "TracedException.h"

ResultLog::ResultLog(std::ostream& _diag) :
	diag(_diag)
{

}

void ResultLog::Log1(const std::string& module, const std::string& message)
{
	diag << "[" << module << "] " << message << std::endl;
}
void ResultLog::Log2(const std::string& module, const std::string& message1, const std::string& message2)
{
	std::string combinedmessage ( message1  );
	combinedmessage += std::string(": ");
	combinedmessage += message2;
	Log1(module, combinedmessage);
}

void ResultLog::LogException(const std::string& module, const TracedException& e)
{
	// record date and time of error
	Log1(module, "TEST FAILED");
	
	// dedicated line for error report
	e.Report(diag);
	diag << std::endl;
}
