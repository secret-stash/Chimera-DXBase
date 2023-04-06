#include "Hooking.hpp"
#include "../Game/Game.hpp"
#include "../D3D/D3DScript.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptManager.hpp"
#include "../Utility/CustomText.hpp"
#include "../Utility/Util.hpp"
#include "../D3D/D3DRenderer.hpp"
#include "../UI/UIManager.hpp"
#include "../Utility/GTA.hpp"
#include "../Features/Local/Local.hpp"
#include <MinHook/MinHook.h>

namespace Chim
{
	int Hooks::VirtualMemory(void* _this, HANDLE handle, PVOID base_addr, int info_class, MEMORY_BASIC_INFORMATION* info, int size, size_t* return_len)
	{
		return 1;
	}

	namespace { std::uint32_t g_HookFrameCount{}; }
	void Hooks::StatGetInt(rage::scrNativeCallContext* src)
	{
		const auto statHash = src->get_arg<Hash>(0);
		const auto outValue = src->get_arg<int*>(1);
		const auto p2 = src->get_arg<int>(2);

		if (g_Running && g_HookFrameCount != *g_GameVariables->m_FrameCount)
		{
			g_HookFrameCount = *g_GameVariables->m_FrameCount;
			ScriptManager::Tick();
		}

		src->set_return_value(STATS::STAT_GET_INT(statHash, outValue, p2));
	}

	const char* Hooks::GetLabelText(void* unk, const char* label)
	{
		if (g_Running)
			if (auto text = g_CustomText->GetText(Joaat(label)))
				return text;

		return static_cast<decltype(&GetLabelText)>(g_Hooking->m_OriginalGetLabelText)(unk, label);
	}

	std::uint64_t Hooks::TaskJumpConstructor(std::uint64_t a1, int a2)
	{
		if (g_LocalFeatures->m_SuperMan || g_LocalFeatures->m_SuperJump || g_LocalFeatures->m_NinjaJump)
			a2 |= 1 << 15;
		if (g_LocalFeatures->m_BeastJump)
			a2 |= (1 << 15) | (1 << 17);
		return static_cast<decltype(&TaskJumpConstructor)>(g_Hooking->m_OriginalTaskJumpConstructor)(a1, a2);
	}

	std::uint64_t* Hooks::FallTaskConstructor(std::uint64_t* _this, std::uint32_t flags)
	{
		if (g_LocalFeatures->m_GracefulLanding) {
			GTAUtility::Bits::SetBits<std::uint32_t>(&flags, 10, 11, 12, 16);
		}
		return static_cast<decltype(&FallTaskConstructor)>(g_Hooking->m_OriginalFallTaskConstructor)(_this, flags);
	}

	LRESULT Hooks::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		g_D3DRenderer->WndProc(hWnd, msg, wParam, lParam);
		return static_cast<decltype(&WndProc)>(g_Hooking->m_OriginalWndProc)(hWnd, msg, wParam, lParam);
	}

	HRESULT Hooks::Present(IDXGISwapChain* dis, UINT syncInterval, UINT flags)
	{
		if (g_Running)
		{
			g_D3DRenderer->BeginFrame();
			g_D3DScript->Tick();
			g_D3DRenderer->EndFrame();
		}

		return g_Hooking->m_D3DHook.GetOriginal<decltype(&Present)>(PresentIndex)(dis, syncInterval, flags);
	}

	HRESULT Hooks::ResizeBuffers(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
	{
		if (g_Running)
		{
			g_D3DRenderer->PreResize();
			auto hr = g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
			if (SUCCEEDED(hr))
			{
				g_D3DRenderer->PostResize();
			}

			return hr;
		}

		return g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
	}

	Hooking::Hooking():
		m_D3DHook(g_GameVariables->m_Swapchain, 18)
	{

		MH_Initialize();
		MH_CreateHook(g_GameFunctions->m_GetLabelText, &Hooks::GetLabelText, &m_OriginalGetLabelText);
		g_Logger->Info("[Hooking] Hooked: GLT");
		MH_CreateHook(g_GameFunctions->m_TaskJumpConstructor, &Hooks::TaskJumpConstructor, &m_OriginalTaskJumpConstructor);
		g_Logger->Info("[Hooking] Hooked: TJC");
		MH_CreateHook(g_GameFunctions->m_FallTaskConstructor, &Hooks::FallTaskConstructor, &m_OriginalFallTaskConstructor);
		g_Logger->Info("[Hooking] Hooked: FTC");
		MH_CreateHook(g_GameFunctions->m_WndProc, &Hooks::WndProc, &m_OriginalWndProc);
		g_Logger->Info("[Hooking] Hooked: WDP");

		m_D3DHook.Hook(&Hooks::Present, Hooks::PresentIndex);
		m_D3DHook.Hook(&Hooks::ResizeBuffers, Hooks::ResizeBuffersIndex);
	}

	Hooking::~Hooking() noexcept
	{
		MH_RemoveHook(g_GameFunctions->m_WndProc);
		g_Logger->Info("[Hooking] Unhooked: WDP");
		MH_RemoveHook(g_GameFunctions->m_FallTaskConstructor);
		g_Logger->Info("[Hooking] Unhooked: FTC");
		MH_RemoveHook(g_GameFunctions->m_TaskJumpConstructor);
		g_Logger->Info("[Hooking] Unhooked: TJC");
		MH_RemoveHook(g_GameFunctions->m_GetLabelText);
		g_Logger->Info("[Hooking] Unhooked: GLT");
		MH_Uninitialize();
	}

	void Hooking::Hook()
	{
		m_D3DHook.Enable();
		MH_EnableHook(MH_ALL_HOOKS);
	}

	void Hooking::Unhook()
	{
		m_D3DHook.Disable();
		MH_DisableHook(MH_ALL_HOOKS);
	}

	void Hooking::ModelSpawnBypass(bool enable)
	{
		*(unsigned short*)g_GameVariables->m_ModelSpawnBypass = enable ? 0x9090 : 0x0574;
	}

	void Hooking::AddOwnedExplosionBypass(bool enable)
	{
		*(unsigned short*)g_GameVariables->m_AddOwnedExplosionBypass = enable ? 0xE990 : 0x850F;
	}
}
