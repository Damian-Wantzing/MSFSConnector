#include "ApproachTransition.h"
#include <ApproachLeg.h>

namespace MSFSConnector
{
	std::vector<std::string> simConnectApproachTransitionDefinitions()
	{
		std::vector<std::string> approachTransitionDefinitionFirst
		{
			"OPEN APPROACH_TRANSITION",
			"TYPE",
			"IAF_ICAO",
			"IAF_REGION",
			"IAF_TYPE",
			"IAF_ALTITUDE",
			"DME_ARC_ICAO",
			"DME_ARC_REGION",
			"DME_ARC_TYPE",
			"DME_ARC_RADIAL",
			"DME_ARC_DISTANCE",
			"NAME",
			"N_APPROACH_LEGS",
			"OPEN APPROACH_LEG"
		};

		std::vector<std::string> approachLegDefinitions = simConnectApproachLegDefinitions();
		approachTransitionDefinitionFirst.insert(approachTransitionDefinitionFirst.end(), approachLegDefinitions.begin(), approachLegDefinitions.end());

		std::vector<std::string> approachTransitionDefinitionSecond
		{
			"CLOSE APPROACH_LEG",
			"CLOSE APPROACH_TRANSITION"
		};

		approachTransitionDefinitionFirst.insert(approachTransitionDefinitionFirst.end(), approachTransitionDefinitionSecond.begin(), approachTransitionDefinitionSecond.end());

		return approachTransitionDefinitionFirst;
	}
}