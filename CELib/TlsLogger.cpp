
#include "stdafx.h"
#include "TlsTime.h"
#include "TlsFile.h"
#include "TlsLogger.h"
#include "TlsRegistry.h"
#include "TlsStream.h"

#ifdef _DEBUG
#include "TlsDbg.h"
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define malloc DEBUG_MALLOC
#define free DEBUG_FREE
#define realloc DEBUG_REALLOC
#endif

#define LOG_DIRECTORY L"\\SysLog"
#define LOG_MAX_AGE   2   // days

//=============================================================================
//
//  /brief Generate a Log Name
//  
//  e.g. EVENT_20030710.log
//
//=============================================================================
CStringW CTlsLogger::GetLogName()
{
  return CStringW(LOG_DIRECTORY) + L"\\" + m_sPrefix + L"_" + CTlsTime::TS().GetExpDate() + L".log";
}

//=============================================================================
CTlsLogger::CTlsLogger(CStringW sPrefix)
: m_sPrefix(sPrefix), m_iCount(0)
{
  CTlsFile::MkDir(LOG_DIRECTORY);
  //RemoveOldLogs();
}

//=============================================================================
CTlsLogger::~CTlsLogger()
{
}

//=============================================================================
//
//  /brief Append char's to the log
//
//=============================================================================
bool CTlsLogger::Append(char *pBuf, WORD len)
{
  CTlsOStream_File file(GetLogName(), true);

  file.Write(pBuf, len);

  m_iCount++;
  if(m_iCount > 250)
  {
    // 250 ist natürlich völlig willkürlich 
    // - es sollen halt ab und zu alte Logs 
    // gelöscht werden.
    m_iCount = 0;
    //RemoveOldLogs();
  }

  return true;
}

//=============================================================================
//
//  /brief Append a Line to the log
//
//=============================================================================
bool CTlsLogger::AppendLine(CStringW sLine)
{
  int iLen = sLine.GetLength();
  char* pMsg = new char[iLen+1];
  wcstombs(pMsg, sLine, iLen);
  pMsg[iLen]   = '\r';
  pMsg[iLen+1] = '\n';
  pMsg[iLen+2] = '\0';

  Append(pMsg, iLen+2);
  delete [] pMsg;
    
  return true;
}

//=============================================================================
bool CTlsLogger::RemoveOldLogs()
{
	WIN32_FIND_DATA FileData;
	CStringW			    msg;
	//bool			      bExists	= false;

  //LOG_DIRECTORY + m_sPrefix + "_" + tm.GetExpDate() + ".log"

  CTlsTime tmTrigger = CTlsTime::TS();
  tmTrigger.Add(- LOG_MAX_AGE);
  CStringW sTrigger = tmTrigger.GetExpDate();

  HANDLE hSearch = ::FindFirstFile(LOG_DIRECTORY L"\\*.*", &FileData);
	if (hSearch != INVALID_HANDLE_VALUE) 
  {
    do
    {
      CStringW sFilename = FileData.cFileName;
      
      if(sFilename.Find(m_sPrefix) != -1)
      {
        CStringW sDate = sFilename.Mid(m_sPrefix.GetLength()+1, 8);
        if(sDate <= sTrigger)
        {
          CTlsFile::Delete(CStringW(LOG_DIRECTORY) + L"\\" + sFilename);
        }
      }
    } while(::FindNextFile(hSearch, &FileData));
      
		FindClose(hSearch);
	}
	return true;
}

//=============================================================================
//bool CTlsLoggerSystemState::Log()
//{
//  if(!m_bLog)
//    return true;
//  
//  SYSTEM_POWER_STATUS_EX2 sps;  
//	GetSystemPowerStatusEx2(&sps, sizeof(SYSTEM_POWER_STATUS_EX2), true);
//
//  static MEMORYSTATUS	MemStatus;
//	GlobalMemoryStatus(&MemStatus);
//
//  //CTlsDevice& rDevice = CTlsDevice::GetInstance();
//  //CTlsPDA& rDevice = CTlsPDA::GetInstance();
//  CStringW sLine;
//  /*sLine.Format(_T("%3d;%d;%3d;%d;%d"), 
//      rDevice.GetBatteryLifePercent(),
//      rDevice.GetBatteryFlag(),
//      rDevice.GetBackupBatteryLifePercent(),
//      rDevice.GetBackupBatteryFlag(),
//      rDevice.GetAvailableMemory());*/
//  /*sLine.Format(_T("%3d;%d;%3d;%d;%d"), 
//      CTlsPDA::GetInstance().GetBatteryLifePercent(),
//      CTlsPDA::GetInstance().GetBatteryFlag(),
//      CTlsPDA::GetInstance().GetBackupBatteryLifePercent(),
//      CTlsPDA::GetInstance().GetBackupBatteryFlag(),
//      CTlsPDA::GetInstance().GetAvailableMemory());*/
//  sLine.Format(_T("%3d;%d;%3d;%d;%d"), 
//      CTlsDevice::GetInstance().GetBatteryLifePercent(),
//      CTlsDevice::GetInstance().GetBatteryFlag(),
//      CTlsDevice::GetInstance().GetBackupBatteryLifePercent(),
//      CTlsDevice::GetInstance().GetBackupBatteryFlag(),
//      CTlsDevice::GetInstance().GetAvailableMemory());
//
//  sLine = CTlsTime::TS().GetDispTime() + ";" + sLine;
//
//  return AppendLine(sLine);
//};

//=============================================================================
CTlsEventLog::CTlsEventLog(CStringW sPrefix, eVerbosity verbosity)
  : CTlsLogger(sPrefix), m_Verbosity(verbosity)
{
  GetVerbosity();
}

//=============================================================================
void CTlsEventLog::SetVerbosity(eVerbosity verbosity)
{
  m_Verbosity = verbosity;
}

//=============================================================================
CTlsEventLog::eVerbosity CTlsEventLog::GetVerbosity()
{
  return m_Verbosity;
}

//=============================================================================
void CTlsEventLog::Log(eErrorLevel eEl, CStringW sMessage)
{
  if(eEl < m_Verbosity)
    return;  // no logging needed.

  CStringW pszErrLevel = L"";
  switch(eEl)
  {
  case EL_INFO: pszErrLevel = "INFO"; break;
  case EL_WARN: pszErrLevel = "WARN"; break;
  case EL_ERR:  pszErrLevel = "ERR"; break;
  case EL_FATAL:pszErrLevel = "FATAL"; break;
  }

  CStringW sLine = CTlsTime::TS().GetDispTime() + L", " + pszErrLevel + L", " + sMessage;

  AppendLine(sLine);
}

//=============================================================================
void CTlsEventLog::LogInfo(CStringW sMessage)
{
  Log(EL_INFO, sMessage);
}

//=============================================================================
void CTlsEventLog::LogWarning(CStringW sMessage)
{
  Log(EL_WARN, sMessage);
}

//=============================================================================
void CTlsEventLog::LogError(CStringW sMessage)
{
  Log(EL_ERR, sMessage);
}

//=============================================================================
void CTlsEventLog::LogFatal(CStringW sMessage)
{
  Log(EL_FATAL, sMessage);
}

//=============================================================================
CTlsEventLog& CTlsEventLog::GetInstance()
{
  static CTlsEventLog TheOneAndOnly;
  return TheOneAndOnly;
}

/*=============================================================================
    E O F
=============================================================================*/
