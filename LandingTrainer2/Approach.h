#pragma once
#include <cstdint>

namespace MSFSConnector
{
	struct Approach
	{
		int32_t type;
		int32_t suffix;
		int32_t runwayNumber;
		int32_t runwayDesignator;
		char FAFICAO[8];
		char FAFRegion[8];
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
	};
}