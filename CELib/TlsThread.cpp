
#include "stdafx.h"
#include "TlsThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#include "TlsDbg.h"
#define new DEBUG_NEW
#define malloc DEBUG_MALLOC
#define free DEBUG_FREE
#define realloc DEBUG_REALLOC
#endif

//=============================================================================
CTlsThread::CTlsThread()
: m_pThread(NULL), m_bFinish(false), m_bRunning(false)
{

}

//=============================================================================
CTlsThread::~CTlsThread()
{
  Stop();
}

//=============================================================================
UINT __cdecl CTlsThread::ThreadFunc(LPVOID pParam)
{
  CTlsThread* pThread = (CTlsThread*)pParam;
  pThread->m_bRunning = true;
  pThread->m_bFinish  = false;

  UINT iRet  = ((CTlsThread*)pParam)->Run();

  pThread->m_bRunning = false;

  return iRet;
}

//=============================================================================
void CTlsThread::Start()
{
  m_bFinish = false;
  m_pThread = AfxBeginThread(ThreadFunc, this);
}

//=============================================================================
void CTlsThread::Stop()
{
  m_bFinish = true;

  if(m_pThread && m_bRunning)
  {
    if(WaitForSingleObject(m_pThread->m_hThread,5000) == WAIT_TIMEOUT)
    {
      TerminateThread(m_pThread->m_hThread, 0);
      m_pThread = NULL;
    }
  }
}

//=============================================================================
bool CTlsThread::FinishRequested()
{
  return m_bFinish;
}

//=============================================================================
int CTlsThread::Run()
{
  return -1;
}

/*=============================================================================
    E O F
=============================================================================*/
