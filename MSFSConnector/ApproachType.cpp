#include "ApproachType.h"

namespace MSFSConnector
{
	std::string ApproachTypeToString(ApproachType type)
	{
		switch (type)
		{
		case UNDEFINED: return "Undefined";
		case GPS: return "GPS";
		case VOR: return "VOR";
		case NDB: return "NDB";
		case ILS: return "ILS";
		case LOCALIZER: return "Localizer";
		case SDF: return "SDF";
		case LDA: return "LDA";
		case VORDME: return "VORDME";
		case NDBDME: return "NDBDME";
		case RNAV: return "RNAV";
		case LOCALIZER_BACK_COURSE: return "Localizer Backcourse";
		}
	}
}