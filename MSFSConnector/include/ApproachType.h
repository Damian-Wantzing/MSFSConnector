#pragma once

#include <string>
namespace MSFSConnector
{
	enum ApproachType
	{
		UNDEFINED,
		GPS,
		VOR,
		NDB,
		ILS,
		LOCALIZER,
		SDF,
		LDA,
		VORDME,
		NDBDME,
		RNAV,
		LOCALIZER_BACK_COURSE
	};

	std::string ApproachTypeToString(ApproachType type);
}