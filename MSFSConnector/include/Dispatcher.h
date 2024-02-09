#pragma once

#include <vector>
#include <shared_mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <unordered_map>

#include <Windows.h>
#include "SimConnect.h"

namespace MSFSConnector
{
	class Dispatcher
	{
	public:
		using CallbackID = size_t;

		static Dispatcher& getInstance(HANDLE sim)
		{
			static Dispatcher instance(sim);
			return instance;
		}

		Dispatcher(Dispatcher const&) = delete;
		void operator=(Dispatcher const&) = delete;

		CallbackID registerCallback(std::function<void(SIMCONNECT_RECV*)> callback);
		void deregisterCallback(CallbackID id);
		void run();

	private:
		Dispatcher(HANDLE sim);
		~Dispatcher();

		static void CALLBACK handleStatic(SIMCONNECT_RECV* pData, DWORD cbData, void* context);
		void handle(SIMCONNECT_RECV* pData, DWORD cbData);

		HANDLE sim;
		std::shared_mutex rwMutex;
		std::unordered_map<size_t, std::function<void(SIMCONNECT_RECV*)>> callbacks;
		std::thread runThread;
		std::atomic<bool> running = false;
		size_t nextCallbackID = 0;
	};
}