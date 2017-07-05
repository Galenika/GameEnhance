// Ares
#pragma once

// Dependencies
#include <Windows.h>
#include <process.h>
#include <string>
#include <vector>
#include <math.h>
#include <thread>
#include <Psapi.h>

typedef float matrix3x4[3][4];

// Setup
#include "Setup.h"

// Utilities
#include "VirtualMethod.h"
#include "GetVFunc.h"
#include "Vector.h"
#include "Color.h"
#include "Math.h"
#include "Draw.h"

// SDK
#include "IClientEntityList.h"
#include "IClientModeShared.h"
#include "IVEngineClient.h"
#include "CHLClient.h"
#include "CUserCmd.h"
#include "CBaseEntity.h"
#include "ISurface.h"
#include "IPanel.h"