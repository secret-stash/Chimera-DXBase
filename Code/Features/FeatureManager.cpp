#include "FeatureManager.hpp"
#include "Local/Local.hpp"
#include "../GTA/Script/ScriptManager.hpp"

namespace Chim
{
	// Helpers
	static void LocalFeaturesTick()
	{
		while (true)
		{
			g_LocalFeatures->Tick();
			Script::Current()->Yield();
		}
	}

	void FeatureManager::Initialize()
	{
		g_LocalFeatures = std::make_unique<LocalFeatures>();
		ScriptManager::Initialize(std::make_unique<Script>(&LocalFeaturesTick));
	}

	void FeatureManager::Destroy()
	{
		g_LocalFeatures.reset();
	}
}