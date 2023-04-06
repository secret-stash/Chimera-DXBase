#include "../Game/Game.hpp"
#include "D3DScript.hpp"
#include "../UI/UIManager.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"

namespace Chim
{
	void D3DScript::Tick()
	{
		g_UiManager->DXTick();
	}
}
