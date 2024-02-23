#pragma once

#include <any>
#include <future>
#include <Windows.h>

#include "AtomicMap.h"
#include "IDCounter.h"
#include "Dispatcher.h"

namespace MSFSConnector
{
	/*The InputEventSender can be used to send custom events to MSFS.
	These custom events are different for each third party addon.*/
	class InputEventSender
	{
	public:
		InputEventSender(HANDLE sim);
		~InputEventSender();
		void callbackHandler(SIMCONNECT_RECV* data);
		// send a custom (third party addon) event to MSFS.
		void sendEvent(std::string name, std::any value, DWORD valueSize);

	private:
		bool hasEvent(std::string name);

		HANDLE sim;
		std::shared_ptr<Dispatcher> dispatcher;
		Dispatcher::CallbackID callbackID;
		DWORD requestID = 0;
		Atomics::AtomicMap<std::string, UINT64> eventHashes;
		std::promise<void> promise;
		std::future<void> future = promise.get_future();
	};
}