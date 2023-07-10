#pragma once
#include "VMTHook.hpp"

namespace Chim
{
	struct Hooks
	{
		static void StatGetInt(rage::scrNativeCallContext* src);
		static const char* GetLabelText(void* unk, const char* label);
		static std::uint64_t TaskJumpConstructor(std::uint64_t a1, int a2);
		static std::uint64_t* FallTaskConstructor(std::uint64_t* _this, std::uint32_t flags);
		static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static void QueueDependency(void* dependency);

		static constexpr auto PresentIndex = 8;
		static HRESULT Present(IDXGISwapChain* dis, UINT syncInterval, UINT flags);

		static constexpr auto ResizeBuffersIndex = 13;
		static HRESULT ResizeBuffers(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
	};

	class Hooking
	{
	public:
		explicit Hooking();
		~Hooking() noexcept;
		Hooking(Hooking const&) = delete;
		Hooking(Hooking&&) = delete;
		Hooking& operator=(Hooking const&) = delete;
		Hooking& operator=(Hooking&&) = delete;

		void Hook();
		void Unhook();

		static void ModelSpawnBypass(bool enable);
		static void AddOwnedExplosionBypass(bool enable);

		friend struct Hooks;
	private:

		void* m_OriginalGetLabelText{};
		void* m_OriginalTaskJumpConstructor{};
		void* m_OriginalFallTaskConstructor{};
		void* m_OriginalWndProc{};
		void* m_OriginalQueueDependency{};
		VMTHook m_D3DHook;
	};

	inline std::unique_ptr<Hooking> g_Hooking;
}
