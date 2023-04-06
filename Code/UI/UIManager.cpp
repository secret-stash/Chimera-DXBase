#include "UIManager.hpp"
#include "../Utility/Timer.hpp"
#include "../D3D/D3DRenderer.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptFiber.hpp"

namespace Chim::UserInterface
{
	void UIManager::DXTick()
	{
		std::lock_guard lock(m_Mutex);

		if (m_Opened)
		{
			m_DrawBaseY = m_PosY;
			DrawHeader();
			if (!m_SubmenuStack.empty())
			{
				auto sub = m_SubmenuStack.top();
				sub->Reset();
				sub->Execute();

				DrawSubmenuBar(sub);
				if (sub->GetNumOptions() != 0)
				{
					DrawRect(
						m_PosX,
						m_PosY + m_HeaderHeight + (m_SubmenuBarHeight * 1.5f),
						m_Width,
						m_OptionHeight * (sub->GetNumOptions() > m_OptionsPerPage ? m_OptionsPerPage : sub->GetNumOptions()),
						m_OptionUnselectedBackgroundColor);

					std::size_t startPoint = 0;
					std::size_t endPoint = sub->GetNumOptions() > m_OptionsPerPage ? m_OptionsPerPage : sub->GetNumOptions();
					if (sub->GetNumOptions() > m_OptionsPerPage && sub->GetSelectedOption() >= m_OptionsPerPage)
					{
						startPoint = sub->GetSelectedOption() - m_OptionsPerPage + 1;
						endPoint = sub->GetSelectedOption() + 1;
					}

					for (std::size_t i = startPoint, j = 0; i < endPoint; ++i, ++j)
					{
						if (i == sub->GetSelectedOption())
						{
							m_CurrentCoord = lerp(m_CurrentCoord, m_DrawBaseY + (m_OptionHeight / 2.f), 0.2f);
							DrawRect(
								m_PosX,
								m_CurrentCoord,
								m_Width,
								m_OptionHeight,
								m_OptionSelectedBackgroundColor);
						}

						DrawOption(sub->GetOption(i), i == sub->GetSelectedOption());
					}
				}
			}

			DrawFooter();
		}
	}

	void UIManager::GameTick()
	{
		if (m_Opened)
			PAD::DISABLE_CONTROL_ACTION(0, 27, true); // Disable phone

		if (m_Opened && g_Settings.m_LockMouse)
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0); // Disable Everything
	}

	void UIManager::CheckForInput()
	{
		ResetInput();

		m_OpenKeyPressed = IsKeyPressed(VK_INSERT);
		m_BackKeyPressed = IsKeyPressed(VK_BACK);
		m_EnterKeyPressed = IsKeyPressed(VK_RETURN);
		m_UpKeyPressed = IsKeyPressed(VK_UP);
		m_DownKeyPressed = IsKeyPressed(VK_DOWN);
		m_LeftKeyPressed = IsKeyPressed(VK_LEFT);
		m_RightKeyPressed = IsKeyPressed(VK_RIGHT);
	}

	void UIManager::HandleInput()
	{
		static Timer openTimer(0ms);
		openTimer.SetDelay(std::chrono::milliseconds(m_OpenDelay));
		if (m_OpenKeyPressed && openTimer.Update())
		{
			m_Opened ^= true;

			if (m_Sounds)
				PlaySound(m_Opened ? "SELECT" : "BACK");
		}

		static Timer backTimer(0ms);
		backTimer.SetDelay(std::chrono::milliseconds(m_BackDelay));
		if (m_Opened && m_BackKeyPressed && backTimer.Update())
		{
			if (m_Sounds)
				PlaySound("BACK");

			if (m_SubmenuStack.size() <= 1)
				m_Opened = false;
			else
				m_SubmenuStack.pop();
		}
		
		if (m_Opened && !m_SubmenuStack.empty())
		{
			auto sub = m_SubmenuStack.top();

			static Timer enterTimer(0ms);
			enterTimer.SetDelay(std::chrono::milliseconds(m_EnterDelay));
			if (m_EnterKeyPressed && sub->GetNumOptions() != 0 && enterTimer.Update())
			{
				if (m_Sounds)
					PlaySound("SELECT");

				if (const auto opt = sub->GetOption(sub->GetSelectedOption()))
					opt->HandleAction(OptionAction::EnterPress);
			}

			static Timer upTimer(0ms);
			upTimer.SetDelay(std::chrono::milliseconds(m_VerticalDelay));
			if (m_UpKeyPressed && sub->GetNumOptions() != 0 && upTimer.Update())
			{
				if (m_Sounds)
					PlaySound("NAV_UP_DOWN");

				sub->ScrollBackward();
			}

			static Timer downTimer(0ms);
			downTimer.SetDelay(std::chrono::milliseconds(m_VerticalDelay));
			if (m_DownKeyPressed && sub->GetNumOptions() != 0 && downTimer.Update())
			{
				if (m_Sounds)
					PlaySound("NAV_UP_DOWN");

				sub->ScrollForward();
			}

			static Timer leftTimer(0ms);
			leftTimer.SetDelay(std::chrono::milliseconds(m_HorizontalDelay));
			if (m_LeftKeyPressed && sub->GetNumOptions() != 0 && leftTimer.Update())
			{
				if (m_Sounds)
					PlaySound("NAV_LEFT_RIGHT");

				if (const auto opt = sub->GetOption(sub->GetSelectedOption()))
					opt->HandleAction(OptionAction::LeftPress);
			}

			static Timer rightTimer(0ms);
			rightTimer.SetDelay(std::chrono::milliseconds(m_HorizontalDelay));
			if (m_RightKeyPressed && sub->GetNumOptions() != 0 && rightTimer.Update())
			{
				if (m_Sounds)
					PlaySound("NAV_LEFT_RIGHT");

				if (const auto opt = sub->GetOption(sub->GetSelectedOption()))
					opt->HandleAction(OptionAction::RightPress);
			}
		}
	}

	void UIManager::ResetInput()
	{
		m_OpenKeyPressed = false;
		m_BackKeyPressed = false;
		m_EnterKeyPressed = false;
		m_UpKeyPressed = false;
		m_DownKeyPressed = false;
		m_LeftKeyPressed = false;
		m_RightKeyPressed = false;
	}

	void UIManager::DrawHeader()
	{
		DrawSprite(
			g_D3DRenderer->m_Header,
			m_PosX,
			m_DrawBaseY + (m_SubmenuBarHeight / 2.f),
			m_Width,
			m_HeaderHeight,
			m_HeaderBackgroundColor);

		m_DrawBaseY += m_HeaderHeight;
	}

	void UIManager::DrawSubmenuBar(AbstractSubmenu* sub)
	{
		char leftText[64] = {};
		std::strncpy(&leftText[0], sub->GetName(), sizeof(leftText) - 1);
		std::transform(std::begin(leftText), std::end(leftText), std::begin(leftText), [](char c) { return static_cast<char>(toupper(c)); });

		char centerText[64] = {};
		std::strncpy(&centerText[0], CHIMERA_NAME, sizeof(centerText) - 1);
		std::transform(std::begin(centerText), std::end(centerText), std::begin(centerText), [](char c) { return static_cast<char>(toupper(c)); });

		char rightText[32] = {};
		std::snprintf(rightText, sizeof(rightText) - 1, "%zu / %zu", sub->GetSelectedOption() + 1, sub->GetNumOptions());

		DrawRect(
			m_PosX,
			m_DrawBaseY + (m_SubmenuBarHeight / 2.f),
			m_Width, m_SubmenuBarHeight,
			m_SubmenuBarBackgroundColor);
		DrawLeftText(
			&leftText[0],
			m_PosX + m_PaddingX,
			m_DrawBaseY + (m_SubmenuBarHeight / 2.f) + m_PaddingY,
			m_SubmenuBarTextColor,
			g_D3DRenderer->m_BaseArial);
		DrawCenteredText(
			&centerText[0],
			m_PosX + (m_Width / 2.f),
			m_DrawBaseY + (m_SubmenuBarHeight / 2.f) + m_PaddingY,
			m_SubmenuBarTextColor,
			g_D3DRenderer->m_BaseArial);
		DrawRightText(
			&rightText[0],
			m_PosX + (m_Width - m_PaddingX),
			m_DrawBaseY + (m_SubmenuBarHeight / 2.f) + m_PaddingY,
			m_SubmenuBarTextColor,
			g_D3DRenderer->m_BaseArial);

		m_DrawBaseY += m_SubmenuBarHeight;
	}

	void UIManager::DrawOption(AbstractOption* opt, bool selected)
	{
		DrawLeftText(
			opt->GetLeftText(),
			m_PosX + m_PaddingX,
			m_DrawBaseY + (m_OptionHeight / 2.f) + m_PaddingY,
			selected ? m_OptionSelectedTextColor : m_OptionUnselectedTextColor,
			g_D3DRenderer->m_BaseArial);
		DrawRightText(
			opt->GetRightText(),
			m_PosX + (m_Width - m_PaddingX),
			m_DrawBaseY + (m_OptionHeight / 2.f) + m_PaddingY,
			selected ? m_OptionSelectedTextColor : m_OptionUnselectedTextColor,
			g_D3DRenderer->m_BaseArial);

		if (opt->GetFlag(OptionFlag::Enterable))
		{
			DrawRect(
				m_PosX + (m_Width - m_SubmenuRectWidth),
				m_DrawBaseY + (m_OptionHeight / 2.f),
				m_SubmenuRectWidth,
				m_OptionHeight,
				m_SubmenuRectColor,
				ImGui::GetForegroundDrawList());
		}

		if (opt->GetFlag(OptionFlag::Toggle))
		{
			DrawSprite(
				g_D3DRenderer->m_Toggle,
				m_PosX + (m_Width - (m_PaddingX * 5.f)),
				m_DrawBaseY + (m_OptionHeight / 2.f) + (m_PaddingY / 2.f),
				32.f,
				32.f,
				m_BoolOption ? m_ToggleOnColor : m_ToggleOffColor);
		}

		m_DrawBaseY += m_OptionHeight;
	}

	void UIManager::DrawFooter()
	{
		DrawRect(
			m_PosX,
			m_DrawBaseY + (m_FooterHeight / 2.f),
			m_Width,
			m_FooterHeight,
			m_FooterBackgroundColor);

		m_DrawBaseY += m_FooterHeight;
	}

	void UIManager::DrawRect(float x, float y, float width, float height, Color color, ImDrawList* draw_list)
	{
		const auto Position = ImVec2(x, y);
		const auto Size = ImVec2(width, height);
		const auto Rect = GetRect(Position, Size);

		draw_list->AddRectFilled(Rect.Max, Rect.Min, IM_COL32(color.r, color.g, color.b, color.a));
	}

	void UIManager::DrawSprite(ID3D11ShaderResourceView* image, float x, float y, float width, float height, Color color, ImDrawList* drawlist)
	{
		const auto Position = ImVec2(x, y);
		const auto Size = ImVec2(width, height);
		const auto Rect = GetRect(Position, Size);

		drawlist->AddImage((void*)image,
			Rect.Min,
			Rect.Max,
			ImVec2(0, 0), ImVec2(1, 1),
			IM_COL32(color.r, color.g, color.b, color.a)
		);
	}

	void UIManager::DrawLeftText(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list)
	{
		const auto Position = ImVec2(x, y);
		ImGui::PushFont(font);
		draw_list->AddText(Position, IM_COL32(color.r, color.g, color.b, color.a), text);
		ImGui::PopFont();
	}

	void UIManager::DrawCenteredText(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list)
	{
		const auto Position = ImVec2((x - (ImGui::CalcTextSize(text).x / 2.f)), y);
		ImGui::PushFont(font);
		draw_list->AddText(Position, IM_COL32(color.r, color.g, color.b, color.a), text);
		ImGui::PopFont();
	}

	void UIManager::DrawRightText(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list)
	{
		const auto Position = ImVec2((x - ImGui::CalcTextSize(text).x), y);
		ImGui::PushFont(font);
		draw_list->AddText(Position, IM_COL32(color.r, color.g, color.b, color.a), text);
		ImGui::PopFont();
	}

	Vector2 UIManager::GetSpriteScale(float size)
	{
		int x;
		int y;
		GRAPHICS::GET_ACTUAL_SCREEN_RESOLUTION(&x, &y);

		return { (static_cast<float>(y) / static_cast<float>(x)) * size, size };
	}

	ImRect UIManager::GetRect(ImVec2 pos, ImVec2 size)
	{
		const auto ItemSize = ImGui::CalcItemSize(ImVec2(size.x, size.y), 0.0f, 0.0f);
		return ImRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + ItemSize.x, pos.y + ItemSize.y));
	}

	void UIManager::PlaySound(const char* name)
	{
		g_ScriptFiber->Queue([=]
		{
			AUDIO::PLAY_SOUND_FRONTEND(-1, name, "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
		});
	}
}
