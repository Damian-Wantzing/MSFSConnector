#include "dispatcher.h"

Dispatcher::Dispatcher(HANDLE sim)
{
	this->sim = sim;
}

bool Dispatcher::subscribe(void (*callback)(SIMCONNECT_RECV* data))
{
	callbacks.push_back(callback);
}

bool Dispatcher::unsubscribe(void (*callback)(SIMCONNECT_RECV* data))
{
	for (size_t i = 0; i < callbacks.size(); ++i)
	{
		if (callbacks[i] != callback) continue;
		
		callbacks.erase(this->callbacks.begin() + i);
	}
}

void Dispatcher::run()
{
	while (true)
	{
		SimConnect_CallDispatch(sim, Dispatcher::handleStatic, this);
	}
}

void Dispatcher::handleStatic(SIMCONNECT_RECV* pData, DWORD cbData, void* context)
{
	Dispatcher* dispatcher = static_cast<Dispatcher*>(context);
	dispatcher->handle(pData, cbData);
}

void CALLBACK Dispatcher::handle(SIMCONNECT_RECV* pData, DWORD cbData)
{
	for (void(*callback)(SIMCONNECT_RECV* data)  : callbacks)
	{
		callback(pData);
	}
}