#pragma once

#include <vector>
#include <string>

namespace MSFSConnector
{
	#pragma pack(push, 1)
	struct Airport
	{
		double latitude;
		double longitude;
		double altitude;
		float magvar;
		char name[32];
		char name64[64];
		char icao[8];
		char region[8];
		double towerLatitude;
		double towerLongitude;
		double towerAltitude; // Altitude of the tower in meters.
		int32_t runwayCount;
		int32_t startCount;
		int32_t frequencyCount;
		int32_t helipadCount;
		int32_t approachesCount;
		int32_t depaturesCount;
		int32_t arrivalsCount;
		int32_t taxiPointsCount;
		int32_t taxiParkingsCount;
		int32_t taxiPathsCount;
		int32_t taxiNamesCount;
		int32_t jetwaysCount;
	};
	#pragma pack(pop)

	std::vector<std::string> simConnectAirportDefinitions();
}