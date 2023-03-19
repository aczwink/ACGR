//Class Header
#include "CLog.h"
//Global
#include <ctime>
#include <Windows.h>
//Local
#include "resource.h"
//Namespaces
using namespace SJCLib;
using namespace CurseGine;

//Class Variables
CFileOutputStream CLog::m_LogFile;

//Non-class Functions
void ExitOnError(CFileOutputStream &file)
{
	file.Close();
	exit(EXIT_FAILURE);
}

//Public Functions
void CLog::Error(CString msg, CString function, CString file, uint32 line)
{
	time_t timeStamp;
	tm timeInfo;

	timeStamp = time(NULL);
	localtime_s(&timeInfo, &timeStamp);

	m_LogFile << CString(timeInfo.tm_sec, NS_DEC, false, 2) << '.' << CString(GetTickCount(), NS_DEC, false, 2) << "\tERROR: " << msg << " in " << function << ' ' << file << '(' << line << ')' << endl;
	m_LogFile.Flush();
	ExitOnError(m_LogFile);
}

void CLog::Init()
{
	time_t timeStamp;
	tm timeInfo;

	m_LogFile.Open("CurseGine.log");

	timeStamp = time(NULL);
	localtime_s(&timeInfo, &timeStamp);

	m_LogFile << "Running CurseGine version " << APPLICATION_VERSION_STRING << " on " << CString(timeInfo.tm_mday, NS_DEC, false, 2) << '.' << CString(timeInfo.tm_mon+1, NS_DEC, false, 2) << '.' << CString(timeInfo.tm_year+1900, NS_DEC, false, 2) << " - " << CString(timeInfo.tm_hour, NS_DEC, false, 2) << ':' << CString(timeInfo.tm_sec, NS_DEC, false, 2) << endl << endl;
}