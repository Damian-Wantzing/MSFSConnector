#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace MSFSConnector
{
	#pragma pack(push, 1)
	struct ApproachTransition
	{
		int32_t type;
		char IAFICAO[8];
		char IAFRegion[8];
		int32_t IAFType;
		float IAFAltitude;
		char DMEArcICAO[8];
		char DMEArcRegion[8];
		int32_t DMEArcType;
		int32_t DMEArcRadial;
		float DMEArcDistance;
		char name[8];
		int32_t approachLegsCount;
	};
	#pragma pack(pop)

	std::vector<std::string> simConnectApproachTransitionDefinitions();
}