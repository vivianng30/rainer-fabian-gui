
#include "stdafx.h"
#include "TlsSystemKeys.h"
#include <aygshell.h>

#ifdef _DEBUG
#include "TlsDbg.h"
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define malloc DEBUG_MALLOC
#define free DEBUG_FREE
#define realloc DEBUG_REALLOC
#endif

//=============================================================================
CTlsSystemKeys& CTlsSystemKeys::GetInstance()
{
  static CTlsSystemKeys keys;

  return keys;
}

//=============================================================================
CTlsSystemKeys::CTlsSystemKeys()
	: m_hCoreDll(NULL), m_hAygShell(NULL)
{
	m_hWnd=NULL;
  for(int idx = 0; idx < KEY_NONE; idx++)
    abHotKeys[idx] = false;

	m_hCoreDll = LoadLibrary(_T("coredll.dll"));
	if(m_hCoreDll)
		m_procUnregisterFunc = (UnregisterFunc1Proc)GetProcAddress(m_hCoreDll, _T("UnregisterFunc1"));

	m_hAygShell = LoadLibrary(_T("aygshell.dll"));
	if(m_hAygShell)
		m_procShGetAppKeyAssocProc = (SHGETAPPKEYASSOCPROC)GetProcAddress(m_hAygShell, _T("SHGetAppKeyAssoc"));
  
}

//=============================================================================
CTlsSystemKeys::~CTlsSystemKeys()
{
  RestoreSystemKeys();

	if(m_hCoreDll)
		::FreeLibrary(m_hCoreDll);

	if(m_hAygShell)
		::FreeLibrary(m_hAygShell);
  
}

//=============================================================================
//
// \brief register a hot key
//
//  KEY_APP1  193 0xC1
//  KEY_APP2  194 0xC2
//  KEY_APP3  195 0xC3
//  KEY_APP4  196 0xC4
//  KEY_APP5  197 0xC5
//  KEY_ACTION  134 0x86
//  KEY_SCRUP 38  0x26
//  KEY_SCRDOWN 40  0x27
//
//=============================================================================
bool CTlsSystemKeys::RegisterHotKey(HWND hWnd, eKey key, bool bRegister)
{
  m_hWnd = hWnd;
  int iKey = 0;

  switch(key)
  {
  case KEY_APP1: iKey = 0xC1; break;
  case KEY_APP2: iKey = 0xC2; break;
  case KEY_APP3: iKey = 0xC3; break;
  case KEY_APP4: iKey = 0xC4; break;
  case KEY_APP5: iKey = 0xC5; break;
  case KEY_ACTION: iKey = 0x86; break;
  case KEY_SCRUP: iKey = 0x26; break;
  case KEY_SCRDOWN: iKey = 0x27; break;
  
  case KEY_LEFT: iKey = 37; break;
  case KEY_UP: iKey = 38; break;
  case KEY_RIGHT: iKey = 39; break;
  case KEY_DOWN: iKey = 40; break;
  case KEY_CANCEL_CALL: iKey = 115; break;

  default: return false;
  }
  
  if(bRegister)
  {
	  ASSERT(m_procUnregisterFunc);
	  BOOL bResult1 = m_procUnregisterFunc(MOD_WIN, iKey);
	  BOOL bResult2 = ::RegisterHotKey(hWnd, iKey, MOD_WIN, iKey);

    return bResult2 && bResult1;
  }
  else
  {
    TCHAR szAppName[255];
    BYTE byteButton;

    m_procUnregisterFunc(MOD_WIN, iKey);

    // bsc: eigentlich völlig bescheuert. 
    // Meiner Ansicht nach sollte hier garantiert nicht das eigene
    // HWND übergeben werden. 
    // Aber ein anderes hab' ich grad nicht zur Hand... 
    // Im Buch ist die ganze Stelle sehr unklar und 
    // seltsamerweise gibt' zu den App-Buttons auch 
    // kein Beispiel auf der CD...
    if(byteButton = m_procShGetAppKeyAssocProc(szAppName))
      ::RegisterHotKey(hWnd, iKey, MOD_WIN, byteButton);

    return true;
  }
}

//=============================================================================
//
// \brief register a hot-key
// \param hWnd the window which receives the Key-Messages
// \param key the Key to register
// \return true if success
//
//=============================================================================
bool CTlsSystemKeys::RegisterHotKey(HWND hWnd, eKey key)
{
  if(key >= KEY_NONE)
    return false;

  if(abHotKeys[key])
    return true;

  RegisterHotKey(hWnd, key, true);

  abHotKeys[key] = true;
  return true;
}

//=============================================================================
//
// \brief unregister a hot-key
// \param key the Key to unregister
// \return true if success
//
//=============================================================================
bool CTlsSystemKeys::UnRegisterHotKey(eKey key)
{
  if(key >= KEY_NONE)
    return false;

  if(!abHotKeys[key])
    return true;

  RegisterHotKey(m_hWnd, key, false);

  abHotKeys[key] = false;

  return true;
}

//=============================================================================
//
// \brief restore the original settings
//
//  the function is called automatically by the destructor.
//  
//  Warning: if the program crashes or doesn't call 
//  RestoreSystemKeys, the settings won't be restored !!!
//
//=============================================================================
void CTlsSystemKeys::RestoreSystemKeys()
{
  for(int idx = 0; idx < KEY_NONE; idx++)
  {
    if(abHotKeys[idx])
      UnRegisterHotKey((eKey)idx);
  }

  SendMessage(HWND_BROADCAST, WM_WININICHANGE, SPI_APPBUTTONCHANGE, 0);
}

//=============================================================================
//
// \brief restore System-Keys setting
//
//  should be called when program looses the focus
// (receiving WM_ACTIVATE with LOWORD(wParam) == WA_INACTIVE)
//
//=============================================================================
void CTlsSystemKeys::DeactivateCurrentSetting()
{
  for(int idx = 0; idx < KEY_NONE; idx++)
  {
    if(abHotKeys[idx])
      RegisterHotKey(m_hWnd, (eKey)idx, false);
  }
}

//=============================================================================
//
// \brief set current System-Keys setting
//
//  should be called when program is activated the focus
//  (receiving WM_ACTIVATE with LOWORD(wParam) != WA_INACTIVE)
//
//=============================================================================
void CTlsSystemKeys::ActivateCurrentSetting(HWND hWnd)
{
  for(int idx = 0; idx < KEY_NONE; idx++)
  {
    if(abHotKeys[idx])
      RegisterHotKey(hWnd, (eKey)idx, true);
  }
}

//=============================================================================
//
// \brief deactive all system-keys
//
//  for keylock-mode
//
//=============================================================================
void CTlsSystemKeys::DisableAllKeys(HWND hWnd)
{
  for(int idx = 0; idx < KEY_NONE; idx++)
    RegisterHotKey(hWnd, (eKey)idx, true);
}

//=============================================================================
//
// \brief check if message is caused by a system-key
// \param message message-id
// \param wParam wParam
// \param lParam lParam
// \return code for system-key or KEY_NONE if it's a non-system-key-message.
//
//  call this function in the defwindow-proc of the window used in the
 // hwnd-param of 'register hot key'
//
//=============================================================================
CTlsSystemKeys::eKey CTlsSystemKeys::IsSystemKey(UINT message, WPARAM wParam, LPARAM lParam)
{
  eKey key = KEY_NONE;

  // +++ check message +++
  switch(message)
  {
  case WM_HOTKEY:
    {
      switch(wParam)
      {
      case 0xC1: key = KEY_APP1; break;
      case 0xC2: key = KEY_APP2; break;
      case 0xC3: key = KEY_APP3; break;
      case 0xC4: key = KEY_APP4; break;
      case 0xC5: key = KEY_APP5; break;
      case 0x86: key = KEY_ACTION; break;
      case 0x26: key = KEY_SCRUP; break;
      case 0x27: key = KEY_SCRDOWN; break;
      default: return KEY_NONE;
      }
    }
    break;
  default:
    return KEY_NONE;
  }

  // +++ check registration +++
  if(!abHotKeys[key])
    return KEY_NONE;

  return key;
}

/*=============================================================================
    E O F
=============================================================================*/
