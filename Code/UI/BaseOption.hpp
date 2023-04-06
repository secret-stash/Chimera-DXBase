#pragma once
#include "../Common.hpp"
#include "AbstractOption.hpp"

namespace Chim::UserInterface
{
	template <typename T>
	class BaseOption : public AbstractOption
	{
	public:
		const char* GetLeftText() override
		{
			return &m_LeftText[0];
		}

		const char* GetRightText() override
		{
			return &m_RightText[0];
		}

		const char* GetDescription() override
		{
			return &m_Description[0];
		}

		void HandleAction(OptionAction action) override
		{
			if (action == OptionAction::EnterPress)
			{
				if (m_Action)
				{
					std::invoke(m_Action);
				}
			}
		}

		bool GetFlag(OptionFlag) override
		{
			return false;
		}

		T& SetLeftText(const char* text)
		{
			std::strncpy(&m_LeftText[0], text, sizeof(m_LeftText) - 1);
			return static_cast<T&>(*this);
		}

		T& SetRightText(const char* text)
		{
			std::strncpy(&m_RightText[0], text, sizeof(m_RightText) - 1);
			return static_cast<T&>(*this);
		}

		T& SetDescription(const char* text)
		{
			if (text)
				std::strncpy(&m_Description[0], text, sizeof(m_Description) - 1);
			return static_cast<T&>(*this);
		}

		T& SetAction(std::function<void()> action)
		{
			m_Action = std::move(action);
			return static_cast<T&>(*this);
		}
	protected:
		explicit BaseOption() = default;
		~BaseOption() noexcept = default;

		BaseOption(BaseOption const&) = default;
		BaseOption& operator=(BaseOption const&) = default;
		BaseOption(BaseOption&&) = default;
		BaseOption& operator=(BaseOption&&) = default;

		char m_LeftText[64] = {};
		char m_RightText[64] = {};
		char m_Description[64] = {};
		std::function<void()> m_Action;
	};
}
