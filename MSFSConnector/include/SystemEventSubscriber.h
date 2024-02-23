#pragma once

#include <Windows.h>
#include <SimConnect.h>
#include <string>
#include <functional>
#include "Dispatcher.h"

namespace MSFSConnector
{
	class SystemEventSubscriber
	{
	public:
		SystemEventSubscriber(HANDLE sim, std::string eventName, std::function<void(SIMCONNECT_RECV*)> callback);
		~SystemEventSubscriber();
		void callbackHandler(SIMCONNECT_RECV* data);

	private:
		bool isEvent(DWORD dwID);

		HANDLE sim;
		std::shared_ptr<Dispatcher> dispatcher;
		std::string eventName;
		std::function<void(SIMCONNECT_RECV*)> callback;
		Dispatcher::CallbackID callbackID;
		DWORD requestID;
	};
}