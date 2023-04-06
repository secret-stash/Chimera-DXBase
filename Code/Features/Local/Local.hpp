#pragma once 
#include "../../Common.hpp"

namespace Chim
{
	class LocalFeatures
	{
	public:
		void Tick()
		{
			SuperMan(m_SuperMan);
			NinjaJump(m_NinjaJump);
			FastRun(m_FastRun);
			FastSwim(m_FastSwim);
			NeverWanted(m_NeverWanted);
			GodMode(m_GodMode);
		}

		bool m_SuperMan = false;
		bool m_SuperJump = false; // TaskJumpConstructor Hook
		bool m_BeastJump = false; // TaskJumpConstructor Hook
		bool m_NinjaJump = false;
		bool m_GracefulLanding = false; // FallTaskConstructor Hook
		bool m_FastRun = false;
		bool m_FastSwim = false;
		bool m_GodMode = false;
		bool m_NeverWanted = false;
	private:
		void SuperMan(bool enable);
		void NinjaJump(bool enable);
		void FastRun(bool enable);
		void FastSwim(bool enable);
		void GodMode(bool enable);
		void NeverWanted(bool enable);
	};

	inline std::unique_ptr<LocalFeatures> g_LocalFeatures;
}