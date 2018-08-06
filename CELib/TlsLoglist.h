
#if !defined(__TLSLOGLIG_H__)
#define __TLSLOGLIG_H__

#include "TlsLogger.h"

class CTlsLoglistListener
{
public:
  //virtual void AddMessage(CStringW sMessage)=0;
	virtual void AddMessageHead(CStringW sMessage)=0;
	virtual void AddMessageTail(CStringW sMessage)=0;
};

class CTlsLoglist  
{
  CTlsEventLog m_EventLog;

  CRITICAL_SECTION m_cs;

  CList<CTlsLoglistListener*, CTlsLoglistListener*> m_Listeners;
  CList<CStringW,CStringW&> m_Messages;

  int m_iMaxLogLen;

public:
	CTlsLoglist(int iMaxLogLen);
	virtual ~CTlsLoglist();

  void AddListener(CTlsLoglistListener* pListener);
  void RemoveListener(CTlsLoglistListener* pListener);

  void AddMessageHead(CStringW sMessage);
  void AddMessageTail(CStringW sMessage);

  int GetNumMessages();
  CStringW GetMessage(int idx);

  void LogToFile(CStringW sMessage);

  void Clear();
};

#endif
/*=============================================================================
    E O F
=============================================================================*/
