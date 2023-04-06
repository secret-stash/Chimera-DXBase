#pragma once
#include "../../Common.hpp"

namespace Chim
{
	using Helper = std::function<void()>;

	class Script
	{
	public:
		explicit Script(Helper function, std::optional<std::size_t> stack = std::nullopt);
		~Script();

		void Tick();
#undef Yield
		void Yield(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);

		static Script* Current();

	private:
		void IntermediateFunction();

	private:
		void* m_ScriptFiber;
		void* m_MainFiber;
		Helper m_Function;
		std::optional<std::chrono::high_resolution_clock::time_point> m_WakeTime;

	};
}