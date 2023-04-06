#include "Logger.hpp"
#include "Util.hpp"

namespace Chim
{
	enum eLogColor
	{
		Black = 0,
		DarkBlue = FOREGROUND_BLUE,
		DarkGreen = FOREGROUND_GREEN,
		DarkCyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
		DarkRed = FOREGROUND_RED,
		DarkMagneta = FOREGROUND_RED | FOREGROUND_BLUE,
		DarkYellow = FOREGROUND_RED | FOREGROUND_GREEN,
		DarkGray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		Gray = FOREGROUND_INTENSITY,
		Blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		Cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		Red = FOREGROUND_INTENSITY | FOREGROUND_RED,
		Magneta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		Yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		White = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
	};

	Logger::Logger()
	{
		try
		{
			m_FilePath.append(std::getenv("USERPROFILE"));
			m_FilePath.append("Documents");
			m_FilePath.append(CHIMERA_NAME);
			if (!fs::exists(m_FilePath))
				fs::create_directory(m_FilePath);
			m_FilePath.append("Cheat.log");
		}
		catch (fs::filesystem_error const&)
		{
		}

		if (!AttachConsole(GetCurrentProcessId()))
			AllocConsole();
		SetConsoleTitleW(CHIMERA_WIDE(CHIMERA_NAME));
		SetLayeredWindowAttributes(GetConsoleWindow(), NULL, 235, LWA_ALPHA);
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);

		m_Console.open("CONOUT$");
		m_Input.open("CONIN$");
		m_File.open(m_FilePath, std::ios_base::out | std::ios_base::app);

		Raw(u8R"ascii(

 ██████╗██╗  ██╗██╗███╗   ███╗███████╗██████╗  █████╗     ██████╗  █████╗ ███████╗███████╗
██╔════╝██║  ██║██║████╗ ████║██╔════╝██╔══██╗██╔══██╗    ██╔══██╗██╔══██╗██╔════╝██╔════╝
██║     ███████║██║██╔████╔██║█████╗  ██████╔╝███████║    ██████╔╝███████║███████╗█████╗  
██║     ██╔══██║██║██║╚██╔╝██║██╔══╝  ██╔══██╗██╔══██║    ██╔══██╗██╔══██║╚════██║██╔══╝  
╚██████╗██║  ██║██║██║ ╚═╝ ██║███████╗██║  ██║██║  ██║    ██████╔╝██║  ██║███████║███████╗
 ╚═════╝╚═╝  ╚═╝╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝

)ascii");
	}

	Logger::~Logger() noexcept
	{
		FreeConsole();
	}

	void Logger::Info(const char* format, ...)
	{
		std::va_list args{};
		va_start(args, format);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), eLogColor::Gray);
		Log("Info", format, args);
		va_end(args);
	}

	void Logger::Error(const char* format, ...)
	{
		std::va_list args{};
		va_start(args, format);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), eLogColor::Red);
		Log("Error", format, args);
		va_end(args);
	}

	void Logger::Raw(const char8_t* format, ...)
	{
		std::va_list args{};
		va_start(args, format);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), eLogColor::Red);
		std::lock_guard lock(m_Mutex);

		std::size_t Length = std::vsnprintf(nullptr, 0, (const char*)format, args) + 1;
		auto Buffer = std::make_unique<char[]>(Length);

		std::uninitialized_fill_n(Buffer.get(), Length, '\0');
		std::vsnprintf(Buffer.get(), Length, (const char*)format, args);

		m_Console << Buffer.get() << std::endl;
		va_end(args);
	}

	void Logger::Log(const char* type, const char* format, std::va_list args)
	{
		std::lock_guard lock(m_Mutex);

		auto time = std::time(nullptr);
		auto tm = std::localtime(&time);

		char prefix[64] = {};
		std::snprintf(prefix, sizeof(prefix) - 1, "[%02d:%02d:%02d] [%s] ", tm->tm_hour, tm->tm_min, tm->tm_sec, type);

		std::size_t messageLength = std::vsnprintf(nullptr, 0, format, args) + 1;
		auto messageBuffer = std::make_unique<char[]>(messageLength);
		
		std::uninitialized_fill_n(messageBuffer.get(), messageLength, '\0');
		std::vsnprintf(messageBuffer.get(), messageLength, format, args);

		m_File << prefix << messageBuffer.get() << std::endl;
		m_Console << prefix << messageBuffer.get() << std::endl;

		m_Messages.push_back(std::move(messageBuffer));
	}

	std::mutex& Logger::GetMutex()
	{
		return m_Mutex;
	}

	std::pair<std::unique_ptr<char[]>*, std::size_t> Logger::GetMessages()
	{
		return std::make_pair(m_Messages.data(), m_Messages.size());
	}

	std::istream& Logger::GetInput()
	{
		return m_Input;
	}
}
