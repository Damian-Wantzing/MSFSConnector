#pragma once

#include <string>

namespace MSFSConnector
{
	struct SimVar
	{
		std::string name;
		std::string unitType;
		SIMCONNECT_DATATYPE dataType = SIMCONNECT_DATATYPE_FLOAT64;
	};
}