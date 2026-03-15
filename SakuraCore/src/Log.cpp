#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace SakuraVNE{

    std::shared_ptr<spdlog::logger> Log::m_Logger;
    
    
    void Log::Init(){
        spdlog::set_pattern("%^[%T] %n: %v%$");

        m_Logger = spdlog::stdout_color_mt("Sakura");
        m_Logger->set_level(spdlog::level::trace);
    }
}