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
		callbackID = Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) { this->callback(data); });
		HRESULT hr = SimConnect_SubscribeToSystemEvent(sim, IDCounter::getID(), eventName.c_str());
		if (FAILED(hr)) throw std::runtime_error("unable to subscribe to event " + eventName);
	}

	SystemEventSubscriber::~SystemEventSubscriber()
	{
		Dispatcher::getInstance(sim).deregisterCallback(callbackID);
	}
}