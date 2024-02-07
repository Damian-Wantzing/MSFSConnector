#include "Facilities.h"

namespace MSFSConnector
{
	void Facilities::callbackHandler(SIMCONNECT_RECV* data)
	{
		if (data->dwID != SIMCONNECT_RECV_ID_FACILITY_DATA && data->dwID != SIMCONNECT_RECV_ID_FACILITY_DATA_END) return;

		switch (data->dwID)
		{
		case SIMCONNECT_RECV_ID_FACILITY_DATA:
		{
			SIMCONNECT_RECV_FACILITY_DATA* facilityData = (SIMCONNECT_RECV_FACILITY_DATA*)data;
			if (facilityData->UserRequestId != requestID) return;

			std::shared_ptr<char[]> dataCopy(new char[facilityData->dwSize]);
			memcpy(dataCopy.get(), &facilityData->Data, facilityData->dwSize);

			result.push_back(std::move(dataCopy));
		}
		break;
		case SIMCONNECT_RECV_ID_FACILITY_DATA_END:
		{
			setDone();
		}
		break;
		}
	}

	Airport Facilities::getAirport(HANDLE sim, std::string name)
	{
		Facilities facilities;
		return facilities.getAirportFacility(sim, name);
	}

	Airport Facilities::getAirportFacility(HANDLE sim, std::string name)
	{
		Dispatcher::CallbackID callbackID = Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });

		requestID = IDCounter::getID();

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN AIRPORT");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LATITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LONGITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ALTITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "MAGVAR");
		SimConnect_AddToFacilityDefinition(sim, requestID, "NAME");
		SimConnect_AddToFacilityDefinition(sim, requestID, "NAME64");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ICAO");
		SimConnect_AddToFacilityDefinition(sim, requestID, "REGION");
		SimConnect_AddToFacilityDefinition(sim, requestID, "TOWER_LATITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "TOWER_LONGITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "TOWER_ALTITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_RUNWAYS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_STARTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_FREQUENCIES");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_HELIPADS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_APPROACHES");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_DEPARTURES");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_ARRIVALS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_TAXI_POINTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_TAXI_PARKINGS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_TAXI_PATHS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_TAXI_NAMES");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_JETWAYS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE AIRPORT");

		HRESULT hr = SimConnect_RequestFacilityData(sim, requestID, requestID, name.c_str());
		if (hr == SIMCONNECT_EXCEPTION_ERROR) throw std::runtime_error("airport not found: " + name);

		waitForDone();

		Airport airport = *reinterpret_cast<Airport*>(result[0].get());

		Dispatcher::getInstance(sim).deregisterCallback(callbackID);

		return airport;
	}

	std::vector<Runway> Facilities::getRunways(HANDLE sim, std::string airport)
	{
		Facilities facilities;
		return facilities.getRunwaysForAirport(sim, airport);
	}

	std::vector<Runway> Facilities::getRunwaysForAirport(HANDLE sim, std::string airport)
	{
		Dispatcher::CallbackID callbackID = Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });

		requestID = IDCounter::getID();

		addRunwayFacilityDefinition(sim);

		HRESULT hr = SimConnect_RequestFacilityData(sim, requestID, requestID, airport.c_str());
		if (hr == SIMCONNECT_EXCEPTION_ERROR) throw std::runtime_error("airport not found: " + airport);

		waitForDone();

		std::vector<Runway> runways;

		for (int i = 1; i < result.size(); i += 13)
		{
			TempRunway tempRunway = *reinterpret_cast<TempRunway*>(result[i].get());
			Pavement primaryThreshold = *reinterpret_cast<Pavement*>(result[i + 1].get());
			Pavement secondaryThreshold = *reinterpret_cast<Pavement*>(result[i + 2].get());
			Pavement primaryBlastpad = *reinterpret_cast<Pavement*>(result[i + 3].get());
			Pavement secondaryBlastpad = *reinterpret_cast<Pavement*>(result[i + 4].get());
			Pavement primaryOverrun = *reinterpret_cast<Pavement*>(result[i + 5].get());
			Pavement secondaryOverrun = *reinterpret_cast<Pavement*>(result[i + 6].get());
			ApproachLights primaryApproachLights = *reinterpret_cast<ApproachLights*>(result[i + 7].get());
			ApproachLights secondaryApproachLights = *reinterpret_cast<ApproachLights*>(result[i + 8].get());
			Vasi primaryLeftVasi = *reinterpret_cast<Vasi*>(result[i + 9].get());
			Vasi secondaryLeftVasi = *reinterpret_cast<Vasi*>(result[i + 10].get());
			Vasi primaryRightVasi = *reinterpret_cast<Vasi*>(result[i + 11].get());
			Vasi secondaryRightVasi = *reinterpret_cast<Vasi*>(result[i + 12].get());

			Runway runway = setRunwayFromTemp(tempRunway);

			runway.primaryThreshold = primaryThreshold;
			runway.secondaryThreshold = secondaryThreshold;
			runway.primaryBlastpad = primaryBlastpad;
			runway.secondaryBlastpad = secondaryBlastpad;
			runway.primaryOverrun = primaryOverrun;
			runway.secondaryOverrun = secondaryOverrun;
			runway.primaryApproachLights = primaryApproachLights;
			runway.secondaryApproachLights = secondaryApproachLights;
			runway.primaryLeftVasi = primaryLeftVasi;
			runway.secondaryLeftVasi = secondaryLeftVasi;
			runway.primaryRightVasi = primaryRightVasi;
			runway.secondaryRightVasi = secondaryRightVasi;

			runways.push_back(runway);
		}

		Dispatcher::getInstance(sim).deregisterCallback(callbackID);

		return runways;
	}

	void Facilities::addRunwayFacilityDefinition(HANDLE sim)
	{
		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN AIRPORT");
		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN RUNWAY");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LATITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LONGITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ALTITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HEADING");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LENGTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "WIDTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "PATTERN_ALTITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SLOPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "TRUE_SLOPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SURFACE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "PRIMARY_ILS_ICAO");
		SimConnect_AddToFacilityDefinition(sim, requestID, "PRIMARY_ILS_REGION");
		SimConnect_AddToFacilityDefinition(sim, requestID, "PRIMARY_ILS_TYPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "PRIMARY_NUMBER");
		SimConnect_AddToFacilityDefinition(sim, requestID, "PRIMARY_DESIGNATOR");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SECONDARY_ILS_ICAO");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SECONDARY_ILS_REGION");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SECONDARY_ILS_TYPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SECONDARY_NUMBER");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SECONDARY_DESIGNATOR");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN PRIMARY_THRESHOLD");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LENGTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "WIDTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ENABLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE PRIMARY_THRESHOLD");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN SECONDARY_THRESHOLD");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LENGTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "WIDTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ENABLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE SECONDARY_THRESHOLD");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN PRIMARY_BLASTPAD");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LENGTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "WIDTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ENABLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE PRIMARY_BLASTPAD");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN SECONDARY_BLASTPAD");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LENGTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "WIDTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ENABLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE SECONDARY_BLASTPAD");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN PRIMARY_OVERRUN");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LENGTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "WIDTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ENABLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE PRIMARY_OVERRUN");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN SECONDARY_OVERRUN");
		SimConnect_AddToFacilityDefinition(sim, requestID, "LENGTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "WIDTH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ENABLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE SECONDARY_OVERRUN");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN PRIMARY_APPROACH_LIGHTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SYSTEM");
		SimConnect_AddToFacilityDefinition(sim, requestID, "STROBE_COUNT");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_END_LIGHTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_REIL_LIGHTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_TOUCHDOWN_LIGHTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ON_GROUND");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ENABLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "OFFSET");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SPACING");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SLOPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE PRIMARY_APPROACH_LIGHTS");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN SECONDARY_APPROACH_LIGHTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SYSTEM");
		SimConnect_AddToFacilityDefinition(sim, requestID, "STROBE_COUNT");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_END_LIGHTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_REIL_LIGHTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_TOUCHDOWN_LIGHTS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ON_GROUND");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ENABLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "OFFSET");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SPACING");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SLOPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE SECONDARY_APPROACH_LIGHTS");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN PRIMARY_LEFT_VASI");
		SimConnect_AddToFacilityDefinition(sim, requestID, "TYPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "BIAS_X");
		SimConnect_AddToFacilityDefinition(sim, requestID, "BIAS_Z");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SPACING");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ANGLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE PRIMARY_LEFT_VASI");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN SECONDARY_LEFT_VASI");
		SimConnect_AddToFacilityDefinition(sim, requestID, "TYPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "BIAS_X");
		SimConnect_AddToFacilityDefinition(sim, requestID, "BIAS_Z");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SPACING");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ANGLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE SECONDARY_LEFT_VASI");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN PRIMARY_RIGHT_VASI");
		SimConnect_AddToFacilityDefinition(sim, requestID, "TYPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "BIAS_X");
		SimConnect_AddToFacilityDefinition(sim, requestID, "BIAS_Z");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SPACING");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ANGLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE PRIMARY_RIGHT_VASI");

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN SECONDARY_RIGHT_VASI");
		SimConnect_AddToFacilityDefinition(sim, requestID, "TYPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "BIAS_X");
		SimConnect_AddToFacilityDefinition(sim, requestID, "BIAS_Z");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SPACING");
		SimConnect_AddToFacilityDefinition(sim, requestID, "ANGLE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE SECONDARY_RIGHT_VASI");

		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE RUNWAY");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE AIRPORT");
	}

	Runway Facilities::setRunwayFromTemp(TempRunway tempRunway)
	{
		Runway runway;

		runway.latitude = tempRunway.latitude;
		runway.longitude = tempRunway.longitude;
		runway.altitude = tempRunway.altitude;
		runway.heading = tempRunway.heading;
		runway.length = tempRunway.length;
		runway.width = tempRunway.width;
		runway.patternAltitude = tempRunway.patternAltitude;
		runway.slope = tempRunway.slope;
		runway.trueSlope = tempRunway.trueSlope;
		runway.surface = tempRunway.surface;
		runway.primaryILSICAO = tempRunway.primaryILSICAO;
		runway.primaryILSRegion = tempRunway.primaryILSRegion;
		runway.primaryILSType = tempRunway.primaryILSType;
		runway.primaryNumber = tempRunway.primaryNumber;
		runway.primaryDesignator = tempRunway.primaryDesignator;
		runway.secondaryILSICAO = tempRunway.secondaryILSICAO;
		runway.secondaryILSRegion = tempRunway.secondaryILSRegion;
		runway.secondaryILSType = tempRunway.secondaryILSType;
		runway.secondaryNumber = tempRunway.secondaryNumber;
		runway.secondaryDesignator = tempRunway.secondaryDesignator;

		return runway;
	}

	std::vector<Approach> Facilities::getApproaches(HANDLE sim, std::string airport)
	{
		Facilities facilities;
		return facilities.getApproachesForAirport(sim, airport);
	}

	std::vector<Approach> Facilities::getApproachesForAirport(HANDLE sim, std::string airport)
	{
		Dispatcher::CallbackID callbackID = Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });

		requestID = IDCounter::getID();

		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN AIRPORT");
		SimConnect_AddToFacilityDefinition(sim, requestID, "OPEN APPROACH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "TYPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "SUFFIX");
		SimConnect_AddToFacilityDefinition(sim, requestID, "RUNWAY_NUMBER");
		SimConnect_AddToFacilityDefinition(sim, requestID, "RUNWAY_DESIGNATOR");
		SimConnect_AddToFacilityDefinition(sim, requestID, "FAF_ICAO");
		SimConnect_AddToFacilityDefinition(sim, requestID, "FAF_REGION");
		SimConnect_AddToFacilityDefinition(sim, requestID, "FAF_HEADING");
		SimConnect_AddToFacilityDefinition(sim, requestID, "FAF_ALTITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "FAF_TYPE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "MISSED_ALTITUDE");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_LNAV");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_LNAVVNAV");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_LP");
		SimConnect_AddToFacilityDefinition(sim, requestID, "HAS_LPV");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_TRANSITIONS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_FINAL_APPROACH_LEGS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "N_MISSED_APPROACH_LEGS");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE APPROACH");
		SimConnect_AddToFacilityDefinition(sim, requestID, "CLOSE AIRPORT");

		HRESULT hr = SimConnect_RequestFacilityData(sim, requestID, requestID, airport.c_str());
		if (hr == SIMCONNECT_EXCEPTION_ERROR) throw std::runtime_error("airport not found: " + airport);

		waitForDone();

		std::vector<Approach> approaches;

		for (int i = 1; i < result.size(); i++)
		{
			Approach approach = *reinterpret_cast<Approach*>(result[i].get());
			approaches.push_back(approach);
		}

		Dispatcher::getInstance(sim).deregisterCallback(callbackID);

		return approaches;
	}
}