#include "log.h"

#include <cstdarg>
#include <android/log.h>

void LogHelper( ELogLevel level, const char *pMsgFormat, va_list &args )
{
    android_LogPriority priority;

    switch ( level )
    {
        case ELogLevel::LogError:
        {
            priority = ANDROID_LOG_ERROR;
            break;
        }
        case ELogLevel::LogFatal:
        {
            priority = ANDROID_LOG_FATAL;
            break;
        }
        case ELogLevel::LogWarning:
        {
            priority = ANDROID_LOG_WARN;
            break;
        }
        default:
        {
            priority = ANDROID_LOG_INFO;
            break;
        }
    }

    __android_log_vprint( priority, "mynativeapp", pMsgFormat, args );

    va_end( args );
}

void Log( ELogLevel eLevel, const char *pchFormat, ... )
{
    va_list args;
    va_start( args, pchFormat );
    LogHelper( eLevel, pchFormat, args );
    va_end( args );
}

void Log( const char *pchFormat, ... )
{
    va_list args;
    va_start( args, pchFormat );
    LogHelper( LogInfo, pchFormat, args );
    va_end( args );
}