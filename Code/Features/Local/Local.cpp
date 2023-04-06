#include "Local.hpp"
#include "../../Common.hpp"
#include "../../GTA/Invoker/Natives.hpp"
#include "../../GTA/Script/ScriptGlobal.hpp"
#include "../../GTA/Script/ScriptManager.hpp"
#include "../../Utility/GTA.hpp"
#include "../../Utility/Util.hpp"
#include "../../Utility/Math.hpp"

// Features using g_LastFeatureName for example, restore only once to improve performance

namespace Chim
{
	void LocalFeatures::SuperMan(bool enable)
	{
		if (enable)
		{
			// Disable Controls
			PAD::DISABLE_CONTROL_ACTION(0, 21, TRUE); // Sprint
			PAD::DISABLE_CONTROL_ACTION(0, 37, TRUE); // Duck

			if (!PED::IS_PED_IN_PARACHUTE_FREE_FALL(PLAYER::PLAYER_PED_ID()))
			{
				if (ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(PLAYER::PLAYER_PED_ID()) > 4.0f)
					TASK::TASK_SKY_DIVE(PLAYER::PLAYER_PED_ID(), TRUE);
			}

			if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()))
			{
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 21))
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, 100.0f, 100.0f, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
				}
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 37))
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, -100.f, -100.0f, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
				}
			}
		}
	}

	static int g_Flip;
	int g_Speed = 6;
	void LocalFeatures::NinjaJump(bool enable)
	{
		if (enable)
		{
			if (PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID()))
			{
				if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()))
				{
					g_Flip -= g_Speed;
					if (g_Flip >= -(360 * 2))
						ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), g_Flip, 0, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 0, 0);
				}
				else
					g_Flip = 0;
			}
		}
	}

	bool g_LastFastRun = false;
	void LocalFeatures::FastRun(bool enable)
	{
		if (enable)
			GTAUtility::GetLocalPlayerInfo()->m_run_speed = 2.5f;
		else if (enable != g_LastFastRun)
			GTAUtility::GetLocalPlayerInfo()->m_run_speed = 1.f;

		g_LastFastRun = enable;
	}

	bool g_LastFastSwim = false;
	void LocalFeatures::FastSwim(bool enable)
	{
		if (enable)
			GTAUtility::GetLocalPlayerInfo()->m_swim_speed = 2.5f;
		else if (enable != g_LastFastSwim)
			GTAUtility::GetLocalPlayerInfo()->m_swim_speed = 1.f;

		g_LastFastSwim = enable;
	}

	bool g_LastNeverWanted = false;
	void LocalFeatures::NeverWanted(bool enable)
	{
		if (enable)
		{
			GTAUtility::GetLocalPlayerInfo()->m_wanted_level = 0;
			GTAUtility::GetLocalPlayerInfo()->m_wanted_level_display = 0;
			GTAUtility::GetLocalPlayerInfo()->m_is_wanted = false;
			g_GameVariables->m_MaxWantedLevel->Apply();
			g_GameVariables->m_MaxWantedLevel2->Apply();
		}
		else if (enable != g_LastNeverWanted)
		{
			g_GameVariables->m_MaxWantedLevel->Restore();
			g_GameVariables->m_MaxWantedLevel2->Restore();
		}

		g_LastNeverWanted = enable;
	}

	bool g_LastGodMode = false;
	void LocalFeatures::GodMode(bool enable)
	{
		if (enable)
			GTAUtility::Offsets::SetValue<bool>({ 0x8, 0x189 }, true);
		else if (enable != g_LastGodMode)
			GTAUtility::Offsets::SetValue<bool>({ 0x8, 0x189 }, false);

		g_LastGodMode = enable;
	}
}