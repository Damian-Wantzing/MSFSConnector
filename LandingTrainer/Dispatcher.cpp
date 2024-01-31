#include "dispatcher.h"

Dispatcher::Dispatcher(HANDLE sim)
{
	this->sim = sim;
	running.store(true);

	runThread = std::thread(&Dispatcher::run, this);
}

Dispatcher::~Dispatcher()
{
	running.store(false);
	runThread.join();
}

void Dispatcher::subscribe(void (*callback)(SIMCONNECT_RECV* data))
{
	rwMutex.lock();
	callbacks.push_back(callback);
	rwMutex.unlock();
}

void Dispatcher::unsubscribe(void (*callback)(SIMCONNECT_RECV* data))
{
	rwMutex.lock();
	for (size_t i = 0; i < callbacks.size(); ++i)
	{
		if (callbacks[i] != callback) continue;
		
		callbacks.erase(this->callbacks.begin() + i);
	}
	rwMutex.unlock();
}

void Dispatcher::run()
{
	while (running)
	{
		SimConnect_CallDispatch(sim, Dispatcher::handleStatic, this);
		Sleep(1);
	}
}

void CALLBACK Dispatcher::handleStatic(SIMCONNECT_RECV* pData, DWORD cbData, void* context)
{
	printf("called handlestatic");
	Dispatcher* dispatcher = static_cast<Dispatcher*>(context);
	dispatcher->handle(pData, cbData);
}

void Dispatcher::handle(SIMCONNECT_RECV* pData, DWORD cbData)
{
	rwMutex.lock_shared();
	for (void(*callback)(SIMCONNECT_RECV* data)  : callbacks)
	{
		callback(pData);
	}
	rwMutex.unlock_shared();
}