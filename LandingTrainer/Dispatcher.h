#pragma once

#include <vector>
#include <Windows.h>
#include <shared_mutex>
#include <thread>
#include <atomic>

#include "SimConnect.h"

class Dispatcher
{
public:
	Dispatcher(HANDLE sim);
	~Dispatcher();

	void subscribe(void (*callback)(SIMCONNECT_RECV* data));
	void unsubscribe(void (*callback)(SIMCONNECT_RECV* data));
	void run();

private:
	static void CALLBACK handleStatic(SIMCONNECT_RECV* pData, DWORD cbData, void* context);
	void handle(SIMCONNECT_RECV* pData, DWORD cbData);
	
	HANDLE sim;
	std::shared_mutex rwMutex;
	std::vector<void(*)(SIMCONNECT_RECV* data)> callbacks;
	std::thread runThread;
	std::atomic<bool> running = false;
};