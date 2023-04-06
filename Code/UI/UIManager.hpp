#pragma once
#include "../Common.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "AbstractSubmenu.hpp"
#include "../GTA/Invoker/Types.hpp"

namespace Chim::UserInterface
{
	class UIManager
	{
	public:
		explicit UIManager() = default;
		~UIManager() noexcept = default;
		UIManager(UIManager const&) = delete;
		UIManager(UIManager&&) = delete;
		UIManager& operator=(UIManager const&) = delete;
		UIManager& operator=(UIManager&&) = delete;
	
		template <typename SubmenuType, typename ...TArgs>
		void AddSubmenu(TArgs&&... args)
		{
			auto sub = std::make_unique<SubmenuType>(std::forward<TArgs>(args)...);
			if (m_SubmenuStack.empty())
			{
				m_SubmenuStack.push(sub.get());
			}

			m_AllSubmenus.push_back(std::move(sub));
		}

		void SwitchToSubmenu(std::uint32_t id)
		{
			for (auto&& sub : m_AllSubmenus)
			{
				if (sub->GetId() == id)
				{
					m_SubmenuStack.push(sub.get());
					return;
				}
			}
		}

		void DXTick();
		void GameTick();
	public:
		std::mutex m_Mutex;

		bool m_Opened = false;
		bool m_MouseLocked = false;
		float m_PosX = 25.f;
		float m_PosY = 25.f;
		float m_Width = 480.f;
		std::size_t m_OptionsPerPage = 11;
		bool m_Sounds = true;
		bool m_BoolOption = false;

		// Offsets
		float m_PaddingX = 7.f;
		float m_PaddingY = 11.f;
		
		// Input
		std::int32_t m_OpenDelay = 200;
		std::int32_t m_BackDelay = 300;
		std::int32_t m_EnterDelay = 300;
		std::int32_t m_VerticalDelay = 120;
		std::int32_t m_HorizontalDelay = 120;

		// Header
		float m_HeaderHeight = 100.f;
		Color m_HeaderBackgroundColor{ 255, 255, 255, 255 };

		// Submenu bar
		float m_SubmenuBarHeight = 45.f;
		Color m_SubmenuBarBackgroundColor{ 24, 24, 24, 255 };
		Color m_SubmenuBarTextColor{ 153, 153, 155, 255 };

		// Options
		float m_OptionHeight = 45.f;
		float m_SubmenuRectWidth = 10.f;
		Color m_ToggleOnColor{ 124, 201, 252, 255 };
		Color m_ToggleOffColor{ 46, 46, 46, 255 };
		Color m_SubmenuRectColor{ 94, 94, 94, 180 };
		Color m_OptionSelectedTextColor{ 10, 10, 10, 255 };
		Color m_OptionUnselectedTextColor{ 153, 153, 155, 255 };
		Color m_OptionSelectedBackgroundColor{ 178, 178, 178, 255 };
		Color m_OptionUnselectedBackgroundColor{ 0, 0, 0, 160 };

		// Smooth Scrolling
		float lerp(float a, float b, float t)
		{
			return a + t * (b - a);
		}

		float m_CurrentCoord = m_PosY;

		// Footer
		float m_FooterHeight = 45.f;
		Color m_FooterBackgroundColor{ 24, 24, 24, 255 };

		void CheckForInput();
		void HandleInput();
	private:
		bool m_OpenKeyPressed = false;
		bool m_BackKeyPressed = false;
		bool m_EnterKeyPressed = false;
		bool m_UpKeyPressed = false;
		bool m_DownKeyPressed = false;
		bool m_LeftKeyPressed = false;
		bool m_RightKeyPressed = false;
		void ResetInput();

		float m_DrawBaseY{};
		void DrawHeader();
		void DrawSubmenuBar(AbstractSubmenu* sub);
		void DrawOption(AbstractOption* opt, bool selected);
		void DrawFooter();

		void DrawRect(float x, float y, float width, float height, Color color, ImDrawList* draw_list = ImGui::GetBackgroundDrawList());
		void DrawSprite(ID3D11ShaderResourceView* image, float x, float y, float width, float height, Color color, ImDrawList* drawlist = ImGui::GetBackgroundDrawList());
		void DrawLeftText(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list = ImGui::GetForegroundDrawList());
		void DrawCenteredText(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list = ImGui::GetForegroundDrawList());
		void DrawRightText(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list = ImGui::GetForegroundDrawList());
		Vector2 GetSpriteScale(float size);

		// Helpers
		ImRect GetRect(ImVec2 pos, ImVec2 size);
		void PlaySound(const char* name);

		std::vector<std::unique_ptr<AbstractSubmenu>> m_AllSubmenus;
		std::stack<AbstractSubmenu*, std::vector<AbstractSubmenu*>> m_SubmenuStack;
	};
}

namespace Chim
{
	inline std::unique_ptr<UserInterface::UIManager> g_UiManager;
}
