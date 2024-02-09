#include "Dispatcher.h"

namespace MSFSConnector
{
	Dispatcher::Dispatcher(HANDLE sim)
	{
		this->sim = sim;
		running.store(true);

		runThread = std::thread(&Dispatcher::run, this);
	}

	Dispatcher::~Dispatcher()
	{
		running.store(false);
		if (runThread.joinable())
		{
			runThread.join();
		}
	}

	Dispatcher::CallbackID Dispatcher::registerCallback(std::function<void(SIMCONNECT_RECV*)> callback)
	{
		std::unique_lock<std::shared_mutex> lock(rwMutex);
		callbacks.insert(std::make_pair(nextCallbackID, callback));

		return nextCallbackID++;
	}

	void Dispatcher::deregisterCallback(CallbackID id)
	{
		std::unique_lock<std::shared_mutex> lock(rwMutex);
		callbacks.erase(id);
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
		std::shared_lock<std::shared_mutex> lock(rwMutex);
		for (std::unordered_map<size_t, std::function<void(SIMCONNECT_RECV*)>>::iterator it = callbacks.begin(); it != callbacks.end(); ++it)
		{
			it->second(pData);
		}
	}
}