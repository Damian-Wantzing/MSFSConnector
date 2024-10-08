#pragma once

#include <map>
#include <any>
#include <future>
#include <atomic>

#include <windows.h>
#include "SimConnect.h"

#include "AtomicList.h"
#include "AtomicMap.h"
#include "Dispatcher.h"
#include "SimVar.h"
#include "IDCounter.h"

namespace MSFSConnector
{
	/* Watch one or more SimVars from MSFS.
	Create an instance of SimVarWatcher and use the addSimVar method to watch a SimVar.
	Watched SimVars can subsequently be read using the get<Type>() method.*/
	class SimVarWatcher
	{
	public:
		SimVarWatcher(HANDLE sim, SIMCONNECT_PERIOD interval, SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER);
		~SimVarWatcher();
		void addSimVar(SimVar simVar);
		void removeSimVar(std::string name);
		void callbackHandler(SIMCONNECT_RECV* data);

		template <typename T>
		T get(std::string name)
		{
			future.wait();

			if (simVarResults.count(name) == 0) throw std::runtime_error("Requested SimVar is not available");
			if (simVarResults.get(name).type() != typeid(T)) throw std::runtime_error("Provided type does not match the SimVar type");
			return std::any_cast<T>(simVarResults.get(name));
		}
	private:
		void addDataDefinitions();

		HANDLE sim;
		std::shared_ptr<Dispatcher> dispatcher;
		Dispatcher::CallbackID callbackID;
		SIMCONNECT_PERIOD interval;
		SIMCONNECT_OBJECT_ID objectID;
		Atomics::AtomicList<SimVar> simVars;
		Atomics::AtomicMap<std::string, std::any> simVarResults;
		DWORD simConnectWatcherID = 0; // The watcherID is only used for the request and should not be used to tell apart two different watchers, since this variable is prone to changing when adding or removing SimVars
		std::promise<void> promise;
		std::shared_future<void> future = promise.get_future();
		std::atomic<bool> promiseSet{ false };
	};
}