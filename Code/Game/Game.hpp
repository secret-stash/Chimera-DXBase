#pragma once
#include "BytePatch.hpp"

// GTA-V
#include <GTAV-Classes/script/scrProgramTable.hpp>
#include <GTAV-Classes/script/scrNativeHandler.hpp>
#include <GTAV-Classes/script/scrNativeRegistration.hpp>
#include <GTAV-Classes/script/scrNativeRegistrationTable.hpp>
#include <GTAV-Classes/ped/CPedFactory.hpp>
#include <GTAV-Classes/ped/CPed.hpp>
#include <GTAV-Classes/player/CPlayerInfo.hpp>
#include <GTAV-Classes/network/CNetworkPlayerMgr.hpp>
#include <GTAV-Classes/network/CNetGamePlayer.hpp>

namespace Chim
{
	class GameVariables
	{
	public:
		explicit GameVariables();
		~GameVariables() noexcept = default;
		GameVariables(GameVariables const&) = delete;
		GameVariables(GameVariables&&) = delete;
		GameVariables& operator=(GameVariables const&) = delete;
		GameVariables& operator=(GameVariables&&) = delete;

		std::uint32_t* m_GameState;
		std::uint32_t* m_FrameCount;
		bool* m_IsSessionStarted;
		HWND m_GameWindow;
		IDXGISwapChain* m_Swapchain;
		rage::scrProgramTable* m_ScriptProgramTable;
		rage::scrNativeRegistrationTable* m_NativeRegistrationTable;
		void** m_AssignReturnAddress;
		std::int64_t** m_GlobalBase;
		CPedFactory** m_PedFactory;
		CNetworkPlayerMgr** m_NetworkPlayerMgr;
		PVOID m_ModelSpawnBypass;
		std::uint16_t* m_AddOwnedExplosionBypass;
		char* m_GameBuild;
		Game::BytePatch* m_MaxWantedLevel;
		Game::BytePatch* m_MaxWantedLevel2;
	};

	class GameFunctions
	{
	public:
		explicit GameFunctions();
		~GameFunctions() noexcept = default;
		GameFunctions(GameFunctions const&) = delete;
		GameFunctions(GameFunctions&&) = delete;
		GameFunctions& operator=(GameFunctions const&) = delete;
		GameFunctions& operator=(GameFunctions&&) = delete;

		using GetLabelText = const char*(void* unk, const char* label);
		GetLabelText* m_GetLabelText;

		using TaskJumpConstructor = std::uint64_t(std::uint64_t a1, int a2);
		TaskJumpConstructor* m_TaskJumpConstructor;

		using FallTaskConstructor = std::uint64_t*(std::uint64_t* _this, std::uint32_t flags);
		FallTaskConstructor* m_FallTaskConstructor;

		using WndProc = LRESULT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		WndProc* m_WndProc;

		using GetNativeHandler = rage::scrNativeHandler(rage::scrNativeRegistrationTable*, rage::scrNativeHash);
		GetNativeHandler* m_GetNativeHandler;

		using FixVectors = void(rage::scrNativeCallContext*);
		FixVectors* m_FixVectors;

		using GetPlayerName = const char*(std::uint32_t id);
		GetPlayerName* m_GetPlayerName;

		using GetNetPlayer = CNetGamePlayer*(std::int32_t id);
		GetNetPlayer* m_GetNetPlayer;
	};

	inline std::unique_ptr<GameVariables> g_GameVariables;
	inline std::unique_ptr<GameFunctions> g_GameFunctions;
}
