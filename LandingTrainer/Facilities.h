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
		static Airport getAirport(HANDLE sim, std::string name);
		static std::vector<Runway> getRunways(HANDLE sim, std::string airport);
		static std::vector<Approach> getApproaches(HANDLE sim, std::string airport);

	private:
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
			condition.wait(lock, [this] { return done; });
		}

		Atomics::AtomicVector<std::shared_ptr<char[]>> result;
		DWORD requestID = 0;

		std::condition_variable condition;
		std::mutex mutex;
		bool done = false;
	};
}