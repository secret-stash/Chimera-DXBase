#include "Game/Game.hpp"
#include "Game/BytePatch.hpp"
#include "Hooking/Hooking.hpp"
#include "Hooking/NativeHooking.hpp"
#include "GTA/Script/ScriptFiber.hpp"
#include "GTA/Script/ScriptManager.hpp"
#include "D3D/D3DScript.hpp"
#include "Menu/MainScript.hpp"
#include "Utility/CustomText.hpp"
#include "D3D/D3DRenderer.hpp"
#include "UI/UIManager.hpp"
#include "Features/FeatureManager.hpp"

/**
 * \brief The entry point of the library
 * \param hInstance The base address of the DLL
 * \param reason The reason for this function being called
 */
BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID)
{
	using namespace Chim;

	if (reason == DLL_PROCESS_ATTACH)
	{
		g_Module = hInstance;
		CreateThread(nullptr, 0, [](LPVOID) -> DWORD
		{
			g_Logger = std::make_unique<Logger>();
			g_Logger->Info(std::format("[Main] {} | Version: {} | Has been injected", CHIMERA_NAME, CHIMERA_VERSION).c_str());

			g_GameFunctions = std::make_unique<GameFunctions>();
			g_GameVariables = std::make_unique<GameVariables>();

			// Wait for the game to load
			while (*g_GameVariables->m_GameState != 0)
			{
				std::this_thread::sleep_for(3s);
				std::this_thread::yield();
			}

			g_CustomText = std::make_unique<CustomText>();
			g_D3DRenderer = std::make_unique<D3DRenderer>();
			g_UiManager = std::make_unique<UserInterface::UIManager>();

			auto ScriptFiberInstance = std::make_unique<ScriptFiber>(10);
			g_MainScript = std::make_shared<MainScript>();
			ScriptManager::Initialize(std::make_unique<Script>(&MainScript::Tick));
			FeatureManager::Initialize();
			g_D3DScript = std::make_shared<D3DScript>();

			g_Hooking = std::make_unique<Hooking>();
			g_Hooking->Hook();

			auto g_NativeHooking = std::make_unique<NativeHooks>();

			g_Logger->Info(std::format("[Main] {} | Version: {} | Has been loaded", CHIMERA_NAME, CHIMERA_VERSION).c_str());
			while (g_Running)
			{
				if (IsKeyPressed(VK_DELETE))
					g_Running = false;

				std::this_thread::sleep_for(3ms);
				std::this_thread::yield();
			}

			std::this_thread::sleep_for(500ms);

			g_NativeHooking.reset();
			g_Hooking->Unhook();

			g_D3DScript.reset();
			FeatureManager::Destroy();
			ScriptManager::Destroy();
			g_MainScript.reset();
			ScriptFiberInstance.reset();

			g_UiManager.reset();
			g_D3DRenderer.reset();
			g_CustomText.reset();

			std::this_thread::sleep_for(500ms);

			g_Hooking.reset();

			Game::BytePatch::RestoreAll();
			g_GameVariables.reset();
			g_GameFunctions.reset();
			
			g_Logger->Info(std::format("[Main] {} | Version: {} | Has been unloaded", CHIMERA_NAME, CHIMERA_VERSION).c_str());
			g_Logger->Info("------Made with love by Chimera#2000------");
			g_Logger.reset();

			FreeLibraryAndExitThread(g_Module, 0);
		}, nullptr, 0, nullptr);
	}

	return true;
}
