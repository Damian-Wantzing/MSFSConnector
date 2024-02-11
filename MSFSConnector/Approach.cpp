#include "Approach.h"
#include "ApproachLeg.h"
#include <ApproachTransition.h>

namespace MSFSConnector
{
	std::vector<std::string> simConnectApproachDefinitions()
	{
		std::vector<std::string> approachDefinitionFirst
		{
			"OPEN AIRPORT",
			"OPEN APPROACH",
			"TYPE",
			"SUFFIX",
			"RUNWAY_NUMBER",
			"RUNWAY_DESIGNATOR",
			"FAF_ICAO",
			"FAF_REGION",
			"FAF_HEADING",
			"FAF_ALTITUDE",
			"FAF_TYPE",
			"MISSED_ALTITUDE",
			"HAS_LNAV",
			"HAS_LNAVVNAV",
			"HAS_LP",
			"HAS_LPV",
			"N_TRANSITIONS",
			"N_FINAL_APPROACH_LEGS",
			"N_MISSED_APPROACH_LEGS"
		};

		std::vector<std::string> approachTransitionDefinitions = simConnectApproachTransitionDefinitions();
		approachDefinitionFirst.insert(approachDefinitionFirst.end(), approachTransitionDefinitions.begin(), approachTransitionDefinitions.end());

		std::vector<std::string> approachDefinitionSecond
		{
			"OPEN FINAL_APPROACH_LEG"
		};
		approachDefinitionFirst.insert(approachDefinitionFirst.end(), approachDefinitionSecond.begin(), approachDefinitionSecond.end());

		std::vector<std::string> approachLegDefinitions = simConnectApproachLegDefinitions();
		approachDefinitionFirst.insert(approachDefinitionFirst.end(), approachLegDefinitions.begin(), approachLegDefinitions.end());

		std::vector<std::string> approachDefinitionThird
		{
			"CLOSE FINAL_APPROACH_LEG",
			"OPEN MISSED_APPROACH_LEG"
		};
		approachDefinitionFirst.insert(approachDefinitionFirst.end(), approachDefinitionThird.begin(), approachDefinitionThird.end());

		approachDefinitionFirst.insert(approachDefinitionFirst.end(), approachLegDefinitions.begin(), approachLegDefinitions.end());

		std::vector<std::string> approachDefinitionFourth
		{
			"CLOSE MISSED_APPROACH_LEG",
			"CLOSE APPROACH",
			"CLOSE AIRPORT"
		};
		approachDefinitionFirst.insert(approachDefinitionFirst.end(), approachDefinitionFourth.begin(), approachDefinitionFourth.end());

		return approachDefinitionFirst;
	}
}