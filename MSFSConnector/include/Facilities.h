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
#include "TempTypes.h"

namespace MSFSConnector
{
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
		std::vector<SIMCONNECT_DATA_FACILITY_AIRPORT> allAirportFacilities(HANDLE sim);
		Airport getAirportFacility(HANDLE sim, std::string name);
		std::vector<Runway> getRunwaysForAirport(HANDLE sim, std::string airport);
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

		static std::vector<SIMCONNECT_DATA_FACILITY_AIRPORT> airports;

		Atomics::AtomicVector<Data> result;
		DWORD requestID = 0;

		std::condition_variable condition;
		std::mutex mutex;
		bool done = false;
	};
}