#pragma once
//SJCLib
#include <SJCLib.h>
//Local
#include "Definitions.h"
//Definitions
#ifdef _DEBUG
#define LOGERROR(msg) CLog::Error(msg, __FUNCTION__, __FILE__, __LINE__);
#else
#define LOGERROR(msg)
#endif

namespace CurseGine
{
	class CURSEGINE_API CLog
	{
	private:
		//Variables
		static SJCLib::CFileOutputStream m_LogFile;
	public:
		//Functions
		static void Error(SJCLib::CString msg, SJCLib::CString function, SJCLib::CString file, uint32 line);
		static void Init();
	};
}