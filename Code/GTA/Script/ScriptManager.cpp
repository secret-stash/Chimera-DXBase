#include "ScriptManager.hpp"

namespace Chim
{
	void ScriptManager::Initialize(std::unique_ptr<Script> script)
	{
		std::lock_guard lock(Mutex);
		Scripts.push_back(std::move(script));
	}

	void ScriptManager::Destroy()
	{
		std::lock_guard lock(Mutex);
		Scripts.clear();
	}

	void ScriptManager::Tick()
	{
		static bool Convert = (ConvertThreadToFiber(nullptr), true);

		std::lock_guard lock(Mutex);
		for (auto const& script : Scripts)
		{
			script->Tick();
		}
	}
}