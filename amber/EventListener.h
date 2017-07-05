#pragma once
#include "SDK.h"
#include "Hitmarker.h"

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

class EventListener : public IGameEventListener2
{
public:
	EventListener(std::vector<const char*> events)
	{
		for (auto& it : events)
			Interfaces::GameEventManager->AddListener(this, it, false);
	}

	~EventListener()
	{
		Interfaces::GameEventManager->RemoveListener(this);
	}

	void FireGameEvent(IGameEvent* event)
	{
		HitMarkers::FireGameEvent(event);
	}

	int GetEventDebugID() override
	{
		return EVENT_DEBUG_ID_INIT;
	}
};