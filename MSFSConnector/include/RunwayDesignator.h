#pragma once

#include <string>

namespace MSFSConnector
{
	enum RunwayDesignator
	{
		NONE,
		LEFT,
		RIGHT,
		CENTER,
		WATER,
		A,
		B,
		LAST
	};

	std::string RunwayDesignatorToString(RunwayDesignator designator);
}