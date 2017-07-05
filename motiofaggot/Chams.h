#pragma once

#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// Our stuff
#include "Entities.h"
#include "offsets.h"
#include "ClientRecvProps.h"
#include "BaseClient.h"
#include "EngineClient.h"
#include "MiscClasses.h"
#include "Surface.h"
#include "Materials.h"


// Valve
#include "Vector.h"
#include "Vector2D.h"
#include "bspflags.h"

// Management Classes
#include "Interfaces.h"

// Functions
#include "UTIL Functions.h"
#include "MathFunctions.h"
#include "checksum_md5.h"
void InitKeyValues(KeyValues* keyValues, char* name);

void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer);

void ForceMaterial(Color color, IMaterial* material, bool useColor = true, bool forceMaterial = true);

IMaterial *CreateMaterial(bool shouldIgnoreZ, bool isLit = true, bool isWireframe = false);