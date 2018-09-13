
#include "stdafx.h"
#include "TlsRegistry.h"
#include <string.h>
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

//=============================================================================
void CTlsRegistry::Init(HKEY hkeyParent, CStringW pcszSubkey, bool)
{
	m_strSubkey = pcszSubkey;
	m_hkeyParent = hkeyParent;
	

	long ret;
	if((ret = RegOpenKeyEx(
		hkeyParent,  //HKEY hKey, 
		pcszSubkey, //LPCWSTR lpSubKey, 
		0,          // DWORD ulOptions, 
		NULL,       //REGSAM samDesired, 
		&m_hkey))    // result
		!= ERROR_SUCCESS)
	{
		DWORD dwDisposition;
		RegCreateKeyEx(hkeyParent, pcszSubkey, 0, NULL, 0, 0, NULL, &m_hkey, &dwDisposition);
	}
}

//=============================================================================
CTlsRegistry::CTlsRegistry(CStringW sPath, bool bWrite)
  : m_nSubkeyIndex(0)
{
  if(sPath.Find(L"HKLM\\") != -1)
    Init(HKEY_LOCAL_MACHINE, sPath.Right(sPath.GetLength()-5), bWrite);
  else if(sPath.Find(L"HKCU\\") != -1)
    Init(HKEY_CURRENT_USER, sPath.Right(sPath.GetLength()-5), bWrite);
  else if(sPath.Find(L"HKCR\\") != -1)
    Init(HKEY_CLASSES_ROOT, sPath.Right(sPath.GetLength()-5), bWrite);
  else if(sPath.Find(L"HKEY_LOCAL_MACHINE\\") != -1)
    Init(HKEY_LOCAL_MACHINE, sPath.Right(sPath.GetLength()-19), bWrite);
  else if(sPath.Find(L"HKEY_CURRENT_USER\\") != -1)
    Init(HKEY_CURRENT_USER, sPath.Right(sPath.GetLength()-17), bWrite);
  else if(sPath.Find(L"HKEY_CLASSES_ROOT\\") != -1)
    Init(HKEY_CLASSES_ROOT, sPath.Right(sPath.GetLength()-17), bWrite);
}

//=============================================================================
CTlsRegistry::CTlsRegistry(HKEY hkeyParent, CStringW pcszSubkey, bool bWrite)
  : m_nSubkeyIndex(0)
{
  Init(hkeyParent, pcszSubkey, bWrite);
}

//=============================================================================
CTlsRegistry::CTlsRegistry(const CTlsRegistry& rSrc) 
{
	DWORD dwDisposition;

	RegCreateKeyEx(rSrc.m_hkey, rSrc.m_strSubkey, 0, NULL, 0, 0, NULL, &m_hkey, &dwDisposition);
	m_strSubkey = rSrc.m_strSubkey;
	m_hkeyParent = rSrc.m_hkeyParent;
	m_nSubkeyIndex = 0;
}

//=============================================================================
CTlsRegistry::~CTlsRegistry()
{
	RegCloseKey(m_hkey);
}

//=============================================================================
void CTlsRegistry::FlushHKLM()
{
	 RegFlushKey(HKEY_LOCAL_MACHINE);
}

void CTlsRegistry::FlushHKCU()
{
	 RegFlushKey(HKEY_CURRENT_USER);
}

//=============================================================================
bool CTlsRegistry::IsValid(CStringW sPath)
{
  if(sPath.Find(L"HKLM\\") != -1)
    return IsValid(HKEY_LOCAL_MACHINE, sPath.Right(sPath.GetLength()-5));
  else if(sPath.Find(L"HKCU\\") != -1)
    return IsValid(HKEY_CURRENT_USER, sPath.Right(sPath.GetLength()-5));
  else if(sPath.Find(L"HKCR\\") != -1)
    return IsValid(HKEY_CLASSES_ROOT, sPath.Right(sPath.GetLength()-5));
  else if(sPath.Find(L"HKEY_LOCAL_MACHINE\\") != -1)
    return IsValid(HKEY_LOCAL_MACHINE, sPath.Right(sPath.GetLength()-19));
  else if(sPath.Find(L"HKEY_CURRENT_USER\\") != -1)
    return IsValid(HKEY_CURRENT_USER, sPath.Right(sPath.GetLength()-17));
  else if(sPath.Find(L"HKEY_CLASSES_ROOT\\") != -1)
    return IsValid(HKEY_CLASSES_ROOT, sPath.Right(sPath.GetLength()-17));
  else
	return false;
}

//=============================================================================
bool CTlsRegistry::IsValid(HKEY hkeyParent, CStringW pcszSubkey)
{
	bool bSuccess = false;

	HKEY hkey;
	if(RegOpenKeyEx(
		hkeyParent,  //HKEY hKey, 
		pcszSubkey, //LPCWSTR lpSubKey, 
		0,          // DWORD ulOptions, 
		NULL,       //REGSAM samDesired, 
		&hkey)    // result
		== ERROR_SUCCESS)
	{
		bSuccess = true;
		RegCloseKey(hkey);
	}

	return bSuccess;
}

//=============================================================================
CStringW CTlsRegistry::ReadString(CStringW pcszKey, CStringW pcszDefault)
{
	if(!AfxIsValidString(pcszKey))
	{
		return TEXT("");
	}

	DWORD	dwDataSize = 0;
	DWORD	dwType = REG_SZ; 

	if(RegQueryValueEx(m_hkey, pcszKey, 0, &dwType, (PBYTE)NULL, &dwDataSize) == ERROR_SUCCESS)
	{
		CStringW strValue;

		if(RegQueryValueEx(m_hkey, pcszKey, 0, &dwType, (PBYTE)(LPTSTR)strValue.GetBuffer(dwDataSize + 1), &dwDataSize) == ERROR_SUCCESS)
		{
			strValue.ReleaseBuffer();
			return strValue;
		}
		else
		{
			strValue.ReleaseBuffer();
		}
	}

	// Write the default value (if any)  Note: If there was a value in the registry, it
	// would already have been written
	if(AfxIsValidString(pcszDefault))
	{
		if(RegSetValueEx(m_hkey, (LPCTSTR)pcszKey, 0, REG_SZ, (PBYTE) (LPCTSTR)pcszDefault, (_tcslen(pcszDefault) + 1) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			//TRACE(TEXT("CTlsRegistry(%s)::ReadString() Unable to write value %s to variable %s\n"), m_strSubkey, pcszDefault, pcszKey);
		}
		return pcszDefault;
	}
	else
		return TEXT("");
}

//=============================================================================
bool CTlsRegistry::WriteString(CStringW pcszKey, CStringW pcszValue)
{
	if(!AfxIsValidString(pcszKey))
	{
		return FALSE;
	}
	if(!AfxIsValidString(pcszValue))
	{
		return FALSE;
	}

	return (RegSetValueEx(m_hkey, (LPCTSTR)pcszKey, 0, REG_SZ, (PBYTE)(LPCTSTR)pcszValue, (_tcslen(pcszValue) + 1) * sizeof(TCHAR)) == ERROR_SUCCESS);
}

//=============================================================================
DWORD CTlsRegistry::ReadDWORD(CStringW pcszKey, DWORD dwDefault)
{
	DWORD dwValue;
	DWORD dwValueSize = sizeof(DWORD);
	DWORD dwType = REG_DWORD;

	if(RegQueryValueEx(m_hkey, pcszKey, 0, &dwType, (PBYTE)&dwValue, &dwValueSize) == ERROR_SUCCESS)
		return dwValue;

	if(dwDefault != 0xDEF0)	// Default specified
		RegSetValueEx(m_hkey, pcszKey, 0, REG_DWORD, (PBYTE)&dwDefault, sizeof(DWORD));

	return dwDefault;
}

//=============================================================================
bool CTlsRegistry::WriteDWORD(CStringW pcszKey, DWORD dwValue)
{
	if(!AfxIsValidString(pcszKey))
	{
		return FALSE;
	}

	return (RegSetValueEx(m_hkey, pcszKey, 0, REG_DWORD, (PBYTE)&dwValue, sizeof(DWORD)) == ERROR_SUCCESS);
}

//=============================================================================
bool CTlsRegistry::WriteBlob(CStringW sKey, CTlsBlob bl)
{
	return (RegSetValueEx(m_hkey, sKey, 0, REG_BINARY, (PBYTE)bl.GetBuffer(), bl.GetBufferLen()) == ERROR_SUCCESS);
}

//=============================================================================
bool CTlsRegistry::WriteMultiString(CStringW sKey, CTlsBlob bl)
{
	int len = bl.GetBufferLen();

	const unsigned char null = 0x00;
	unsigned char * wert = bl.GetBuffer();
	CTlsBlob neu;

	for (int x = 0; x < len; x++)
	{
		neu.Append(wert + x,1);
		neu.Append(&null,1);
		
	}

	PBYTE buffer = (PBYTE)neu.GetBuffer();
	int bufferlen = neu.GetBufferLen();
	bool ok = RegSetValueEx(m_hkey, sKey, 0, REG_MULTI_SZ, buffer, bufferlen) == ERROR_SUCCESS;
	return ok;
}

//=============================================================================
CStringW CTlsRegistry::ReadMultiString(CStringW pcszKey, CStringW pcszDefault)
{
	if(!AfxIsValidString(pcszKey))
	{
		return TEXT("");
	}

	DWORD	dwDataSize = 0;
	DWORD	dwType = REG_MULTI_SZ; 

	if(RegQueryValueEx(m_hkey, pcszKey, 0, &dwType, (PBYTE)NULL, &dwDataSize) == ERROR_SUCCESS)
	{
		CStringW strValue;

		if(RegQueryValueEx(m_hkey, pcszKey, 0, &dwType, (PBYTE)(LPTSTR)strValue.GetBuffer(dwDataSize + 1), &dwDataSize) == ERROR_SUCCESS)
		{
			strValue.ReleaseBuffer();
			return strValue;
		}
		else
		{
			strValue.ReleaseBuffer();
		}
	}

	// Write the default value (if any)  Note: If there was a value in the registry, it
	// would already have been written
	if(AfxIsValidString(pcszDefault))
	{
		if(RegSetValueEx(m_hkey, (LPCTSTR)pcszKey, 0, REG_MULTI_SZ, (PBYTE) (LPCTSTR)pcszDefault, (_tcslen(pcszDefault) + 1) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			//TRACE(TEXT("CTlsRegistry(%s)::ReadString() Unable to write value %s to variable %s\n"), m_strSubkey, pcszDefault, pcszKey);
		}
		return pcszDefault;
	}
	else
		return TEXT("");
}

//=============================================================================
bool CTlsRegistry::GetFirstSubkey(CStringW &strSubkey)
{
	DWORD	dwNameSize = MAX_PATH;

	m_nSubkeyIndex = 0;

	if(RegEnumKeyEx(m_hkey, m_nSubkeyIndex, strSubkey.GetBuffer(dwNameSize), &dwNameSize, 0, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		strSubkey.ReleaseBuffer();
		return FALSE;	// No subkeys
	}

	strSubkey.ReleaseBuffer();
	return TRUE;
}

//=============================================================================
bool CTlsRegistry::GetNextSubkey(CStringW &strSubkey)
{
	DWORD	dwNameSize = MAX_PATH;

	m_nSubkeyIndex++;

	if(RegEnumKeyEx(m_hkey, m_nSubkeyIndex, strSubkey.GetBuffer(dwNameSize), &dwNameSize, 0, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		strSubkey.ReleaseBuffer();
		return FALSE;	// No more subkeys
	}

	strSubkey.ReleaseBuffer();
	return TRUE;
}

//=============================================================================
bool CTlsRegistry::GetFirstValue(CStringW& strSubkey)
{
	DWORD	dwNameSize = MAX_PATH;

	m_nSubkeyIndex = 0;

	if(RegEnumValue(m_hkey, m_nSubkeyIndex, strSubkey.GetBuffer(dwNameSize), &dwNameSize, 0, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		strSubkey.ReleaseBuffer();
		return FALSE;	// No subkeys
	}

	strSubkey.ReleaseBuffer();
	return TRUE;
}

//=============================================================================
int CTlsRegistry::GetNumValues()
{
    DWORD dwKeys;
    DWORD dwValues;
    LONG l = RegQueryInfoKey(m_hkey, NULL, NULL, NULL, &dwKeys, NULL, NULL, &dwValues, NULL, NULL, NULL, NULL);

    return dwValues;
}

//=============================================================================
bool CTlsRegistry::DeleteValue(CStringW sKey)
{
    DWORD dw;
    dw = RegDeleteValue(m_hkey, sKey);
    return true;
}

//=============================================================================
bool CTlsRegistry::GetNextValue(CStringW& strSubkey)
{
	DWORD	dwNameSize = MAX_PATH;

	m_nSubkeyIndex++;

	if(RegEnumValue(m_hkey, m_nSubkeyIndex, strSubkey.GetBuffer(dwNameSize), &dwNameSize, 0, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		strSubkey.ReleaseBuffer();
		return FALSE;	// No more subkeys
	}

	strSubkey.ReleaseBuffer();
	return TRUE;
}

//=============================================================================
unsigned int CTlsRegistry::HexDigit(wchar_t c)
{
  if(c >= '0' && c <= '9')
    return c - '0';
  if(c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if(c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  
  ASSERT(true);
  return 0; // dummy
}

//=============================================================================
int CTlsRegistry::Import(CStringW sFileName)
{
  int iSuccess = 0;
  CTlsIStream_File is(sFileName);
  CStringW sCurrKey;
  while(!is.IsEof())
  {
    CStringW sLine;
    is.ReadLine(sLine);
    
    sLine.TrimLeft();
    sLine.TrimRight();
    
    if(sLine.IsEmpty())
      continue;
    if(sLine == "")
      continue;
    if(sLine.GetAt(0) == ';')
      continue;
    
    // e.g. [HKEY_CURRENT_USER\Software\AIS\scanner\Settings]
    
    if(sLine.GetAt(0) == '[')
    {
      sCurrKey = sLine.Mid(1, sLine.GetLength() - 2);
      continue;
    }
    
    // e.g. "ClientName"="BURSP00.1021"
    int idx = sLine.Find(L"=");
    if(idx != -1)
    {
      CStringW sLeft = sLine.Left(idx-1);
	  sLeft.TrimLeft();
      sLeft.TrimRight();
      if(sLeft.GetAt(0) == '\"')
        sLeft = sLeft.Right(sLeft.GetLength()-1);
      if(sLeft.GetAt(sLeft.GetLength()-1) == '\"')
        sLeft = sLeft.Left(sLeft.GetLength()-1);

	  
      
      CStringW sRight = sLine.Right(sLine.GetLength()-idx-1);
      
	  if(sLeft==_T("#delValue"))
	  {
		  sRight = sRight.Right(sRight.GetLength()- sRight.Find(L"\"") - 1);
		  sRight = sRight.Left(sRight.Find(L"\"") );
		  sRight.Replace(L"\\\\", L"\\");
		  CTlsRegistry reg(sCurrKey);
		  if(!reg.DeleteValue(sRight))
			  iSuccess = -1;
	  }
	  else if(sLeft==_T("#delKey"))
	  {
		  CTlsRegistry reg(sCurrKey);
		  sRight = sRight.Right(sRight.GetLength()- sRight.Find(L"\"") - 1);
		  sRight = sRight.Left(sRight.Find(L"\"") );
		  sRight.Replace(L"\\\\", L"\\");
		  if(!reg.DeleteKey(sRight))
			  iSuccess = -1;
	  }
	  else if(sLeft==_T("#delNode"))
	  {
		  CTlsRegistry reg(sCurrKey);
		  sRight = sRight.Right(sRight.GetLength()- sRight.Find(L"\"") - 1);
		  sRight = sRight.Left(sRight.Find(L"\"") );
		  sRight.Replace(L"\\\\", L"\\");
		  LPTSTR pName = (LPTSTR) (LPCTSTR)sRight;
		  if(!reg.RegDelnode(pName))
			  iSuccess = -1;
	  }
      // e.g. "AutoCnct"=dword:00000000
      else if(sRight.Find(L"dword:") != -1)
      {
        //CStringW sNo = sRight.Right(sRight.GetLength()-6);
		CStringW sNo = sRight.Right(sRight.GetLength()- sRight.Find(L":") - 1);
		sNo.TrimLeft();
        sNo.TrimRight();
        
        DWORD dw = 0;
        dw |= HexDigit(sNo.GetAt(0)) << 28;
        dw |= HexDigit(sNo.GetAt(1)) << 24;
        dw |= HexDigit(sNo.GetAt(2)) << 20;
        dw |= HexDigit(sNo.GetAt(3)) << 16;
        dw |= HexDigit(sNo.GetAt(4)) << 12;
        dw |= HexDigit(sNo.GetAt(5)) <<  8;
        dw |= HexDigit(sNo.GetAt(6)) <<  4;
        dw |= HexDigit(sNo.GetAt(7));
        
        CTlsRegistry reg(sCurrKey);
        if(!reg.WriteDWORD(sLeft, dw))
          iSuccess = -1;
      }
      
      // e.g. "4"=hex:\
      //           80,fe,ca,22,8b,bc,c2,01
      else if(sRight.Find(L"hex:") != -1)
      {
        //CStringW sBlob = sRight.Right(sRight.GetLength()-4);
		CStringW sBlob = sRight.Right(sRight.GetLength()- sRight.Find(L":") - 1);
        sBlob.TrimLeft();
        sBlob.TrimRight();
        
        bool bContinue = false;
        if(sBlob.GetAt(sBlob.GetLength()-1) == '\\')
        {
          sBlob = sBlob.Left(sBlob.GetLength()-1);
          bContinue = true;
        }
        
        while(bContinue)
        {
          CStringW sL;
          is.ReadLine(sL);
          sL.TrimLeft();
          sL.TrimRight();
          if(sL.GetAt(sL.GetLength()-1) == '\\')
            sL = sL.Left(sL.GetLength()-1);
          else
            bContinue = false;
          sBlob += sL;
        }
        
        CTlsBlob bl;
        unsigned char uiCurrbyte = 0;
        for(int idx = 0; idx < sBlob.GetLength(); idx++)
        {
          wchar_t c = sBlob.GetAt(idx);
          if(c == ' ' || c == '\t')
            continue;
          if(c == L',')
          {
            bl.Append(&uiCurrbyte, 1);
            uiCurrbyte = 0;
            continue;
          }
          int i = HexDigit(c);
          uiCurrbyte = (uiCurrbyte << 4) | (unsigned char) i;
        }
        bl.Append(&uiCurrbyte, 1);
        
        CTlsRegistry reg(sCurrKey);
        if(!reg.WriteBlob(sLeft, bl))
          iSuccess = -1;
      }
	  /*else if(sRight.Find(L"hex(3):") != -1)
      {
        //CStringW sBlob = sRight.Right(sRight.GetLength()-4);
		CStringW sBlob = sRight.Right(sRight.GetLength()- sRight.Find(L":") - 1);
        sBlob.TrimLeft();
        sBlob.TrimRight();
        
        bool bContinue = false;
        if(sBlob.GetAt(sBlob.GetLength()-1) == '\\')
        {
          sBlob = sBlob.Left(sBlob.GetLength()-1);
          bContinue = true;
        }
        
        while(bContinue)
        {
          CStringW sL;
          is.ReadLine(sL);
          sL.TrimLeft();
          sL.TrimRight();
          if(sL.GetAt(sL.GetLength()-1) == '\\')
            sL = sL.Left(sL.GetLength()-1);
          else
            bContinue = false;
          sBlob += sL;
        }
        
        CTlsBlob bl;
        unsigned char uiCurrbyte = 0;
        for(int idx = 0; idx < sBlob.GetLength(); idx++)
        {
          wchar_t c = sBlob.GetAt(idx);
          if(c == ' ' || c == '\t')
            continue;
          if(c == L',')
          {
            bl.Append(&uiCurrbyte, 1);
            uiCurrbyte = 0;
            continue;
          }
          int i = HexDigit(c);
          uiCurrbyte = (uiCurrbyte << 4) | i;
        }
        bl.Append(&uiCurrbyte, 1);
        
        CTlsRegistry reg(sCurrKey);
        if(!reg.WriteBlob(sLeft, bl))
          iSuccess = -1;
      }*/
	  else if (sRight.Find(L"hex(7):") != -1)
      {
        CStringW sBlob = sRight.Right(sRight.GetLength()- sRight.Find(L":") - 1);
        sBlob.TrimLeft();
        sBlob.TrimRight();

		

        bool bContinue = false;

        if(sBlob.GetAt(sBlob.GetLength()-1) == '\\')
        {
          sBlob = sBlob.Left(sBlob.GetLength()-1);

          bContinue = true;
        }

		

        while(bContinue)
        {
          CStringW sL;

          is.ReadLine(sL);
          sL.TrimLeft();
          sL.TrimRight();

          if(sL.GetAt(sL.GetLength()-1) == '\\')
            sL = sL.Left(sL.GetLength()-1);
          else
            bContinue = false;

          sBlob += sL;
        }


        CTlsBlob bl;
        unsigned char uiCurrbyte = 0;

        for(int idx = 0; idx < sBlob.GetLength(); idx++)
        {
          wchar_t c = sBlob.GetAt(idx);

          if(c == ' ' || c == '\t')
            continue;

          if(c == L',')
          {
            bl.Append(&uiCurrbyte, 1);
            uiCurrbyte = 0;

            continue;
          }
          int i = HexDigit(c);

          uiCurrbyte = (uiCurrbyte << 4) | (unsigned char) i;
        }
        bl.Append(&uiCurrbyte, 1);

		//MessageBox(NULL, bl.GetString() , L"4 hex7", MB_OK);

        CTlsRegistry reg(sCurrKey);

        if(!reg. WriteMultiString (sLeft, bl))
		{
			
          iSuccess = -1;
		}
		
      }
      else
      {
        sRight = sRight.Right(sRight.GetLength()- sRight.Find(L"\"") - 1);
		sRight = sRight.Left(sRight.Find(L"\"") );

        //if(sRight.GetAt(0) == '\"')
        //  sRight = sRight.Right(sRight.GetLength()-1);
		//if(sRight.GetAt(sRight.GetLength()-1) == '\"')
        //  sRight = sRight.Left(sRight.GetLength()-1);

		
        
		sRight.Replace(L"\\\\", L"\\");

        CTlsRegistry reg(sCurrKey);
        if(!reg.WriteString(sLeft, sRight))
          iSuccess = -1;
      }
      
    }
	//if(iSuccess != 0)
	//	MessageBox(NULL, sLine, L"Fehler Zeile", MB_OK);
  }

  

  FlushHKCU();
  FlushHKLM();
  
  return iSuccess;
}

//=============================================================================
CTlsBlob CTlsRegistry::ReadBlob(CStringW sKey, CTlsBlob bin)
{
	byte *dwValue = NULL;
	DWORD dwValueSize = 0;

	DWORD dwType = REG_BINARY;

	LONG l = RegQueryValueEx(m_hkey, sKey, 0, &dwType, (PBYTE)dwValue, &dwValueSize);

	switch(l) 
	{
	case ERROR_SUCCESS:
		{
			dwValue = new byte[dwValueSize];
			LONG l = RegQueryValueEx(m_hkey, sKey, 0, &dwType, (PBYTE)dwValue, &dwValueSize);

			CTlsBlob ret(dwValue,dwValueSize);

			delete [] dwValue;
			return ret;
		}

		break;

	case ERROR_MORE_DATA:
		{
			// Sollte nicht passieren
		}

	default:
		break;
	}

	return bin;
}

//=============================================================================
CStringW CTlsRegistry::GetSubkey()
{
	return m_strSubkey;
}

//=============================================================================
HKEY CTlsRegistry::GetParentKey()
{
	return m_hkeyParent;
}

//=============================================================================
bool CTlsRegistry::DeleteKey(CStringW sKey)
{
    DWORD dw;
    dw = RegDeleteKey(m_hkey, sKey);
	if(dw!=ERROR_SUCCESS)
	{
		return false;
	}
    return true;
}

/**************************************************************/
//
//  RegDelnodeRecurse()
//
//  Purpose:    Deletes a registry key and all its subkeys / values.
//
//  Parameters: hKeyRoot    -   Root key
//              lpSubKey    -   SubKey to delete
//
//  Return:     TRUE if successful.
//              FALSE if an error occurs.
//
/**************************************************************/

BOOL CTlsRegistry::RegDelnodeRecurse (LPTSTR lpSubKey)
{
	LPTSTR lpEnd;
	LONG lResult;
	DWORD dwSize;
	TCHAR szName[MAX_PATH];
	HKEY hKey;
	FILETIME ftWrite;

	// First, see if we can delete the key without having
	// to recurse.

	lResult = RegDeleteKey(m_hkey, lpSubKey);

	if (lResult == ERROR_SUCCESS) 
		return TRUE;

	lResult = RegOpenKeyEx (m_hkey, lpSubKey, 0, KEY_READ, &hKey);

	if (lResult != ERROR_SUCCESS) 
	{
		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("Key not found.\n");
			return TRUE;
		} 
		else {
			printf("Error opening key.\n");
			return FALSE;
		}
	}

	// Check for an ending slash and add one if it is missing.

	lpEnd = lpSubKey + lstrlen(lpSubKey);

	if (*(lpEnd - 1) != TEXT('\\')) 
	{
		*lpEnd =  TEXT('\\');
		lpEnd++;
		*lpEnd =  TEXT('\0');
	}

	// Enumerate the keys

	dwSize = MAX_PATH;
	lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
		NULL, NULL, &ftWrite);

	if (lResult == ERROR_SUCCESS) 
	{
		do {

			StringCchCopy (lpEnd, MAX_PATH*2, szName);

			if (!RegDelnodeRecurse(lpSubKey)) {
				break;
			}

			dwSize = MAX_PATH;

			lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
				NULL, NULL, &ftWrite);

		} while (lResult == ERROR_SUCCESS);
	}

	lpEnd--;
	*lpEnd = TEXT('\0');

	RegCloseKey (hKey);

	// Try again to delete the key.

	lResult = RegDeleteKey(m_hkey, lpSubKey);

	if (lResult == ERROR_SUCCESS) 
		return TRUE;

	return FALSE;
}

/**************************************************************/
//
//  RegDelnode()
//
//  Purpose:    Deletes a registry key and all its subkeys / values.
//
//  Parameters: hKeyRoot    -   Root key
//              lpSubKey    -   SubKey to delete
//
//  Return:     TRUE if successful.
//              FALSE if an error occurs.
//
/**************************************************************/

BOOL CTlsRegistry::RegDelnode (LPTSTR lpSubKey)
{
	TCHAR szDelKey[MAX_PATH*2];

	StringCchCopy (szDelKey, MAX_PATH*2, lpSubKey);
	return RegDelnodeRecurse(szDelKey);

}

/*=============================================================================
    E O F
=============================================================================*/
