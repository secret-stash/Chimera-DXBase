#pragma once
#include "BaseSubmenu.hpp"

namespace Chim::UserInterface
{
	class RegularSubmenu : public BaseSubmenu<RegularSubmenu>
	{
	public:
		explicit RegularSubmenu() = default;
		~RegularSubmenu() noexcept = default;
		explicit RegularSubmenu(const char* name, std::uint32_t id, std::function<void(RegularSubmenu*)> action)
		{
			SetName(name);
			SetId(id);
			SetAction(std::move(action));
		}

		RegularSubmenu(RegularSubmenu const&) = default;
		RegularSubmenu& operator=(RegularSubmenu const&) = default;
		RegularSubmenu(RegularSubmenu&&) = default;
		RegularSubmenu& operator=(RegularSubmenu&&) = default;
	};
}
