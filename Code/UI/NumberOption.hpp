#pragma once
#include "BaseOption.hpp"
#include "UIManager.hpp"

namespace Chim::UserInterface
{
	template <typename NumberType>
	class NumberOption : public BaseOption<NumberOption<NumberType>>
	{
	public:
		explicit NumberOption() = default;
		explicit NumberOption(const char* text, const char* description, NumberType* number, NumberType min, NumberType max, NumberType step = 1, std::size_t precision = 3, bool actionOnHorizontal = true, const char* prefix = "", const char* suffix = "", std::function<void()> action = [] {}):
			m_ActionOnHorizontal(actionOnHorizontal),
			m_Number(number),
			m_Min(min),
			m_Max(max),
			m_Step(step),
			m_Precision(precision)
		{
			Base::SetLeftText(text);
			if (description)
				Base::SetDescription(description);
			Base::SetAction(std::move(action));
			std::strncpy(&m_Prefix[0], prefix, sizeof(m_Prefix) - 1);
			std::strncpy(&m_Suffix[0], suffix, sizeof(m_Suffix) - 1);
		}

		~NumberOption() noexcept = default;
		NumberOption(NumberOption const&) = default;
		NumberOption& operator=(NumberOption const&) = default;
		NumberOption(NumberOption&&) = default;
		NumberOption& operator=(NumberOption&&) = default;

		const char* GetRightText() override
		{
			MemoryStringStream stream(Base::m_RightText);
			stream
				<< std::setprecision(m_Precision)
				<< std::fixed
				<< m_Prefix
				<< static_cast<DisplayType>(*m_Number)
				<< m_Suffix;
			return Base::GetRightText();
		}

		void HandleAction(OptionAction action) override
		{
			if (action == OptionAction::LeftPress)
			{
				if (*m_Number - m_Step < m_Min)
					*m_Number = m_Max;
				else
					*m_Number -= m_Step;

				if (m_ActionOnHorizontal && Base::m_Action)
					std::invoke(Base::m_Action);
			}
			else if (action == OptionAction::RightPress)
			{
				if (*m_Number + m_Step > m_Max)
					*m_Number = m_Min;
				else
					*m_Number += m_Step;

				if (m_ActionOnHorizontal && Base::m_Action)
					std::invoke(Base::m_Action);
			}

			Base::HandleAction(action);
		}
	private:
		char m_Prefix[32] = {};
		char m_Suffix[32] = {};
		bool m_ActionOnHorizontal{};

		NumberType* m_Number{};
		NumberType m_Min{};
		NumberType m_Max{};
		NumberType m_Step{};
		std::size_t m_Precision{};


		using Base = BaseOption<NumberOption<NumberType>>;
		using DisplayType = std::conditional_t<sizeof(NumberType) == 1, std::uint32_t, NumberType>;
	};
}
