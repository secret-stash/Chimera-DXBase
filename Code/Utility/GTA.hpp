#pragma once 
#include "../Common.hpp"
#include "../Game/Game.hpp"

namespace Chim::GTAUtility
{
	inline CPed* GetLocalPed()
	{
		if (const auto PedFactory = *g_GameVariables->m_PedFactory)
		{
			return PedFactory->m_local_ped;
		}

		return nullptr;
	}

	inline CPlayerInfo* GetLocalPlayerInfo()
	{
		if (const auto PedFactory = *g_GameVariables->m_PedFactory)
		{
			if (const auto Ped = PedFactory->m_local_ped)
			{
				return Ped->m_player_info;
			}
		}

		return nullptr;
	}

	namespace Offsets
	{
		inline std::uintptr_t GetMultilayerPointer(std::uintptr_t base_address, std::vector<DWORD> offsets)
		{
			std::uintptr_t Pointer = *(std::uintptr_t*)(base_address);
			if (!Pointer)
				return NULL;
			auto Level = offsets.size();

			for (auto i = 0; i < Level; i++)
			{
				if (i == Level - 1)
				{
					Pointer += offsets[i];
					if (!Pointer)
						return NULL;
					return Pointer;
				}
				else
				{
					Pointer = *(std::uint64_t*)(Pointer + offsets[i]);
					if (!Pointer)
						return NULL;
				}
			}

			return Pointer;
		}

		template <typename T>
		inline T GetValue(std::vector<DWORD> offsets)
		{
			std::uintptr_t Address = GetMultilayerPointer(std::uintptr_t(g_GameVariables->m_PedFactory), offsets);
			if (Address == NULL)
				return NULL;

			return *((T*)Address);
		}

		template <typename T>
		inline void SetValue(std::vector<DWORD> offsets, T value)
		{
			std::uintptr_t Address = GetMultilayerPointer(std::uintptr_t(g_GameVariables->m_PedFactory), offsets);
			if (Address == NULL)
				return;

			*reinterpret_cast<T*>(Address) = value;
		}
	}

	namespace Bits
	{
		template<typename T>
		bool HasBitsSet(T* address, T bits)
		{
			return (*address & bits) == bits;
		}

		template<typename T>
		inline constexpr void SetBit(T* value, T bit)
		{
			*value |= (1 << bit);
		}

		template<typename T, typename ...T2>
		inline constexpr void SetBits(T* value, T2... bit)
		{
			(SetBit<T>(value, std::forward<T>(bit)), ...);
		}

		template<typename T>
		inline constexpr void ClearBit(T* value, T bit)
		{
			*value &= ~(1 << bit);
		}

		template<typename T, typename ...T2>
		inline constexpr void ClearBits(T* value, T2... bit)
		{
			(ClearBits<T>(value, std::forward<T>(bit)), ...);
		}
	}
}