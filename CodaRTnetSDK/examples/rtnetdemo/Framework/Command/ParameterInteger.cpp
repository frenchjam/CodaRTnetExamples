#include "ParameterInteger.h"
#include <stdio.h>

ParameterInteger::ParameterInteger() :
	value(0L)
{
}

ParameterInteger::~ParameterInteger()
{
}

void ParameterInteger::Parse(const std::string& str)
{
	int numfields = sscanf(str.c_str(), " %d", &value);
	if (numfields == 1)
		Set();
}
