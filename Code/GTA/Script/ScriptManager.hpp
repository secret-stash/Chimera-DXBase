#pragma once
#include "Script.hpp"

namespace Chim::ScriptManager
{
	extern void Initialize(std::unique_ptr<Script> script);
	extern void Destroy();

	extern void Tick();

	inline std::recursive_mutex Mutex;
	inline std::vector<std::unique_ptr<Script>> Scripts;
}