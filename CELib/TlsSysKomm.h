
#if !defined(__TLSSYSKOMM_H__)
#define __TLSSYSKOMM_H__


class CTlsSysKommConst
{
public:
  enum eCodes { SK_FLUSHDB=1, 
                SK_EV_DISCONNECT,
                SK_EV_NORESPONSE, 
                SK_EV_SHLCHANGED, 
                SK_EV_WRONGOBC,
                SK_EV_OBCIDENT,
                SK_EV_OBCTRANSFERED,
                SK_EV_NOCHANGE};

};

class CTlsSysKommListener : public CTlsSysKommConst
{
public:
  virtual void SysNotification(eCodes iCode, int iArg)=0;
};

class CTlsSysKomm : public CTlsSysKommConst
{
public:

private:
  bool m_bEnabled;
  UINT m_iMessageCode;
  bool m_bInSendMessage;
  CList <CTlsSysKommListener*, CTlsSysKommListener*> m_Listeners;

	CTlsSysKomm();

public:
	virtual ~CTlsSysKomm();

  static CTlsSysKomm& GetInstance();

  // +++ Listeners +++
  void AddListener(CTlsSysKommListener* pListener);
  void RemoveListener(CTlsSysKommListener* pListener);

  // +++ Notifications +++
  void Enable(bool b);
  void Notify(eCodes iCode, int iArg=0);
  void WindowMessage(int iCode, int iArg);  // MUST be called by Main-Frame-Window
  UINT GetWindowMessage() { return m_iMessageCode; }

  // +++ check for running processes +++
  bool RegisterApp(CStringW sApp);
  bool IsRunning(CStringW sApp);
  bool KillApp(CStringW sApp);

  // +++ start an application +++
  bool StartApp(CStringW sName);
  bool CheckforInstance(CStringW sName);
  static bool ExecApp(CStringW sName, DWORD wait = 0);
  
  static HWND HasOneTopWindowTextIncluded(const wchar_t *text);
};

#endif
/*=============================================================================
    E O F
=============================================================================*/
