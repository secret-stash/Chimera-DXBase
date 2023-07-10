#include "Game.hpp"
#include "BytePatch.hpp"
#include "../Hooking/Hooking.hpp"

namespace Chim
{
	GameVariables::GameVariables():
		m_GameState(Signature("GS", "48 85 C9 74 4B 83 3D").Scan().Add(7).Rip().As<decltype(m_GameState)>()),
		m_FrameCount(Signature("FC", "8B 15 ? ? ? ? 41 FF CF").Scan().Add(2).Rip().As<decltype(m_FrameCount)>()),
		m_IsSessionStarted(Signature("ISS", "40 38 35 ? ? ? ? 75 0E 4C 8B C3 49 8B D7 49 8B CE").Scan().Add(3).Rip().As<decltype(m_IsSessionStarted)>()),
		m_GameWindow(FindWindowA("grcWindow", "Grand Theft Auto V")),
		m_Swapchain(Signature("SC", "44 38 35 ? ? ? ? 48 8B 01").Scan().Sub(13).Rip().As<decltype(m_Swapchain)&>()),
		m_ScriptProgramTable(Signature("SPT", "48 8B 1D ? ? ? ? 41 83 F8 FF").Scan().Add(3).Rip().As<decltype(m_ScriptProgramTable)>()),
		m_NativeRegistrationTable(Signature("NRT", "76 32 48 8B 53 40 48 8D 0D").Scan().Add(9).Rip().As<decltype(m_NativeRegistrationTable)>()),
		m_GlobalBase(Signature("SGB", "48 85 FF 48 89 1D").Scan().Sub(12).Rip().As<decltype(m_GlobalBase)>()),
		m_PedFactory(Signature("PF", "48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81").Scan().Add(3).Rip().As<decltype(m_PedFactory)>()),
		m_NetworkPlayerMgr(Signature("NPM", "48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF").Scan().Add(3).Rip().As<decltype(m_NetworkPlayerMgr)>()),
		m_ModelSpawnBypass(Signature("MSB", "48 8B C8 FF 52 30 84 C0 74 05 48").Scan().Add(8).As<decltype(m_ModelSpawnBypass)>()),
		m_AddOwnedExplosionBypass(Signature("AOEB", "0F 85 ? ? ? ? 48 8B 05 ? ? ? ? 48 8B 48 08 E8").Scan().As<decltype(m_AddOwnedExplosionBypass)>())
	{
		m_GameBuild = Signature("GB", "48 83 EC 60 48 8D 0D ? ? ? ? E8").Scan().Sub(17).Add(268).Rip().As<decltype(m_GameBuild)>();
		m_MaxWantedLevel = Game::BytePatch::Make(Signature("MWL", "8B 43 6C 89 05").Scan().Add(5).Rip().As<std::uint32_t*>(), 0).get();
		g_Logger->Info("[Memory] Patched: MWL");
		m_MaxWantedLevel2 = Game::BytePatch::Make(Signature("MWL2", "8B 43 6C 89 05").Scan().Add(14).Rip().As<std::uint32_t*>(), 0).get();
		g_Logger->Info("[Memory] Patched: MWL2");
	}

	GameFunctions::GameFunctions():
		m_GetLabelText(Signature("GLT", "48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 85 D2 75 44 E8").Scan().As<decltype(m_GetLabelText)>()),
		m_TaskJumpConstructor(Signature("TJC", "48 89 5C 24 ? 89 54 24 10 57 48 83 EC 30 0F 29 74 24").Scan().As<decltype(m_TaskJumpConstructor)>()),
		m_FallTaskConstructor(Signature("FTC", "E8 ? ? ? ? B3 04 08 98 A0 00 00 00").Scan().Add(1).Rip().As<decltype(m_FallTaskConstructor)>()),
		m_WndProc(Signature("WDP", "44 8B E2 4C 8B E9 3B D0").Scan().Sub(52).As<decltype(m_WndProc)>()),
		m_QueueDependency(Signature("QD", "48 89 5C 24 ? 57 48 83 EC ? 0F B6 99").Scan().As<decltype(m_QueueDependency)>()),
		m_GetNativeHandler(Signature("GNH", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").Scan().Add(12).Rip().As<decltype(m_GetNativeHandler)>()),
		m_FixVectors(Signature("FV", "83 79 18 ? 48 8B D1 74 4A FF 4A 18").Scan().As<decltype(m_FixVectors)>()),
		m_GetPlayerName(Signature("GPN", "85 D2 7E 0E 48 8B C8").Scan().Add(28).As<decltype(m_GetPlayerName)>()),
		m_GetNetPlayer(Signature("GNP", "48 83 EC 28 33 C0 38 05 ? ? ? ? 74 0A").Scan().As<decltype(m_GetNetPlayer)>())
	{
	}
}
