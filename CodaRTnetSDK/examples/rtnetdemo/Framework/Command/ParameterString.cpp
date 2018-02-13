#include "ParameterString.h"

ParameterString::ParameterString()
{
}

ParameterString::~ParameterString()
{
}

void ParameterString::Parse(const std::string& str)
{
	value = str;
	Set();
}
