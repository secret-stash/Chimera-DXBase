#pragma once
#include "BaseOption.hpp"

namespace Chim::UserInterface
{
	class RegularOption : public BaseOption<RegularOption>
	{
	public:
		explicit RegularOption() = default;
		explicit RegularOption(const char* text, const char* description = nullptr, std::function<void()> action = [] {})
		{
			SetLeftText(text);
			if (description)
				SetDescription(description);
			SetAction(std::move(action));
		}

		~RegularOption() noexcept = default;
		RegularOption(RegularOption const&) = default;
		RegularOption& operator=(RegularOption const&) = default;
		RegularOption(RegularOption&&) = default;
		RegularOption& operator=(RegularOption&&) = default;
	};
}
