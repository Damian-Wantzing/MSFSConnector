#pragma once
#include <cstdint>

namespace MSFSConnector
{
	struct Vasi
	{
		int32_t type;
		float biasX;
		float biasZ;
		float spacing;
		float angle;
	};
}