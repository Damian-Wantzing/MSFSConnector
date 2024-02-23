#include "Dispatcher.h"

#include "SimConnectExceptions.h"

namespace MSFSConnector
{
	Dispatcher::Dispatcher(HANDLE sim)
	{
		this->sim = sim;

		runThread = std::thread(&Dispatcher::run, this);
		running.store(true);
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
			HRESULT hr;

			try
			{
				hr = SimConnect_CallDispatch(sim, Dispatcher::handleStatic, this);
			}
			catch (const std::exception&)
			{
				throw SimConnectUnresponsiveException("There was an error connecting to the sim");
			}

			if (FAILED(hr)) throw SimConnectFailureException("there was an error calling dispatch");
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
		for (std::map<size_t, std::function<void(SIMCONNECT_RECV*)>>::iterator it = callbacks.begin(); it != callbacks.end(); ++it)
		{
			it->second(pData);
		}
	}
}