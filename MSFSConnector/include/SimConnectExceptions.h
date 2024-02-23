#pragma once

#include <stdexcept>

namespace MSFSConnector
{
	class SimConnectUnresponsiveException : public std::runtime_error
	{
	public:
		explicit SimConnectUnresponsiveException(const std::string& message) : std::runtime_error(message) {}
	};

	class SimConnectFailureException : public std::runtime_error
	{
	public:
		explicit SimConnectFailureException(const std::string& message) : std::runtime_error(message) {}
	};
}