#pragma once
#include "../Common.hpp"
#include "AbstractOption.hpp"

namespace Chim::UserInterface
{
	class AbstractSubmenu
	{
	public:
		virtual ~AbstractSubmenu() noexcept = default;

		virtual const char* GetName() = 0;
		virtual std::uint32_t GetId() = 0;
		virtual void Execute() = 0;
		virtual void Reset() = 0;

		virtual AbstractOption* GetOption(std::size_t i) = 0;
		virtual std::size_t GetNumOptions() = 0;

		virtual std::size_t GetSelectedOption() = 0;
		virtual void SetSelectedOption(std::size_t) = 0;

		void ScrollForward()
		{
			if (GetSelectedOption() < GetNumOptions() - 1)
				SetSelectedOption(GetSelectedOption() + 1);
			else
				SetSelectedOption(0);
		}

		void ScrollBackward()
		{
			if (GetSelectedOption() > 0)
				SetSelectedOption(GetSelectedOption() - 1);
			else
				SetSelectedOption(GetNumOptions() - 1);
		}
	protected:
		explicit AbstractSubmenu() = default;
		AbstractSubmenu(AbstractSubmenu const&) = default;
		AbstractSubmenu& operator=(AbstractSubmenu const&) = default;
		AbstractSubmenu(AbstractSubmenu&&) = default;
		AbstractSubmenu& operator=(AbstractSubmenu&&) = default;
	};
}
