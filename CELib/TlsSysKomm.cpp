
#include "stdafx.h"
#include "TlsSysKomm.h"
#include "TlsRegistry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#include "TlsDbg.h"
#define new DEBUG_NEW
#define malloc DEBUG_MALLOC
#define free DEBUG_FREE
#define realloc DEBUG_REALLOC
#endif

UINT TlsMessageBox(CStringW sCaption, CStringW sText, UINT iFlags);

//=============================================================================
CTlsSysKomm& CTlsSysKomm::GetInstance()
{
  static CTlsSysKomm TheOneAndOnly;
  return TheOneAndOnly;
}

//=============================================================================
CTlsSysKomm::CTlsSysKomm()
  : m_bInSendMessage(false), m_bEnabled(false)
{
  m_iMessageCode = RegisterWindowMessage(L"AISSCANNERMSG");

  if(m_iMessageCode < 0xC000 || m_iMessageCode > 0xFFFF)
    m_iMessageCode = WM_USER+1; // fallback: WM_USER....
}

//=============================================================================
CTlsSysKomm::~CTlsSysKomm()
{
}

//=============================================================================
void CTlsSysKomm::Enable(bool b)
{
  m_bEnabled = b;
}

//=============================================================================
void CTlsSysKomm::AddListener(CTlsSysKommListener* pListener)
{
  m_Listeners.AddTail(pListener);
}

//=============================================================================
void CTlsSysKomm::RemoveListener(CTlsSysKommListener* pListener)
{
  m_Listeners.RemoveAt(m_Listeners.Find(pListener));
}

//=============================================================================
void CTlsSysKomm::Notify(CTlsSysKommListener::eCodes iCode, int iArg)
{
  if(m_bEnabled)
  {
    m_bInSendMessage = true;
    ::SendMessage(HWND_BROADCAST, m_iMessageCode, iCode, iArg);
    m_bInSendMessage = false;
  }
}

//=============================================================================
void CTlsSysKomm::WindowMessage(int iCode, int iArg)
{
  if(m_bInSendMessage)
    return;

  POSITION pos = m_Listeners.GetHeadPosition();
  for (int i=0;i < m_Listeners.GetCount();i++)
  {
    CTlsSysKommListener* pCurr = m_Listeners.GetNext(pos);
    pCurr->SysNotification((CTlsSysKommListener::eCodes)iCode, iArg);
  }
}

//=============================================================================
bool CTlsSysKomm::RegisterApp(CStringW sApp)
{
  HANDLE hMutex = CreateMutex(NULL, TRUE, sApp);
  
  bool bFound=false;
  
  if(GetLastError() == ERROR_ALREADY_EXISTS)	
    bFound = true;
  
  if(hMutex)	
    ReleaseMutex	(hMutex);    
  
  CTlsRegistry reg(HKEY_CURRENT_USER, TEXT("Software\\AIS\\Process"));
  reg.WriteDWORD(sApp, (unsigned long)GetCurrentProcessId());

  return bFound;
}

//=============================================================================
bool CTlsSysKomm::IsRunning(CStringW sApp)
{
  HANDLE hMutex = CreateMutex(NULL, FALSE, sApp);
  
  bool bFound=false;
  
  if(GetLastError() == ERROR_ALREADY_EXISTS)	
    bFound = true;
  
  if(hMutex)	
    ReleaseMutex(hMutex);

  CloseHandle(hMutex);
  
  return bFound;
}

//=============================================================================
bool CTlsSysKomm::StartApp(CStringW sName)
{
//  if(IsRunning(sName))
//    return true;

  CStringW sExeName;
  GetModuleFileName(NULL, sExeName.GetBuffer(512), 512);
  
  CStringW sPath = sExeName.GetBuffer(0);
                     // Kaum zu glauben, aber wenn man GetBuffer(0) weg-
                     // läßt, gehts nicht.            :-((( bsc

  int idx = sPath.ReverseFind(L'\\');
  if(idx != -1)
    sPath = sPath.Left(idx+1);
  sPath += sName;


  PROCESS_INFORMATION pi;

  if(CreateProcess( 
    sPath, // pszImageName 
    NULL, // pszCmdLine
    NULL, // psaProcess, 
    NULL, // psaThread, 
    NULL, // fInheritHandles, 
    0, // fdwCreate, 
    NULL, // pvEnvironment, 
    NULL, // pszCurDir, 
    NULL, // psiStartInfo, 
    &pi // pProcInfo 
    ) == 0)
  {
    return false;
  }

  return true;
}

//=============================================================================
bool CTlsSysKomm::ExecApp(CStringW sPath, DWORD wait)
{
	LPCTSTR pszCmdLine = NULL;
	LPCTSTR pszImageName = NULL;

	CStringW ext = sPath.Right(4);

	if (ext.CompareNoCase(L".cab") == 0)
	{
		pszImageName = L"wceload.exe";
		pszCmdLine = L"/delete 0 /noui \"" + sPath + L"\""; // löscht die cab datei nach dem installieren nicht
	}
	else
	{
		pszImageName = sPath;
	}


  PROCESS_INFORMATION pi;

  if(CreateProcess( 
    pszImageName, // pszImageName 
    pszCmdLine, // pszCmdLine
    NULL, // psaProcess, 
    NULL, // psaThread, 
    NULL, // fInheritHandles, 
    0, // fdwCreate, 
    NULL, // pvEnvironment, 
    NULL, // pszCurDir, 
    NULL, // psiStartInfo, 
    &pi // pProcInfo 
    ) == 0)
  {
    return false;
  }

  WaitForSingleObject(pi.hProcess,wait);

  return true;
}

//=============================================================================
bool CTlsSysKomm::KillApp(CStringW sApp)
{
/*
  CTlsRegistry reg(HKEY_CURRENT_USER, TEXT("Software\\AIS\\Process"));
  HANDLE h = (HANDLE)reg.ReadDWORD(sApp, 0);

  if(h != INVALID_HANDLE_VALUE)
    TerminateProcess(h, 0);
*/
  HWND hwnd = FindWindow(NULL, sApp);
  if(hwnd)
    SendMessage(hwnd, WM_CLOSE, 0, 0);
  
  return true;
}

//=============================================================================
bool CTlsSysKomm::CheckforInstance(CStringW sName)
{
    HWND hwnd = ::FindWindow(NULL, sName);
    if (hwnd)
    {
        ::SetForegroundWindow(hwnd);
        ::BringWindowToTop(hwnd);

        
        //pWndPrev->SetActiveWindow();
        return true;
    }
    else
    {
        return false;
    }
}

//=============================================================================
HWND CTlsSysKomm::HasOneTopWindowTextIncluded(const wchar_t *text)
{	
	wchar_t wtext[1000];

	int z = (sizeof(wtext) - 2) / 2;
	memset(wtext,0,sizeof(wtext));

	HWND child;
	child = ::GetWindow(NULL, GW_CHILD);

	while (child != NULL) 
	{
		::GetWindowText(child,wtext,z);

		if (wcsstr(wtext, text) != NULL)
		{
			return child;
		}

		child = ::GetWindow(child,GW_HWNDNEXT);
	}

	return NULL;
}

/*=============================================================================
    E O F
=============================================================================*/
