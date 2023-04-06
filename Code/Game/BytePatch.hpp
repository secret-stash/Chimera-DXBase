#pragma once
#include "../Common.hpp"
#include <span>

namespace Game
{
	template<typename T>
	concept SpanCompatibleType = requires(T a)
	{
		std::span{ a };
	};

	class BytePatch
	{
	public:
		virtual ~BytePatch();

		void Apply() const;

		void Restore() const;

		void Remove() const;

		template <typename TAddr>
		static const std::unique_ptr<BytePatch>& Make(TAddr address, std::remove_pointer_t<std::remove_reference_t<TAddr>> value)
		{
			return m_Patches.emplace_back(
				std::unique_ptr<BytePatch>(new BytePatch(address, value)));
		}

		template <typename TAddr, typename T> requires SpanCompatibleType<T>
		static const std::unique_ptr<BytePatch>& Make(TAddr address, T span_compatible)
		{
			return m_Patches.emplace_back(
				std::unique_ptr<BytePatch>(new BytePatch(address, std::span{ span_compatible })));
		}

		static void RestoreAll();

	private:
		template <typename TAddr>
		BytePatch(TAddr address, std::remove_pointer_t<std::remove_reference_t<TAddr>> value)
			: m_Address(address)
		{
			m_Size = sizeof(std::remove_pointer_t<std::remove_reference_t<TAddr>>);

			m_Original = std::make_unique<byte[]>(m_Size);
			memcpy(m_Original.get(), m_Address, m_Size);

			m_Value = std::make_unique<byte[]>(m_Size);
			memcpy(m_Value.get(), &value, m_Size);
		}

		template <typename TAddr, typename T, std::size_t N>
		BytePatch(TAddr address, std::span<T, N> span)
			: m_Address((void*)address)
		{
			m_Size = span.size();

			m_Original = std::make_unique<byte[]>(m_Size);
			memcpy(m_Original.get(), m_Address, m_Size);

			m_Value = std::make_unique<byte[]>(m_Size);
			for (int i = 0; i < m_Size; i++)
				m_Value[i] = span[i];
		}

	protected:
		static inline std::vector<std::unique_ptr<BytePatch>> m_Patches;

	private:
		void* m_Address;
		std::unique_ptr<byte[]> m_Value;
		std::unique_ptr<byte[]> m_Original;
		std::size_t m_Size;

		friend bool operator== (const std::unique_ptr<BytePatch>& a, const BytePatch* b);
	};
}