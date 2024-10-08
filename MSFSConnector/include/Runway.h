#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <Windows.h>
#include <SimConnect.h>

#include "Pavement.h"
#include "ApproachLights.h"
#include "Vasi.h"

namespace MSFSConnector
{
	struct Runway
	{
		double latitude;
		double longitude;
		double altitude; // Altitude of the runway in meters.
		float heading;
		float length;
		float width;
		float patternAltitude; // Altitude of the pattern in meters.
		float slope;
		float trueSlope;
		int32_t surface;
		std::string primaryILSICAO;
		std::string primaryILSRegion;
		int32_t primaryILSType;
		int32_t primaryNumber;
		int32_t primaryDesignator;
		Pavement primaryThreshold;
		Pavement primaryBlastpad;
		Pavement primaryOverrun;
		ApproachLights primaryApproachLights;
		Vasi primaryLeftVasi;
		Vasi primaryRightVasi;
		std::string secondaryILSICAO;
		std::string secondaryILSRegion;
		int32_t secondaryILSType;
		int32_t secondaryNumber;
		int32_t secondaryDesignator;
		Pavement secondaryThreshold;
		Pavement secondaryBlastpad;
		Pavement secondaryOverrun;
		ApproachLights secondaryApproachLights;
		Vasi secondaryLeftVasi;
		Vasi secondaryRightVasi;
	};

	std::vector<std::string> simConnectRunwayDefinitions();
}