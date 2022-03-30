#pragma once

#include "ServerityLevel.h"
#include <fstream>

namespace asuna
{
    class Logger {
    public:
        /**
         * setup logger environment
         */
        static void InitLogger(const char* dir, const char* filename);

        static void Info(const char* message, ...);
        static void Warning(const char* message, ...);
        static void Error(const char* message, ...);

        static void InfoScript(const char* message, ...);
        static void WarningScript(const char* message, ...);
        static void ErrorScript(const char* message, ...);


        static void ToConsole(const char* message);
		static void ToFile(const char* message);


    private:

        static void Log(SeverityLevel level, const char* tag, const char* message);

        static void SetupConsoleLog();
        static void SetupFileLog(const char* dir, const char* filename);

        static bool console_enable_;
        static bool file_enable_;

    };

}

