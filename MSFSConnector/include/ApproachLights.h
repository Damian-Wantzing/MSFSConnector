#pragma once

#include <cstdint>

namespace MSFSConnector
{
	#pragma pack(push, 1)
	struct ApproachLights
	{
		int32_t system;
		int32_t strobeCount;
		int32_t hasEndLights;
		int32_t hasReilLights;
		int32_t hasTouchdownLights;
		int32_t onGround;
		int32_t enabled;
		float offset;
		float spacing;
		float slope;
	};
	#pragma pack(pop)
}