#include "SystemEventSubscriber.h"

#include "Dispatcher.h"
#include "IDCounter.h"
#include <SimConnect.h>
#include <Windows.h>

#include "SimConnectExceptions.h"

namespace MSFSConnector
{
	SystemEventSubscriber::SystemEventSubscriber(HANDLE sim, std::string eventName, std::function<void(SIMCONNECT_RECV*)> callback)
	{
		this->sim = sim;
		this->eventName = eventName;
		this->callback = callback;
		requestID = IDCounter::getID();
		dispatcher = Dispatcher::getInstance(sim);
		callbackID = dispatcher->registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });

		HRESULT hr;

		hr = SimConnect_SubscribeToSystemEvent(sim, requestID, eventName.c_str());
		if (hr == E_FAIL) throw SimConnectFailureException("unable to subscribe to event " + eventName);
	}

	SystemEventSubscriber::~SystemEventSubscriber()
	{
		dispatcher->deregisterCallback(callbackID);
	}

	void SystemEventSubscriber::callbackHandler(SIMCONNECT_RECV* data)
	{
		if (!isEvent(data->dwID)) return;

		SIMCONNECT_RECV_EVENT* event = (SIMCONNECT_RECV_EVENT*)data;
		if (event->uEventID != requestID) return;

		callback(data);
	}

	bool SystemEventSubscriber::isEvent(DWORD dwID)
	{
		if (dwID == SIMCONNECT_RECV_ID_EVENT) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT_OBJECT_ADDREMOVE) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT_FILENAME) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT_FRAME) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT_MULTIPLAYER_SERVER_STARTED) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT_MULTIPLAYER_CLIENT_STARTED) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT_MULTIPLAYER_SESSION_ENDED) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT_RACE_END) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT_RACE_LAP) return true;
		if (dwID == SIMCONNECT_RECV_ID_EVENT_EX1) return true;

		return false;
	}

}