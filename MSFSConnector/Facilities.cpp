#include "Facilities.h"

#include "IDCounter.h"
#include "SimConnectExceptions.h"

namespace MSFSConnector
{
	std::vector<SIMCONNECT_DATA_FACILITY_AIRPORT> Facilities::airports;

	void Facilities::callbackHandler(SIMCONNECT_RECV* data)
	{
		if (data->dwID != SIMCONNECT_RECV_ID_FACILITY_DATA && data->dwID != SIMCONNECT_RECV_ID_FACILITY_DATA_END && data->dwID != SIMCONNECT_RECV_ID_AIRPORT_LIST) return;

		switch (data->dwID)
		{
		case SIMCONNECT_RECV_ID_FACILITY_DATA:
		{
			SIMCONNECT_RECV_FACILITY_DATA* facilityData = (SIMCONNECT_RECV_FACILITY_DATA*)data;
			if (facilityData->UserRequestId != requestID) return;
			
			Data data;
			data.type = facilityData->Type;
			data.data = std::make_shared<char[]>(facilityData->dwSize);
			memcpy(data.data.get(), &facilityData->Data, facilityData->dwSize);
			result.push_back(data);
		}
		break;
		case SIMCONNECT_RECV_ID_FACILITY_DATA_END:
		{
			SIMCONNECT_RECV_FACILITY_DATA_END* dataEnd = (SIMCONNECT_RECV_FACILITY_DATA_END*)data;
			if (dataEnd->RequestId != requestID) return;
			setDone();
		}
		break;
		case SIMCONNECT_RECV_ID_AIRPORT_LIST:
		{
			SIMCONNECT_RECV_AIRPORT_LIST* airportList = (SIMCONNECT_RECV_AIRPORT_LIST*)data;
			if (airportList->dwRequestID != requestID) return;

			for (size_t i = 0; i < airportList->dwArraySize; i++)
			{
				Data data;
				data.type = airportList->dwID;
				data.data = std::make_shared<char[]>(sizeof(SIMCONNECT_DATA_FACILITY_AIRPORT));
				memcpy(data.data.get(), &airportList->rgData[i], sizeof(SIMCONNECT_DATA_FACILITY_AIRPORT));
				result.push_back(data);
			}

			if ((airportList->dwEntryNumber + 1) >= airportList->dwOutOf) setDone();
		}
		}
	}

	std::vector<SIMCONNECT_DATA_FACILITY_AIRPORT> Facilities::allAirports(HANDLE sim)
	{
		if (airports.size() > 0) return airports;
		Facilities facilities;
		return facilities.allAirportFacilities(sim);
	}

	std::vector<SIMCONNECT_DATA_FACILITY_AIRPORT> Facilities::allAirportFacilities(HANDLE sim)
	{
		dispatcher = Dispatcher::getInstance(sim);
		Dispatcher::CallbackID callbackID = dispatcher->registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });

		requestID = IDCounter::getID();

		HRESULT hr;
		hr = SimConnect_RequestFacilitiesList(sim, SIMCONNECT_FACILITY_LIST_TYPE_AIRPORT, requestID);
		if (hr == E_FAIL) throw SimConnectFailureException("there was an error requesting the facilities list");

		waitForDone();

		for (int i = 0; i < result.size(); i++)
		{
			SIMCONNECT_DATA_FACILITY_AIRPORT airport = *reinterpret_cast<SIMCONNECT_DATA_FACILITY_AIRPORT*>(result[i].data.get());
			airports.push_back(airport);
		}

		return airports;
	}

	Airport Facilities::getAirport(HANDLE sim, std::string name)
	{
		if (!isAirport(sim, name)) throw std::runtime_error("Airport does not exist");

		Facilities facilities;
		return facilities.getAirportFacility(sim, name);
	}

	bool Facilities::isAirport(HANDLE sim, std::string name)
	{
		std::vector<SIMCONNECT_DATA_FACILITY_AIRPORT> airports = allAirports(sim);

		bool airportExists = false;
		for (int i = 0; i < airports.size(); i++)
		{
			if (strcmp(airports[i].Ident, name.c_str()) == 0) airportExists = true;
		}

		return airportExists;
	}

	Airport Facilities::getAirportFacility(HANDLE sim, std::string name)
	{
		dispatcher = Dispatcher::getInstance(sim);
		Dispatcher::CallbackID callbackID = dispatcher->registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });

		requestID = IDCounter::getID();

		HRESULT hr;

		for (const std::string& definition : simConnectAirportDefinitions())
		{
			hr = SimConnect_AddToFacilityDefinition(sim, requestID, definition.c_str());
			if (hr == E_FAIL) throw SimConnectFailureException("there was an error adding to the facility defintion");
		}

		hr = SimConnect_RequestFacilityData(sim, requestID, requestID, name.c_str());
		if (hr == SIMCONNECT_EXCEPTION_ERROR) throw std::runtime_error("airport not found: " + name);

		waitForDone();

		Airport airport = *reinterpret_cast<Airport*>(result[0].data.get());

		dispatcher->deregisterCallback(callbackID);

		return airport;
	}

	std::vector<Runway> Facilities::getRunways(HANDLE sim, std::string airport)
	{
		Facilities facilities;
		return facilities.getRunwaysForAirport(sim, airport);
	}

	std::vector<Runway> Facilities::getRunwaysForAirport(HANDLE sim, std::string airport)
	{
		dispatcher = Dispatcher::getInstance(sim);
		Dispatcher::CallbackID callbackID = dispatcher->registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });

		requestID = IDCounter::getID();

		HRESULT hr;

		for (const std::string& definition : simConnectRunwayDefinitions())
		{
			hr = SimConnect_AddToFacilityDefinition(sim, requestID, definition.c_str());
			if (hr == E_FAIL) throw std::runtime_error("there was an error adding to the facility defintion");
		}

		hr = SimConnect_RequestFacilityData(sim, requestID, requestID, airport.c_str());
		if (hr == SIMCONNECT_EXCEPTION_ERROR) throw std::runtime_error("airport not found: " + airport);

		waitForDone();

		std::vector<Runway> runways;

		for (int i = 1; i < result.size(); i += 13)
		{
			TempRunway tempRunway = *reinterpret_cast<TempRunway*>(result[i].data.get());
			Pavement primaryThreshold = *reinterpret_cast<Pavement*>(result[i + 1].data.get());
			Pavement secondaryThreshold = *reinterpret_cast<Pavement*>(result[i + 2].data.get());
			Pavement primaryBlastpad = *reinterpret_cast<Pavement*>(result[i + 3].data.get());
			Pavement secondaryBlastpad = *reinterpret_cast<Pavement*>(result[i + 4].data.get());
			Pavement primaryOverrun = *reinterpret_cast<Pavement*>(result[i + 5].data.get());
			Pavement secondaryOverrun = *reinterpret_cast<Pavement*>(result[i + 6].data.get());
			ApproachLights primaryApproachLights = *reinterpret_cast<ApproachLights*>(result[i + 7].data.get());
			ApproachLights secondaryApproachLights = *reinterpret_cast<ApproachLights*>(result[i + 8].data.get());
			Vasi primaryLeftVasi = *reinterpret_cast<Vasi*>(result[i + 9].data.get());
			Vasi secondaryLeftVasi = *reinterpret_cast<Vasi*>(result[i + 10].data.get());
			Vasi primaryRightVasi = *reinterpret_cast<Vasi*>(result[i + 11].data.get());
			Vasi secondaryRightVasi = *reinterpret_cast<Vasi*>(result[i + 12].data.get());

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

		dispatcher->deregisterCallback(callbackID);

		return runways;
	}

	std::vector<Approach> Facilities::getApproaches(HANDLE sim, std::string airport)
	{
		if (!isAirport(sim, airport)) throw std::runtime_error("Airport does not exist");

		Facilities facilities;
		return facilities.getApproachesForAirport(sim, airport);
	}

	std::vector<Approach> Facilities::getApproachesForAirport(HANDLE sim, std::string airport)
	{
		dispatcher = Dispatcher::getInstance(sim);
		Dispatcher::CallbackID callbackID = dispatcher->registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });

		requestID = IDCounter::getID();

		HRESULT hr;

		for (const std::string& definition : simConnectApproachDefinitions())
		{
			hr = SimConnect_AddToFacilityDefinition(sim, requestID, definition.c_str());
			if (hr == E_FAIL) throw SimConnectFailureException("there was an error adding to the facility defintion");
		}

		hr = SimConnect_RequestFacilityData(sim, requestID, requestID, airport.c_str());
		if (hr == SIMCONNECT_EXCEPTION_ERROR) throw SimConnectFailureException("airport not found: " + airport);
		
		waitForDone();

		std::vector<Approach> approaches;

		for (int i = 0; i < result.size(); i++)
		{
			switch (result[i].type)
			{
			case SIMCONNECT_FACILITY_DATA_AIRPORT:
				break; // we do not want to do anything with the airport
			case SIMCONNECT_FACILITY_DATA_APPROACH:
			{
				TempApproach tempApproach = *reinterpret_cast<TempApproach*>(result[i].data.get());

				Approach approach;
				approach.type = tempApproach.type;
				approach.suffix = tempApproach.suffix;
				approach.runwayNumber = tempApproach.runwayNumber;
				approach.runwayDesignator = tempApproach.runwayDesignator;
				approach.FAFICAO = tempApproach.FAFICAO;
				approach.FAFRegion = tempApproach.FAFRegion;
				approach.FAFHeading = tempApproach.FAFHeading;
				approach.FAFAltitude = tempApproach.FAFAltitude;
				approach.FAFType = tempApproach.FAFType;
				approach.missedAltitude = tempApproach.missedAltitude;
				approach.hasLNAV = tempApproach.hasLNAV;
				approach.hasLNAVVNAV = tempApproach.hasLNAVVNAV;
				approach.hasLP = tempApproach.hasLP;
				approach.hasLPV = tempApproach.hasLPV;
				approach.transitionsCount = tempApproach.transitionsCount;
				approach.finalApproachLegsCount = tempApproach.finalApproachLegsCount;
				approach.missedApproachLegsCount = tempApproach.missedApproachLegsCount;

				approaches.push_back(approach);
				break;
			}
			case SIMCONNECT_FACILITY_DATA_APPROACH_TRANSITION:
			{
				TempApproachTransition tempTransition = *reinterpret_cast<TempApproachTransition*>(result[i].data.get());

				ApproachTransition transition;

				transition.type = tempTransition.type;
				transition.IAFICAO = tempTransition.IAFICAO;
				transition.IAFRegion = tempTransition.IAFRegion;
				transition.IAFType = tempTransition.IAFType;
				transition.IAFAltitude = tempTransition.IAFAltitude;
				transition.DMEArcICAO = tempTransition.DMEArcICAO;
				transition.DMEArcRegion = tempTransition.DMEArcRegion;
				transition.DMEArcType = tempTransition.DMEArcType;
				transition.DMEArcRadial = tempTransition.DMEArcRadial;
				transition.DMEArcDistance = tempTransition.DMEArcDistance;
				transition.name = tempTransition.name;
				transition.approachLegsCount = tempTransition.approachLegsCount;
				
				approaches.back().approachTransitions.push_back(transition);

				break;
			}
			case SIMCONNECT_FACILITY_DATA_APPROACH_LEG:
			{
				ApproachLeg leg = *reinterpret_cast<ApproachLeg*>(result[i].data.get());
				approaches.back().approachTransitions.back().approachLegs.push_back(leg);
				break;
			}
			case SIMCONNECT_FACILITY_DATA_FINAL_APPROACH_LEG:
			{
				ApproachLeg leg = *reinterpret_cast<ApproachLeg*>(result[i].data.get());
				approaches.back().finalApproachLegs.push_back(leg);
				break;
			}
			case SIMCONNECT_FACILITY_DATA_MISSED_APPROACH_LEG:
			{
				ApproachLeg leg = *reinterpret_cast<ApproachLeg*>(result[i].data.get());
				approaches.back().missedApproachLegs.push_back(leg);
				break;
			}
			}
		}

		dispatcher->deregisterCallback(callbackID);

		return approaches;
	}
}