#pragma once

#include <Windows.h>
#include <cstdint>
#include <memory>

struct Data {
	DWORD type;
	std::shared_ptr<char[]> data;
};

#pragma pack(push, 1)
struct TempRunway
{
	double latitude;
	double longitude;
	double altitude;
	float heading;
	float length;
	float width;
	float patternAltitude;
	float slope;
	float trueSlope;
	int32_t surface;
	char primaryILSICAO[8];
	char primaryILSRegion[8];
	int32_t primaryILSType;
	int32_t primaryNumber;
	int32_t primaryDesignator;
	char secondaryILSICAO[8];
	char secondaryILSRegion[8];
	int32_t secondaryILSType;
	int32_t secondaryNumber;
	int32_t secondaryDesignator;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TempApproach
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
#pragma pack(pop)

#pragma pack(push, 1)
struct TempApproachTransition
{
	int32_t type;
	char IAFICAO[8];
	char IAFRegion[8];
	int32_t IAFType;
	float IAFAltitude;
	char DMEArcICAO[8];
	char DMEArcRegion[8];
	int32_t DMEArcType;
	int32_t DMEArcRadial;
	float DMEArcDistance;
	char name[8];
	int32_t approachLegsCount;
};
#pragma pack(pop)