#pragma once

namespace Chim
{
	class Settings
	{
	public:
		std::atomic_bool m_LockMouse = false;
	};

	inline Settings g_Settings;
}
