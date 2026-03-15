#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <memory>

namespace SakuraVNE
{
    class Log
    {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetLogger() {return m_Logger;}
    private:
        static std::shared_ptr<spdlog::logger> m_Logger;
    };
}


#define LOG_INFO(...)  ::SakuraVNE::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  ::SakuraVNE::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::SakuraVNE::Log::GetLogger()->error(__VA_ARGS__)