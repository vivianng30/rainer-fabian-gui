
#if !defined(__TLSSYSTEMKEYS_H__)
#define __TLSSYSTEMKEYS_H__

// xxx #include <winbase.h>

//===========================================================================//
//
//\brief 
//
//  Verändert die Hardware-Buttons des Organizers.
//  Umsetzung lt. 'The Windows CE Technology Tutorial', pg. 250.
//  Vorsicht: Benutzt undokumentierte Windows-Funktionen !!
//
//  see CTlsFrameWnd for a full sample.
//
//===========================================================================//
class CTlsSystemKeys  
{
public:
  enum eKey { KEY_APP1, KEY_APP2, KEY_APP3, KEY_APP4, KEY_APP5, 
              KEY_ACTION, KEY_SCRUP, KEY_SCRDOWN, KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_DOWN, KEY_CANCEL_CALL, KEY_NONE};
    // leave KEY_NONE always as last entry !!!

private:
  typedef BOOL (__stdcall *UnregisterFunc1Proc)( UINT, UINT );
  typedef BYTE (*SHGETAPPKEYASSOCPROC)(LPCTSTR ptszApp);

	HINSTANCE m_hCoreDll;
  HINSTANCE m_hAygShell;

  HWND      m_hWnd;
	UnregisterFunc1Proc  m_procUnregisterFunc;
  SHGETAPPKEYASSOCPROC m_procShGetAppKeyAssocProc;
  
  bool RegisterHotKey(HWND hWnd, eKey key, bool bRegister);

  bool abHotKeys[KEY_NONE];
  
public:
	CTlsSystemKeys();
	virtual ~CTlsSystemKeys();

  static CTlsSystemKeys& GetInstance();

  bool RegisterHotKey(HWND hWnd, eKey Key);
  bool UnRegisterHotKey(eKey Key);

  void RestoreSystemKeys();

  void ActivateCurrentSetting(HWND hWnd);
  void DeactivateCurrentSetting();
  void DisableAllKeys(HWND hWnd);

  eKey IsSystemKey(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif
/*=============================================================================
    E O F
=============================================================================*/
