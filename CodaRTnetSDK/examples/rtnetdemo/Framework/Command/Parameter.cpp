#include "Parameter.h"

Parameter::Parameter() : isset(false)
{
}

Parameter::~Parameter()
{
}

bool Parameter::IsSet() const
{ 
	return isset;
}

void Parameter::Set()
{
	isset = true;
}
