#include "NativeHooking.hpp"
#include "Hooking.hpp"
#include "../Game/Game.hpp"

namespace Chim
{
	auto ALL_SCRIPT_HASH = Joaat("ALL_SCRIPTS");

	NativeHooks::NativeHooks()
	{
		AddNativeDetour("SGI", Joaat("main_persistent"), 0x767FBC2AC802EF3D, Hooks::StatGetInt);

		for (auto& entry : *g_GameVariables->m_ScriptProgramTable)
			if (entry.m_program)
				HookProgram(entry.m_program);

		g_NativeHooks = this;
	}

	NativeHooks::~NativeHooks()
	{
		m_ScriptHooks.clear();
		g_NativeHooks = nullptr;
	}

	void NativeHooks::AddNativeDetour(const char* name, rage::scrNativeHash hash, rage::scrNativeHandler detour)
	{
		AddNativeDetour(name, ALL_SCRIPT_HASH, hash, detour);
	}

	void NativeHooks::AddNativeDetour(const char* name, rage::joaat_t script_hash, rage::scrNativeHash hash, rage::scrNativeHandler detour)
	{
		if (const auto& it = m_NativeRegistrations.find(script_hash); it != m_NativeRegistrations.end())
		{
			it->second.emplace_back(hash, detour);
			return;
		}

		m_NativeRegistrations.emplace(script_hash, std::vector<NativeDetour>({ { hash, detour } }));

		g_Logger->Info("[Natives] Hooked: %s", name);
	}

	void NativeHooks::HookProgram(rage::scrProgram* program)
	{
		std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> native_replacements;
		const auto script_hash = program->m_name_hash;

		// Functions that need to be detoured for all scripts
		if (const auto& pair = m_NativeRegistrations.find(ALL_SCRIPT_HASH); pair != m_NativeRegistrations.end())
			for (const auto& native_hook_reg : pair->second)
				native_replacements.insert(native_hook_reg);

		// Functions that only need to be detoured for a specific script
		if (const auto& pair = m_NativeRegistrations.find(script_hash); pair != m_NativeRegistrations.end())
			for (const auto& native_hook_reg : pair->second)
				native_replacements.insert(native_hook_reg);

		if (!native_replacements.empty())
		{
			m_ScriptHooks.emplace(
				program,
				std::make_unique<ScriptHook>(program, native_replacements)
			);
		}
	}

	void NativeHooks::UnhookProgram(rage::scrProgram* program)
	{
		m_ScriptHooks.erase(program);
	}
}