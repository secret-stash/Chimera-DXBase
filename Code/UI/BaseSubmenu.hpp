#pragma once
#include "AbstractSubmenu.hpp"
#include "AbstractOption.hpp"

namespace Chim::UserInterface
{
	template <typename T>
	class BaseSubmenu : public AbstractSubmenu
	{
	public:
		const char* GetName() override
		{
			return &m_Name[0];
		}

		std::uint32_t GetId() override
		{
			return m_Id;
		}

		void Execute() override
		{
			if (m_Action)
				std::invoke(m_Action, static_cast<T*>(this));
			if (m_SelectedOption >= m_Options.size())
				m_SelectedOption = 0;
		}

		void Reset() override
		{
			m_Options.clear();
		}

		AbstractOption* GetOption(std::size_t i) override
		{
			if (i > m_Options.size())
				return nullptr;
			return m_Options[i].get();
		}

		std::size_t GetNumOptions() override
		{
			return m_Options.size();
		}

		std::size_t GetSelectedOption() override
		{
			return m_SelectedOption;
		}

		void SetSelectedOption(std::size_t n) override
		{
			m_SelectedOption = n;
		}

		template <typename OptionType, typename ...TArgs>
		void AddOption(TArgs&&... args)
		{
			m_Options.push_back(std::make_unique<OptionType>(std::forward<TArgs>(args)...));
		}

		T& SetName(const char* name)
		{
			std::strncpy(&m_Name[0], name, sizeof(m_Name) - 1);
			return static_cast<T&>(*this);
		}

		T& SetId(std::uint32_t id)
		{
			m_Id = id;
			return static_cast<T&>(*this);
		}

		T& SetAction(std::function<void(T*)> action)
		{
			m_Action = std::move(action);
			return static_cast<T&>(*this);
		}
	protected:
		explicit BaseSubmenu() = default;
		~BaseSubmenu() noexcept = default;

		BaseSubmenu(BaseSubmenu const&) = default;
		BaseSubmenu& operator=(BaseSubmenu const&) = default;
		BaseSubmenu(BaseSubmenu&&) = default;
		BaseSubmenu& operator=(BaseSubmenu&&) = default;

		char m_Name[64] = {};
		std::uint32_t m_Id{};
		std::function<void(T*)> m_Action;

		std::vector<std::unique_ptr<AbstractOption>> m_Options;
		std::size_t m_SelectedOption{};
	};
}
