#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace MSFSConnector
{
	#pragma pack(push, 1)
	struct ApproachLeg
	{
		int32_t type;
		char FIXICAO[8];
		char FIXRegion[8];
		int32_t FIXType;
		double FIXLatitude;
		double FIXLongitude;
		double FIXAltitude; // Altitude of the FIX in meters.
		int32_t flyOver;
		int32_t distanceMinute;
		int32_t trueDegree;
		int32_t turnDirection;
		char originICAO[8];
		char originRegion[8];
		int32_t originType;
		double originLatitude;
		double originLongitude;
		double originAltitude; // Altitude of the origin in meters.
		float theta;
		float RHO;
		float course;
		float routeDistance;
		int32_t approachAltDesc;
		float altitude1;
		float altitude2;
		float speedLimit;
		float verticalAngle;
		char arcCenterFIXICAO[8];
		char arcCenterFIXRegion[8];
		int32_t arcCenterFIXType;
		double arcCenterFIXLatitude;
		double arcCenterFIXLongitude;
		double arcCenterFIXAltitude; // Altitude of the Arc Center Fix in meters.
		int32_t isIAF;
		int32_t isIF;
		int32_t isFAF;
		int32_t isMAP;
	};
	#pragma pack(pop)

	std::vector<std::string> simConnectApproachLegDefinitions();
}