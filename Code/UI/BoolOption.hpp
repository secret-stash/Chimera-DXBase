#pragma once
#include "BaseOption.hpp"
#include "UIManager.hpp"

namespace Chim::UserInterface
{
	template <typename BoolType = bool>
	class BoolOption : public BaseOption<BoolOption<BoolType>>
	{
	public:
		explicit BoolOption(const char* text, const char* description, BoolType* b00l, std::function<void()> action = [] {}) :
			m_Bool(b00l)
		{
			Base::SetLeftText(text);
			if (description)
				Base::SetDescription(description);
			Base::SetAction(std::move(action));
		}

		~BoolOption() noexcept = default;
		BoolOption(BoolOption const&) = default;
		BoolOption& operator=(BoolOption const&) = default;
		BoolOption(BoolOption&&) = default;
		BoolOption& operator=(BoolOption&&) = default;

		void HandleAction(OptionAction action) override
		{
			if (action == OptionAction::EnterPress)
			{
				*m_Bool = !*m_Bool;
			}

			Base::HandleAction(action);
		}

		bool GetFlag(OptionFlag flag) override
		{
			if (flag == OptionFlag::Toggle)
			{
				g_UiManager->m_BoolOption = *m_Bool;
				return true;
			}

			return Base::GetFlag(flag);
		}
	private:
		BoolType* m_Bool;
		using Base = BaseOption<BoolOption<BoolType>>;
	};
}
