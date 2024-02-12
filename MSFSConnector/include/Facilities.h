#pragma once

#include <string>
#include <stdexcept>
#include <future>
#include <vector>

#include <Windows.h>
#include "SimConnect.h"

#include "IDCounter.h"
#include "Dispatcher.h"
#include "AtomicVector.h"
#include "Airport.h"
#include "Approach.h"
#include "ApproachTransition.h"
#include "ApproachLeg.h"
#include "Runway.h"

namespace MSFSConnector
{
	struct Data{
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

	class Facilities
	{
	public:
		void callbackHandler(SIMCONNECT_RECV* data);
		static std::vector<SIMCONNECT_DATA_FACILITY_AIRPORT> allAirports(HANDLE sim);
		static Airport getAirport(HANDLE sim, std::string name);
		static bool isAirport(HANDLE sim, std::string name);
		static std::vector<Runway> getRunways(HANDLE sim, std::string airport);
		static std::vector<Approach> getApproaches(HANDLE sim, std::string airport);

	private:
		static std::vector<SIMCONNECT_DATA_FACILITY_AIRPORT> airports;

		std::vector<SIMCONNECT_DATA_FACILITY_AIRPORT> allAirportFacilities(HANDLE sim);
		Airport getAirportFacility(HANDLE sim, std::string name);

		std::vector<Runway> getRunwaysForAirport(HANDLE sim, std::string airport);
		Runway setRunwayFromTemp(TempRunway tempRunway);

		std::vector<Approach> getApproachesForAirport(HANDLE sim, std::string airport);

		void setDone()
		{
			std::lock_guard<std::mutex> lock(mutex);
			done = true;
			condition.notify_one();
		}

		void waitForDone()
		{
			std::unique_lock<std::mutex> lock(mutex);
			if (!condition.wait_for(lock, std::chrono::milliseconds(5000), [this] { return done; })) throw std::runtime_error("Facilities request timed out");
		}

		Atomics::AtomicVector<Data> result;
		DWORD requestID = 0;

		std::condition_variable condition;
		std::mutex mutex;
		bool done = false;
	};
}