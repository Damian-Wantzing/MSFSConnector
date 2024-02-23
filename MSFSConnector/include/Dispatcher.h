#pragma once

#include <shared_mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <map>

#include <Windows.h>
#include "SimConnect.h"

namespace MSFSConnector
{
	class Dispatcher
	{
	public:
		using CallbackID = size_t;

		Dispatcher(HANDLE sim);
		~Dispatcher();

		static std::shared_ptr<Dispatcher> getInstance(HANDLE sim)
		{
			static std::shared_ptr<Dispatcher> instance = std::make_shared<Dispatcher>(sim);
			return instance;
		}

		CallbackID registerCallback(std::function<void(SIMCONNECT_RECV*)> callback);
		void deregisterCallback(CallbackID id);
		void run();

	private:

		static void CALLBACK handleStatic(SIMCONNECT_RECV* pData, DWORD cbData, void* context);
		void handle(SIMCONNECT_RECV* pData, DWORD cbData);

		HANDLE sim;
		std::shared_mutex rwMutex;
		std::map<size_t, std::function<void(SIMCONNECT_RECV*)>> callbacks;
		std::thread runThread;
		std::atomic<bool> running = false;
		size_t nextCallbackID = 0;
	};
}