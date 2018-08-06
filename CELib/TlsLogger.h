
#if !defined(__TLSLOGGER_H__)
#define __TLSLOGGER_H__


//===========================================================================//
//
//\brief 
//
//  Generiert Dateinamen, löscht automatisch alte Logs usw.
//
//===========================================================================//
class CTlsLogger  
{
  CStringW m_sPrefix;
  int     m_iCount;

  CStringW GetLogName();
	bool	  RemoveOldLogs();

public:
  CTlsLogger(CStringW sPrefix);
  virtual ~CTlsLogger();

  bool  AppendLine(CStringW sLine);
	bool	Append(char *pBuf, WORD len);
};

class CTlsEventLog : protected CTlsLogger
{
public:
  enum eErrorLevel {EL_INFO = 1, EL_WARN = 2, EL_ERR = 3, EL_FATAL = 4};
  enum eVerbosity  {VB_ALL = 0, VB_WARN = 2, VB_ERROR = 3, VB_FATAL = 4, VB_NONE=99};

private:
	eVerbosity m_Verbosity;

  void Log(eErrorLevel eErrorLevel, CStringW sMessage);

public:
  CTlsEventLog(CStringW sPrefix = L"MSG", eVerbosity verbosity = VB_ALL);

  void       SetVerbosity(eVerbosity verbosity);
  eVerbosity GetVerbosity();
  
  void LogInfo(CStringW sMessage);
  void LogWarning(CStringW sMessage);
  void LogError(CStringW sMessage);
  void LogFatal(CStringW sMessage);

  static CTlsEventLog& GetInstance();
};

#endif
/*=============================================================================
    E O F
=============================================================================*/
