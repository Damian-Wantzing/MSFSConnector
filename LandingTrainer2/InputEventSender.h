#pragma once

#include <any>
#include <future>
#include <Windows.h>

#include "AtomicMap.h"
#include "IDCounter.h"
#include "Dispatcher.h"

namespace MSFSConnector
{
	class InputEventSender
	{
	public:
		InputEventSender(HANDLE sim);
		~InputEventSender();
		void callbackHandler(SIMCONNECT_RECV* data);
		void sendEvent(std::string name, std::any value, DWORD valueSize);

	private:
		bool hasEvent(std::string name);

		HANDLE sim;
		Dispatcher::CallbackID callbackID;
		DWORD requestID = 0;
		Atomics::AtomicMap<std::string, UINT64> eventHashes;
		std::promise<void> promise;
		std::future<void> future = promise.get_future();
	};
}