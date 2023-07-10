#pragma once 
#include "../../Hooking/VMTHook.hpp"
#include "../Invoker/Invoker.hpp"

namespace Chim
{
	class ScriptHook
	{
	public:
		explicit ScriptHook(rage::joaat_t script_hash, std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> replacements);
		explicit ScriptHook(rage::scrProgram* program, std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> replacements);
		~ScriptHook();

		void Ensure();
	private:
		void HookInstance(rage::scrProgram* program);

		static void scrProgramDetour(rage::scrProgram* this_, bool free_memory);
		static std::unordered_map<rage::scrProgram*, ScriptHook*> s_Map;

		rage::joaat_t m_ScriptHash;
		rage::scrProgram* m_Program;
		std::unique_ptr<VMTHook> m_VMTHook;
		std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> m_NativeReplacementsPointer;
		std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler*> m_NativeHandlerPointer;
	};
}