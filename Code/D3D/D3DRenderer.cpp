#include "D3DRenderer.hpp"
#include "../Game/Game.hpp"
#include "../UI/UIManager.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Chim
{
	D3DRenderer::D3DRenderer():
		m_Swapchain(g_GameVariables->m_Swapchain)
	{
		if (FAILED(m_Swapchain->GetDevice(__uuidof(ID3D11Device), (void**)(&m_Device))))
			g_Logger->Error("Failed to get D3D Device");
		m_Device->GetImmediateContext(&m_Context);

		ImGui::CreateContext();
		ImGui_ImplDX11_Init(m_Device, m_Context);
		ImGui_ImplWin32_Init(g_GameVariables->m_GameWindow);

		ImGui::StyleColorsClassic();
		auto& style = ImGui::GetStyle();
		style.WindowRounding = 10.f;
		style.PopupRounding = 10.f;
		style.FrameRounding = 10.f;
		style.ChildBorderSize = 0.f;
		style.FrameBorderSize = 0.f;
		style.PopupBorderSize = 0.f;
		style.TabBorderSize = 0.f;
		style.WindowBorderSize = 0.f;

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr; // Crucial for not leaving the ImGui.ini file

		m_FontConfig.FontDataOwnedByAtlas = false;
		m_BaseArial = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ArialBD.ttf", 22.f, &m_FontConfig);

		LoadTextureFromFile(std::format("{}\\Documents\\{}\\Textures\\Header.png", std::getenv("USERPROFILE"), CHIMERA_NAME).c_str(), m_Device, &m_Header, &m_HeaderSize.x, &m_HeaderSize.y);
		LoadTextureFromFile(std::format("{}\\Documents\\{}\\Textures\\Toggle.png", std::getenv("USERPROFILE"), CHIMERA_NAME).c_str(), m_Device, &m_Toggle, &m_ToggleSize.x, &m_ToggleSize.y);
	}

	D3DRenderer::~D3DRenderer() noexcept
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void D3DRenderer::BeginFrame()
	{
		if (g_UiManager->m_Opened && g_Settings.m_LockMouse)
		{
			ImGui::GetIO().MouseDrawCursor = true;
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
		else
		{
			ImGui::GetIO().MouseDrawCursor = false;
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void D3DRenderer::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void D3DRenderer::PreResize()
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();
	}

	void D3DRenderer::PostResize()
	{
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	void D3DRenderer::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (g_UiManager->m_Opened && msg == WM_KEYUP && wParam == 'X')
			g_Settings.m_LockMouse = !g_Settings.m_LockMouse;

		g_UiManager->CheckForInput();
		g_UiManager->HandleInput();

		if (ImGui::GetCurrentContext())
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}

	bool D3DRenderer::LoadTextureFromFile(const char* filename, ID3D11Device* d3dDevice, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
	{
		// Load from disk into a raw RGBA buffer
		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
		if (image_data == NULL)
			return false;

		// Create texture
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = image_width;
		desc.Height = image_height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D* pTexture = NULL;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = image_data;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		d3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		d3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);

		pTexture->Release();

		*out_width = image_width;
		*out_height = image_height;
		stbi_image_free(image_data);
		return true;
	}
}
