#include <iostream>
#include <fstream>
#include <ios>
#include <winsock.h>
#include <iomanip>

#include "codaRTNetProtocolCPP/RTNetClient.h"

#include "Framework/TracedException.h"
#include "Framework/Command/CommandList.h"
#include "Framework/IO/CommandListReader.h"
#include "Framework/IO/DataWriter.h"
#include "Framework/Tests/SelfTest.h"

#include "RTNetDemoOptions.h"
#include "StartSystem.h"
#include "Alignment.h"
#include "CodaMode.h"
#include "CodaPacketMode.h"
#include "AcqContinuous.h"
#include "AcqContinuousBuffered.h"
#include "AcqSingleShot.h"

CODANET_VERSION program_version = { 1, 0, 1, 0 };

int main(int argc, char* argv[])
{
	// name to log for this module
	const char* modulename = "RTNetDemo";

	// create log
	ResultLog results(std::cout);

	// result to return: non-zero indicates an error
	// will be set to zero if all commands succeed
	int result = 1;

	try
	{
		// log command line program name
		results.Log1(modulename, argv[0]);

		// perform self-test of parameter loading (will throw TracedException if fails)
		SelfTest(results);
	
		// parse and display command line arguments
		// will throw exception if fails to parse
		RTNetDemoOptions options;
		options.Parse(results, argc, argv);
	
		// connect data file output
		results.Log2(modulename, "Opening data file", options.DataFile());
		std::ofstream datafile(options.DataFile().c_str());
		if (!datafile.is_open())
		{
			// throw TracedException to quit
			STOP(modulename, "Could not create data file");
		}

		// instantiate client
		codaRTNet::RTNetClient client;

		// instantiate writer to output packet data to file
		DataWriter dataoutput(datafile);

		// register commands which operate on client
		CommandList commands;
		commands.RegisterCommand("StartSystem", new StartSystemFactory(results, client));
		commands.RegisterCommand("Alignment", new AlignmentFactory(results, client));
		commands.RegisterCommand("CodaMode", new CodaModeFactory(results, client));
		commands.RegisterCommand("CodaPacketMode", new CodaPacketModeFactory(results, client));
		commands.RegisterCommand("AcqContinuous", new AcqContinuousFactory(results, client, dataoutput));
		commands.RegisterCommand("AcqContinuousBuffered", new AcqContinuousBufferedFactory(results, client, dataoutput));
		commands.RegisterCommand("AcqSingleShot", new AcqSingleShotFactory(results, client, dataoutput));

		// load the parameter file
		results.Log2(modulename, "Opening command file", options.CommandFile());
		std::ifstream commandstream(options.CommandFile().c_str());
		CommandListReader(commandstream).Read(commands);

		// attempt client connection
		results.Log2(modulename, "Connecting server", options.Server());
		unsigned long ip = codanet_ntohl( inet_addr(options.Server().c_str()) );
		try
		{
			client.connect(ip, 10111);
		}
		catch (...)
		{
			STOP(modulename, "Could not connect to server");
		}

		// retrieve and log server version
		results.Log1(modulename, "Retrieving server version");
		codaRTNet::Version server_version;
		client.getServerVersion(server_version);
		
		// put server in log
		std::ostrstream server_description;
		server_description << "Server: " << 
			server_version.wMajor << "." <<
			std::setfill('0') << std::setw(2) << server_version.wMinor << 
			" Build " << server_version.wBuild <<
			" Revision " << server_version.wRevision << std::ends;
		results.Log1(modulename, server_description.str());

		// output file header
		dataoutput.BeginFile();

		// put server in file
		dataoutput.AddVersion("server", server_version);
		
		// put program version in file
		dataoutput.AddVersion("program", (codaRTNet::Version&)program_version);

		// run all commands - will throw exception on failure
		commands.RunAll();

		// output file footer
		dataoutput.EndFile();
	}
	catch (const TracedException& e)
	{
		// will receive exceptions here when thrown by the STOP macros
		results.LogException(modulename, e);
	}
	catch (...)
	{
		// this should not happen during normal running as we are explicitly catching known exception types
		results.Log1(modulename, "Unrecognised exception");
	}

	// return result (non-zero if error)
	return result;
}
