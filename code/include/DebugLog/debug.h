#pragma once

#include <iostream>
#include <fstream>
#include <string>

#ifdef NDEBUG
// Only write to the log file, without printing to the console.
#define LOG_APP(_str, ...) { Debug::Log::GetInstance()->PrintApp_Log(_str, __VA_ARGS__); }

// Only write to the log file, without printing to the console.
#define LOG_CLEAN(_str, ...) { Debug::Log::GetInstance()->PrintClean(_str, __VA_ARGS__); }
#else
// Used for : MACRO DEBUG_LOG (APP logs)
#define LOG_APP(_str, ...) { std::cout << "[APP LOG] : " << Debug::Log::GetInstance()->PrintApp_Log(_str, __VA_ARGS__) << std::endl; }

// Used for : MACRO DEBUG_LOG (user logs)
#define LOG_CLEAN(_str, ...) { std::cout << Debug::Log::GetInstance()->PrintClean(_str, __VA_ARGS__) << std::endl; }
#endif

// Used for : MACRO DEBUG_LOG (user logs)
#define LOG_DEBUG(_str, ...) { std::cout << "[USER DEBUG LOG] : " << Debug::Log::GetInstance()->PrintDebug(_str, __VA_ARGS__) << std::endl; }

// Used for : MACRO THROW EXCEPTION (APP error logs)
#define LOG_THROW(_str, ...) { std::string thrownText = Debug::Log::GetInstance()->PrintApp_Throw(_str, __VA_ARGS__); std::cout << "\n\n" << __FILE__ << " (l." << __LINE__ << ") " << "[APP EXCEPTION] : \n" << thrownText << std::endl; throw std::runtime_error(thrownText); }


// Max size of a log.
#define LOG_MAX_SIZE 1024


namespace Debug
{
	enum LogType
	{
		NONE,
		USER_LOG,
		APP_LOG,
		APP_THROW
	};

    class Log
	{
    private:
		Log(std::string _fileName="log.txt");

        static Log* singleton;
		std::ofstream f;
		bool bIsCurrentlyDestroying = false;
    public:
        static Log* GetInstance();

		void OpenFile(std::string _fileName);
		std::string PrintDebug(const char* _format, ...);
		std::string PrintApp_Log(const char* _format, ...);
		std::string PrintApp_Throw(const char* _format, ...);
		std::string PrintClean(const char* _format, ...);
		void WriteInFile(const char* _format, LogType _type, bool _showTimestamp = true);

        ~Log();
        void Destroy();
    };
}
