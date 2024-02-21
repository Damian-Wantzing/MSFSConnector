#include "SystemEventSubscriber.h"

#include "Dispatcher.h"
#include "IDCounter.h"
#include <SimConnect.h>
#include <Windows.h>

namespace MSFSConnector
{
	SystemEventSubscriber::SystemEventSubscriber(HANDLE sim, std::string eventName, std::function<void(SIMCONNECT_RECV*)> callback)
	{
		this->sim = sim;
		this->eventName = eventName;
		this->callback = callback;
		requestID = IDCounter::getID();
		callbackID = Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });
		HRESULT hr = SimConnect_SubscribeToSystemEvent(sim, requestID, eventName.c_str());
		if (FAILED(hr)) throw std::runtime_error("unable to subscribe to event " + eventName);
	}

	SystemEventSubscriber::~SystemEventSubscriber()
	{
		Dispatcher::getInstance(sim).deregisterCallback(callbackID);
	}

	void SystemEventSubscriber::callbackHandler(SIMCONNECT_RECV* data)
	{
		if (data->dwID != SIMCONNECT_RECV_ID_EVENT) return;

		SIMCONNECT_RECV_EVENT* event = (SIMCONNECT_RECV_EVENT*)data;
		if (event->uEventID != requestID) return;

		callback(data);
	}
}