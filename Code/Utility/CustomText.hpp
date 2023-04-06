#pragma once

namespace Chim
{
	class CustomText
	{
	public:
		explicit CustomText() = default;
		~CustomText() noexcept = default;

		CustomText(CustomText const&) = delete;
		CustomText(CustomText&&) = delete;
		CustomText& operator=(CustomText const&) = delete;
		CustomText& operator=(CustomText&&) = delete;

		/**
		 * \brief Registers custom text
		 * \param hash The hash of the label to register
		 * \param text The content of the custom text
		 */
		void AddText(std::uint32_t hash, const char* text);

		/**
		 * \brief Unregisters custom text
		 * \param hash The hash of the label to unregister
		 */
		void RemoveText(std::uint32_t hash);

		/**
		 * \brief Retrieves custom text
		 * \param hash The hash of the label
		 * \return A pointer to the string if it's registered, nullptr otherwise
		 */
		const char* GetText(std::uint32_t hash);
	private:
		std::unordered_map<std::uint32_t, std::unique_ptr<char[]>> m_TextMap;
	};

	inline std::unique_ptr<CustomText> g_CustomText;
}
