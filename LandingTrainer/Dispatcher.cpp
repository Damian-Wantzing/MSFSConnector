#include "Dispatcher.h"

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

Dispatcher::CallbackID Dispatcher::registerCallback(std::function<void(SIMCONNECT_RECV*)> callback)
{
	rwMutex.lock();
	callbacks.insert(std::make_pair(nextCallbackID, callback));
	rwMutex.unlock();

	return nextCallbackID++;
}

void Dispatcher::deregisterCallback(CallbackID id)
{
	rwMutex.lock();
	callbacks.erase(id);
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
	Dispatcher* dispatcher = static_cast<Dispatcher*>(context);
	dispatcher->handle(pData, cbData);
}

void Dispatcher::handle(SIMCONNECT_RECV* pData, DWORD cbData)
{
	rwMutex.lock_shared();
	for (std::map<size_t, std::function<void (SIMCONNECT_RECV *)>>::iterator it = callbacks.begin(); it != callbacks.end(); ++it)
	{
		it->second(pData);
	}
	rwMutex.unlock_shared();
}