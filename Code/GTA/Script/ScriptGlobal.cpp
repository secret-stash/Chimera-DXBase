#include "../../Game/Game.hpp"
#include "ScriptGlobal.hpp"

namespace Chim
{
    ScriptGlobal::ScriptGlobal(std::size_t index)
        : m_Index(index)
    {

    }

    ScriptGlobal ScriptGlobal::Add(std::ptrdiff_t offset)
    {
        return m_Index + offset;
    }

    ScriptGlobal ScriptGlobal::At(std::ptrdiff_t idx, std::size_t size)
    {
        return m_Index + 1 + (idx * size);
    }

    void* ScriptGlobal::Address() const
    {
        return g_GameVariables->m_GlobalBase[m_Index >> 0x12 & 0x3F] + (m_Index & 0x3FFFF);
    }
}