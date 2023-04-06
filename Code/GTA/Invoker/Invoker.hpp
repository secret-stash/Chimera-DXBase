#pragma once
#include "CrossMap.hpp"

namespace Chim
{
    class Invoker
    {
    private:
        Invoker() = default;

    public:
        ~Invoker() = default;

        static void CacheHandlers()
        {
            GetInstance().CacheHandlersImpl();
        }
        static bool HandlersCached()
        {
            return GetInstance().m_HandlersCached;
        }

        static void BeginCall()
        {
            GetInstance().BeginCallImpl();
        }

        template<typename Args>
        static void PushArg(Args&& arg)
        {
            GetInstance().m_CallContext.push_arg(std::forward<Args>(arg));
        }

        static void EndCall(rage::scrNativeHash hash)
        {
            GetInstance().EndCallImpl(hash);
        }

        template<typename T>
        static T& GetReturnValue()
        {
            return *GetInstance().m_CallContext.get_return_value<T>();
        }

    private:
        void CacheHandlersImpl();

        void BeginCallImpl();
        void EndCallImpl(rage::scrNativeHash hash);

        static Invoker& GetInstance()
        {
            static Invoker i{};

            return i;
        }

    private:
        class NativeCallContext : public rage::scrNativeCallContext
        {
        public:
            NativeCallContext();

        private:
            std::uint64_t m_ReturnStack[10];
            std::uint64_t m_ArgStack[100];

        };

    private:
        NativeCallContext m_CallContext;
        std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> m_HandlerCache;
        bool m_HandlersCached = false;

    };
}

template<typename ReturnType, typename ...Args>
inline ReturnType Invoke(rage::scrNativeHash hash, Args&& ...args)
{
    using namespace Chim;

    Invoker::BeginCall();
    (Invoker::PushArg(std::forward<Args>(args)), ...);
    Invoker::EndCall(hash);

    if constexpr (!std::is_void_v<ReturnType>)
    {
        return Invoker::GetReturnValue<ReturnType>();
    }
}