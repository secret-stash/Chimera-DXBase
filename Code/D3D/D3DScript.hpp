#pragma once

namespace Chim
{
	class D3DScript
	{
	public:
		explicit D3DScript() = default;
		~D3DScript() noexcept = default;

		void Tick();
	};

	inline std::shared_ptr<D3DScript> g_D3DScript;
}