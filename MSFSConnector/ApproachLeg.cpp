#include "ApproachLeg.h"

namespace MSFSConnector
{
	std::vector<std::string> simConnectApproachLegDefinitions()
	{
		return std::vector<std::string>
		{
			"TYPE",
			"FIX_ICAO",
			"FIX_REGION",
			"FIX_TYPE",
			"FIX_LATITUDE",
			"FIX_LONGITUDE",
			"FIX_ALTITUDE",
			"FLY_OVER",
			"DISTANCE_MINUTE",
			"TRUE_DEGREE",
			"TURN_DIRECTION",
			"ORIGIN_ICAO",
			"ORIGIN_REGION",
			"ORIGIN_TYPE",
			"ORIGIN_LATITUDE",
			"ORIGIN_LONGITUDE",
			"ORIGIN_ALTITUDE",
			"THETA",
			"RHO",
			"COURSE",
			"ROUTE_DISTANCE",
			"APPROACH_ALT_DESC",
			"ALTITUDE1",
			"ALTITUDE2",
			"SPEED_LIMIT",
			"VERTICAL_ANGLE",
			"ARC_CENTER_FIX_ICAO",
			"ARC_CENTER_FIX_REGION",
			"ARC_CENTER_FIX_TYPE",
			"ARC_CENTER_FIX_LATITUDE",
			"ARC_CENTER_FIX_LONGITUDE",
			"ARC_CENTER_FIX_ALTITUDE",
			"IS_IAF",
			"IS_IF",
			"IS_FAF",
			"IS_MAP"
		};
	}
}