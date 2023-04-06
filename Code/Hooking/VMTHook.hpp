#pragma once

namespace Chim
{
	class VMTHook
	{
	public:
		/**
		 * \brief Initializes the hook class
		 * \param object The object to be hooked
		 * \param numFuncs The number of functions in the virtual table
		 */
		explicit VMTHook(void* object, std::size_t numFuncs) noexcept:
			m_Object(reinterpret_cast<std::uintptr_t**>(object)),
			m_NumFuncs(numFuncs),
			m_Original(*m_Object),
			m_New(std::make_unique<std::uintptr_t[]>(m_NumFuncs + 1))
		{
			std::copy_n(m_Original - 1, m_NumFuncs + 1, m_New.get());
		}

		~VMTHook() noexcept = default;

		/**
		 * \brief Enables the hook
		 */
		void Enable() noexcept
		{
			*m_Object = &m_New[1];
		}

		/**
		 * \brief Disables the hook
		 */
		void Disable() noexcept
		{
			*m_Object = m_Original;
		}

		/**
		 * \brief Hooks a function
		 * \param replacement The replacement function pointer
		 * \param index The index of the function in the virtual table
		 */
		void Hook(void* replacement, std::size_t index) noexcept
		{
			m_New[index + 1] = reinterpret_cast<std::uintptr_t>(replacement);
		}

		/**
		 * \brief Unhooks a function
		 * \param index The index of the function in the virtual table
		 */
		void Unhook(std::size_t index) noexcept
		{
			m_New[index + 1] = m_Original[index];
		}

		/**
		 * \brief Gets an original function
		 * \param index The index of the function in the virtual table
		 */
		template <typename T>
		T GetOriginal(std::size_t index) noexcept
		{
			return reinterpret_cast<T>(m_Original[index]);
		}

		VMTHook(VMTHook const&) noexcept = delete;
		VMTHook(VMTHook&&) noexcept = delete;
		VMTHook& operator=(VMTHook const&) noexcept = delete;
		VMTHook& operator=(VMTHook&&) noexcept = delete;
	private:
		std::uintptr_t** m_Object;
		std::size_t m_NumFuncs;

		std::uintptr_t* m_Original;
		std::unique_ptr<std::uintptr_t[]> m_New;
	};
}
