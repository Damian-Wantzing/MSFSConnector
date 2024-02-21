#pragma once
#include <cstdint>
#include <vector>
#include <string>

#include "ApproachLeg.h"

namespace MSFSConnector
{
	struct ApproachTransition
	{
		int32_t type;
		std::string IAFICAO;
		std::string IAFRegion;
		int32_t IAFType;
		float IAFAltitude; // Altitude of the Initial Approach Fix in meters.
		std::string DMEArcICAO;
		std::string DMEArcRegion;
		int32_t DMEArcType;
		int32_t DMEArcRadial;
		float DMEArcDistance;
		std::string name;
		int32_t approachLegsCount;
		std::vector<ApproachLeg> approachLegs;
	};

	std::vector<std::string> simConnectApproachTransitionDefinitions();
}