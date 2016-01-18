#ifndef _RT_NET_DEMO_OPTIONS_H_
#define _RT_NET_DEMO_OPTIONS_H_

#include "Framework/CommandLineOptions.h"

class ResultLog;

/** Command line options class specific to our RTNetDemo program */
class RTNetDemoOptions : protected CommandLineOptions
{
public:
	RTNetDemoOptions();

	/** Call CommandLineOptions::Parse and log the resulting set of options 
			@param results Log to write options to
			@param argc The argc from main 
			@param argv The argv from main
			@throws TracedException if there was a problem parsing options in base class */
	void Parse(ResultLog& results, int argc, char* argv[]) throw(TracedException);

	/** Structured text file containing commands */
	const std::string& CommandFile() const
	{ return commandfile; }

	/** RT Net server ip address as string */
	const std::string& Server() const
	{ return server; }

	/** File to write data output */
	const std::string& DataFile() const
	{ return datafile; }

private:
	std::string commandfile;
	std::string server;
	std::string datafile;
};

#endif
