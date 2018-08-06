
#if !defined(__TSLTHREAD_H__)
#define __TSLTHREAD_H__


class CTlsThread  
{
  CWinThread*     m_pThread;
  bool            m_bFinish;
  bool            m_bRunning;

  static UINT __cdecl ThreadFunc(LPVOID pParam);

protected:
  bool FinishRequested();

public:
	CTlsThread();
	virtual ~CTlsThread();

  void Start();
  void Stop();

  virtual int Run();

};

#endif
/*=============================================================================
    E O F
=============================================================================*/
