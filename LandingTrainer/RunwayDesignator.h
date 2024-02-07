#pragma once

#include <string>

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

std::string RunwayDesignatorToString(RunwayDesignator designator)
{
	switch (designator)
	{
	case NONE: return "";
	case LEFT: return "L";
	case RIGHT: return "R";
	case CENTER: return "C";
	case WATER: return "Water";
	case A: return "A";
	case B: return "B";
	case LAST: return "Last";
	}
}