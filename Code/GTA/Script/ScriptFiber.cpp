#include "ScriptFiber.hpp"
#include "Script.hpp"
#include "ScriptManager.hpp"

namespace Chim
{
	ScriptFiber::ScriptFiber(std::size_t num_fibers)
	{
		for (std::size_t i = 0; i < num_fibers; ++i)
		{
			ScriptManager::Initialize(std::make_unique<Script>(&FiberFunction));
		}

		g_ScriptFiber = this;
	}

	ScriptFiber::~ScriptFiber()
	{
		g_ScriptFiber = nullptr;
	}

	void ScriptFiber::Queue(std::function<void()> function)
	{
		if (function)
		{
			std::lock_guard lock(m_Mutex);
			m_Jobs.push(std::move(function));
		}
	}

	void ScriptFiber::FiberTick()
	{
		std::unique_lock lock(m_Mutex);
		if (!m_Jobs.empty())
		{
			const auto job = std::move(m_Jobs.top());
			m_Jobs.pop();
			lock.unlock();

			std::invoke(std::move(job));
		}
	}

	void ScriptFiber::FiberFunction()
	{
		while (true)
		{
			g_ScriptFiber->FiberTick();
			Script::Current()->Yield();
		}
	}
}