#pragma once
#include <cstdint>

namespace MSFSConnector
{
	#pragma pack(push, 1)
	struct Vasi
	{
		int32_t type;
		float biasX;
		float biasZ;
		float spacing;
		float angle;
	};
	#pragma pack(pop)
}