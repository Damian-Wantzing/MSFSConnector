#include "Airport.h"

namespace MSFSConnector
{
	std::vector<std::string> simConnectAirportDefinitions()
	{
		return std::vector<std::string> {
			"OPEN AIRPORT",
			"LATITUDE",
			"LONGITUDE",
			"ALTITUDE",
			"MAGVAR",
			"NAME",
			"NAME64",
			"ICAO",
			"REGION",
			"TOWER_LATITUDE",
			"TOWER_LONGITUDE",
			"TOWER_ALTITUDE",
			"N_RUNWAYS",
			"N_STARTS",
			"N_FREQUENCIES",
			"N_HELIPADS",
			"N_APPROACHES",
			"N_DEPARTURES",
			"N_ARRIVALS",
			"N_TAXI_POINTS",
			"N_TAXI_PARKINGS",
			"N_TAXI_PATHS",
			"N_TAXI_NAMES",
			"N_JETWAYS",
			"CLOSE AIRPORT"
		};
	}
}