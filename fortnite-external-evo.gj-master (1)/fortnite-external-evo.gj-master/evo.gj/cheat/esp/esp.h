#pragma once 
#include "../../util/util.h"
#include "../sdk/sdk.h"
#include "../aimbot/aimbot.h"
class Esp {
public:
	static void ActorLoop();
};
inline bool bCornerBox = true;
inline bool bFovCircle = false;
inline float fovSize = 120.0f;

namespace Debug {
	static bool PrintPointers = 0;
	static bool PrintLocations = false;
}