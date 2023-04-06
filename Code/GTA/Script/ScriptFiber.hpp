#pragma once 
#include "../../Common.hpp"

namespace Chim
{
	class ScriptFiber
	{
	public:
		explicit ScriptFiber(std::size_t count);
		~ScriptFiber();

		void Queue(std::function<void()> function);
	private:
		void FiberTick();
		static void FiberFunction();
	private:
		std::recursive_mutex m_Mutex;
		std::stack<std::function<void()>> m_Jobs;
	};

	inline ScriptFiber* g_ScriptFiber{};
}