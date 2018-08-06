
#include "stdafx.h"
#include "TlsLoglist.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#include "TlsDbg.h"
#define new DEBUG_NEW
#define malloc DEBUG_MALLOC
#define free DEBUG_FREE
#define realloc DEBUG_REALLOC
#endif

#include "TlsTime.h"
#include "tlsblob.h"

//=============================================================================
CTlsLoglist::CTlsLoglist(int iMaxLogLen)
: m_EventLog(L"COMM") , m_iMaxLogLen(iMaxLogLen)
{
}

//=============================================================================
CTlsLoglist::~CTlsLoglist()
{
}

//=============================================================================
void CTlsLoglist::AddListener(CTlsLoglistListener* pListener)
{
 m_Listeners.AddTail(pListener);
}

void CTlsLoglist::Clear()
{
	m_Messages.RemoveAll();
}

//=============================================================================
void CTlsLoglist::RemoveListener(CTlsLoglistListener* pListener)
{
  m_Listeners.RemoveAt(m_Listeners.Find(pListener));
}

//=============================================================================
void CTlsLoglist::AddMessageHead(CStringW sMessage)
{
  //m_cs.Lock();
  InitializeCriticalSection(&m_cs);

  //LogToFile(wParam);
  // Geht nicht (wegen Deadlock ??)

  SYSTEMTIME t;
  GetLocalTime(&t);

  CStringW zeit;
  zeit.Format(TEXT("%02d:%02d:%02d: "), t.wHour, t.wMinute, t.wSecond);
  
  sMessage = zeit + sMessage;

  m_Messages.AddHead(sMessage);
  if(m_Messages.GetCount() > m_iMaxLogLen)
    m_Messages.RemoveTail();

  POSITION pos = m_Listeners.GetHeadPosition();
  for (int i=0;i < m_Listeners.GetCount();i++)
  {
    CTlsLoglistListener* pCurr = m_Listeners.GetNext(pos);
    pCurr->AddMessageHead(sMessage);
  }

  //m_cs.Unlock();
  DeleteCriticalSection(&m_cs);
}

//=============================================================================
void CTlsLoglist::AddMessageTail(CStringW sMessage)
{
	//m_cs.Lock();
	InitializeCriticalSection(&m_cs);

	//LogToFile(wParam);
	// Geht nicht (wegen Deadlock ??)

	SYSTEMTIME t;
	GetLocalTime(&t);

	CStringW zeit;
	zeit.Format(TEXT("%02d:%02d:%02d: "), t.wHour, t.wMinute, t.wSecond);

	sMessage = zeit + sMessage;

	m_Messages.AddTail(sMessage);
	if(m_Messages.GetCount() > m_iMaxLogLen)
		m_Messages.RemoveHead();

	POSITION pos = m_Listeners.GetHeadPosition();
	for (int i=0;i < m_Listeners.GetCount();i++)
	{
		CTlsLoglistListener* pCurr = m_Listeners.GetNext(pos);
		pCurr->AddMessageTail(sMessage);
	}

	//m_cs.Unlock();
	DeleteCriticalSection(&m_cs);
}

//=============================================================================
int CTlsLoglist::GetNumMessages()
{
  InitializeCriticalSection(&m_cs);

  int iRet = m_Messages.GetCount();

  DeleteCriticalSection(&m_cs);

  return iRet;
}

//=============================================================================
CStringW CTlsLoglist::GetMessage(int idx)
{
  //m_cs.Lock();
	InitializeCriticalSection(&m_cs);

  CStringW sMessage = m_Messages.GetAt(m_Messages.FindIndex(idx));

  //m_cs.Unlock();
  DeleteCriticalSection(&m_cs);

  return sMessage;
}

//=============================================================================
void CTlsLoglist::LogToFile(CStringW sMessage)
{
  m_EventLog.LogInfo(sMessage);
}

/*=============================================================================
    E O F
=============================================================================*/
