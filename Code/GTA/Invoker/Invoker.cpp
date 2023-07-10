#include "Invoker.hpp"
#include "../../Game/Game.hpp"
#include "../../Utility/Logger.hpp"

namespace Chim
{
    Invoker::NativeCallContext::NativeCallContext()
    {
        m_return_value = &m_ReturnStack[0];
        m_args = &m_ArgStack[0];
    }

    void Invoker::CacheHandlersImpl()
    {
        for (const auto [firstHash, currHash] : g_CrossMap)
        {
            auto handler = g_GameFunctions->m_GetNativeHandler(g_GameVariables->m_NativeRegistrationTable, currHash);

            m_HandlerCache.emplace(firstHash, handler);
        }

        m_HandlersCached = true;
    }

    void Invoker::BeginCallImpl()
    {
        m_CallContext.reset();
    }

    void Invoker::EndCallImpl(rage::scrNativeHash hash)
    {
        if (const auto& it = m_HandlerCache.find(hash); it != m_HandlerCache.end())
        {
            if (const auto& handler = it->second)
            {
                __try
                {
                    handler(&m_CallContext);
                    g_GameFunctions->m_FixVectors(&m_CallContext);
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    g_Logger->Error("Failed to invoke native 0x%p", hash);
                }
            }
            else
            {
                g_Logger->Error("Failed to find handler for native 0x%p", hash);
            }
        }
        else
        {
            g_Logger->Error("Failed to find current hash for native 0x%p", hash);
        }
    }
}