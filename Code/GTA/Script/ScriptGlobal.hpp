#pragma once
#include <cstddef>

namespace Chim
{
    class ScriptGlobal
    {
    public:
        /**
         * @brief Construct a new Script Global object
         *
         * @param idx
         */
        ScriptGlobal(std::size_t idx);

        /**
         * @brief Add an offset to a global
         *
         * @param offset Offset value
         * @return ScriptGlobal
         */
        ScriptGlobal Add(std::ptrdiff_t offset);

        /**
         * @brief Get an idx in an array of a certain struct size
         *
         * @param idx Idx to access
         * @param size Size of the struct in question
         * @return ScriptGlobal
         */
        ScriptGlobal At(std::ptrdiff_t idx, std::size_t size);

        /**
         * @brief Get the address of the global in question
         *
         * @return void*
         */
        void* Address() const;

        /**
         * @brief Cast the Script Global to any type
         *
         * @tparam T The object to cast the pointer to
         * @return T* The cast object pointer
         */
        template<typename T>
        T As() const
        {
            return reinterpret_cast<T>(Address());
        }

    private:
        std::size_t m_Index;

    };
}