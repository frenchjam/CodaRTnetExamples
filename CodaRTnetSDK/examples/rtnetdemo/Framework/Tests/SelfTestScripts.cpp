#include "../Command/Command.h"
#include "../Command/ParameterString.h"
#include "../Command/ParameterInteger.h"
#include "../Command/CommandFactory.h"
#include "../Command/CommandList.h"
#include "../IO/CommandListReader.h"
#include "SelfTestMacros.h"
#include "SelfTestScripts.h"

class PlantTree : public Command
{
public:
	PlantTree()
	{
		RegisterParameter("variety", variety);
	}

	virtual void Run()
	{
		Comment1("Planted new", variety.Value());
	}

private:
	ParameterString variety;
};

class PlantTreeFactory : public CommandFactory
{
public:
	PlantTreeFactory(ResultLog& _results) : CommandFactory(_results)
	{	}
	virtual Command* NewCommandInstance()
	{ return new PlantTree; }
};

class WaterIt : public Command
{
public:
	WaterIt()
	{
		RegisterParameter("rainfall", rainfall);
	}

	virtual void Run() throw(TracedException)
	{
		// verify value
		if (rainfall.Value() < 0)
			COMMAND_STOP("Cannot have negative rainfall");

		// add to log
		Comment1("Measured rainfall", rainfall.Value());
	}

private:
	ParameterInteger rainfall;
};

class WaterItFactory : public CommandFactory
{
public:
	WaterItFactory(ResultLog& _results) : CommandFactory(_results)
	{	}
	virtual Command* NewCommandInstance()
	{ return new WaterIt; }
};

class BuildTreeHouse : public Command
{
public:
	virtual void Run()
	{
		Comment("Built a tree-house!");
	}
};

class BuildTreeHouseFactory : public CommandFactory
{
public:
	BuildTreeHouseFactory(ResultLog& _results) : CommandFactory(_results)
	{	}
	virtual Command* NewCommandInstance()
	{ return new BuildTreeHouse; }
};

void SelfTestScripts() throw(TracedException)
{
	// a new log of our test results
	std::ostrstream testdiag;
	ResultLog testresults(testdiag);

	// list of commands
	CommandList commands;
	commands.RegisterCommand("PlantTree", new PlantTreeFactory(testresults));
	commands.RegisterCommand("WaterIt", new WaterItFactory(testresults));
	commands.RegisterCommand("BuildTreeHouse", new BuildTreeHouseFactory(testresults));

	// script to run
	const char* testscript = 
		" { " \
		"  \"PlantTree\" : { \"variety\" : \"Oak Tree\" } , " \
		"  \"WaterIt\" : { \"rainfall\" : 1022 } , " \
		"  \"BuildTreeHouse\" : { }, " \
		"  \"WaterIt\" : { \"rainfall\" : 37 } " \
    " } ";
	std::istrstream testinput(testscript);

	// parse it
	CommandListReader(testinput).Read(commands);

	// run it
	commands.RunAll();

	// null-terminator
	testdiag << std::ends;

	// freeze results
	testdiag.freeze();

	// expected log output
	const char* expected_diag = 
		"[PlantTree] Planted new: Oak Tree\n" \
		"[WaterIt] Measured rainfall: 1022\n" \
		"[BuildTreeHouse] Built a tree-house!\n" \
		"[WaterIt] Measured rainfall: 37\n";

	// actual
	const char* actual_diag = testdiag.str();

	// check if got what was expected in log
	SELF_TEST_ASSERT (strcmp(expected_diag, actual_diag) == 0);
}
