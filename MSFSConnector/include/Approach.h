#pragma once
#include <cstdint>
#include <vector>
#include <string>

#include "ApproachTransition.h"

namespace MSFSConnector
{
	struct Approach
	{
		int32_t type;
		int32_t suffix;
		int32_t runwayNumber;
		int32_t runwayDesignator;
		std::string FAFICAO;
		std::string FAFRegion;
		float FAFHeading;
		float FAFAltitude;
		int32_t FAFType;
		float missedAltitude;
		int32_t hasLNAV;
		int32_t hasLNAVVNAV;
		int32_t hasLP;
		int32_t hasLPV;
		int32_t transitionsCount;
		int32_t finalApproachLegsCount;
		int32_t missedApproachLegsCount;
		std::vector<ApproachTransition> approachTransitions;
		std::vector<ApproachLeg> finalApproachLegs;
		std::vector<ApproachLeg> missedApproachLegs;
	};

	std::vector<std::string> simConnectApproachDefinitions();
}