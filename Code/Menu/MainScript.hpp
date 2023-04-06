#pragma once

namespace Chim
{
	class MainScript
	{
	public:
		explicit MainScript() = default;
		~MainScript() noexcept = default;

		void Initialize();
		static void Tick();
	};

	inline std::shared_ptr<MainScript> g_MainScript;
}