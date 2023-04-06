#pragma once
#include "BaseOption.hpp"
#include "UIManager.hpp"

namespace Chim::UserInterface
{
	class SubOption : public BaseOption<SubOption>
	{
	public:
		explicit SubOption() = default;
		explicit SubOption(const char* text, const char* description, std::uint32_t subId, std::function<void()> action = [] {}) :
			m_SubId(subId)
		{
			SetLeftText(text);
			if (description)
				SetDescription(description);
			SetAction(std::move(action));
		}

		~SubOption() noexcept = default;
		SubOption(SubOption const&) = default;
		SubOption& operator=(SubOption const&) = default;
		SubOption(SubOption&&) = default;
		SubOption& operator=(SubOption&&) = default;

		void HandleAction(OptionAction action) override
		{
			if (action == OptionAction::EnterPress)
			{
				g_UiManager->SwitchToSubmenu(m_SubId);
			}

			BaseOption::HandleAction(action);
		}

		bool GetFlag(OptionFlag flag) override
		{
			if (flag == OptionFlag::Enterable)
			{
				return true;
			}

			return BaseOption::GetFlag(flag);
		}
	private:
		std::uint32_t m_SubId{};
	};
}
