#pragma once

#define ATTR_FORMAT(type,format_pos,arg_pos) __attribute__((format(printf,format_pos,arg_pos)))

#define LOG_LEVELS( _T ) \
	_T( Fatal ) \
	_T( Error ) \
	_T( Warning ) \
	_T( Info ) \
	_T( Detail ) \

#define AS_ENUM( level ) Log##level,
enum ELogLevel { LOG_LEVELS( AS_ENUM ) LogLevelMax };
#undef AS_ENUM

void Log( const char *pchFormat, ... ) ATTR_FORMAT( printf, 1, 2 );
void Log( ELogLevel eLevel, const char *pchFormat, ... ) ATTR_FORMAT( printf, 2, 3 );