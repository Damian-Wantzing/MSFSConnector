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
#include "Runway.h"

namespace MSFSConnector
{
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
		void addRunwayFacilityDefinition(HANDLE sim);
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

		Atomics::AtomicVector<std::shared_ptr<char[]>> result;
		DWORD requestID = 0;

		std::condition_variable condition;
		std::mutex mutex;
		bool done = false;
	};
}