// RegKey.h: interface for the CRegKey class.
//
/////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////////////////
// CRegKey
/////////////////////////////////////////////////////////////////////////////////////////

class CRegKey
{
public:
	CRegKey();
	~CRegKey();

// Attributes
public:
	operator HKEY() const;
	HKEY m_hKey;
	TCHAR chRegPath[200];

// Operations
public:
	LONG Open(HKEY hKeyParent, LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_ALL_ACCESS);
	LONG Open(HKEY hKeyParent, LPCSTR lpszKeyName,
		REGSAM samDesired = KEY_ALL_ACCESS);
	LONG Close();
	LONG EnumKey(DWORD dwIndex, TCHAR* pstrName);
	LONG EnumKey(DWORD dwIndex, char* pstrName);
	LONG EnumValue(DWORD dwIndex, TCHAR* strName, DWORD* dwType);
	LONG EnumValue(DWORD dwIndex, char* strName, DWORD* dwType);
	LONG EnumValue(DWORD dwIndex, TCHAR* strName, DWORD* dwType, LPBYTE lpData, LPDWORD lpcbData );
	LONG EnumValue(DWORD dwIndex, char* strName, DWORD* dwType, LPBYTE lpData, LPDWORD lpcbData );
#if 0
	BOOL QueryType(CStringW strValue, DWORD* dwType);
#endif
	LONG QueryValue(LPCTSTR strName, DWORD& dwValue);
	LONG QueryValue(LPCSTR strName, DWORD& dwValue);
	LONG QueryValue(LPCTSTR strName, TCHAR* strValue);
#if 0
	LONG QueryValue(CStringW strName, LPBYTE buffer, DWORD& dwCount);
#endif
	LONG SetValue(LPCTSTR strName, const DWORD dwValue);
	LONG SetValue(LPCTSTR strName, LPCTSTR lpszValue);
	LONG SetValue(LPCTSTR strName, LPBYTE buffer, DWORD dwCount);
	LONG SetValueMulti(LPCTSTR strName, LPCTSTR strValue);
	LONG RecurseDeleteKey(LPCTSTR lpszKey);
#if 0
	BOOL RenameChild(CStringW strOld, CStringW strNew);
	BOOL RenameValue(CStringW strOld, CStringW strNew);
#endif
	BOOL Duplicate(CRegKey& keyDest);

	LONG Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		LPDWORD lpdwDisposition = NULL);
	HKEY Detach();
	void Attach(HKEY hKey);
	LONG DeleteSubKey(LPCTSTR lpszSubKey);
	LONG DeleteValue(LPCTSTR lpszValue);
};

inline CRegKey::CRegKey()
{m_hKey = NULL;}

inline CRegKey::~CRegKey()
{Close();}

inline CRegKey::operator HKEY() const
{return m_hKey;}

inline LONG CRegKey::Close()
{
	LONG lRes = ERROR_SUCCESS;
	if (m_hKey != NULL)
	{
		lRes = RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
	return lRes;
}

inline LONG CRegKey::Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired)
{
	ASSERT(hKeyParent != NULL);
	HKEY hKey = NULL;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		ASSERT(lRes == ERROR_SUCCESS);
		m_hKey = hKey;

		_tcscpy( chRegPath, lpszKeyName );
	}
	return lRes;
}
inline LONG CRegKey::Open(HKEY hKeyParent, LPCSTR lpszKeyName, REGSAM samDesired)
{
	TCHAR suKeyName[100];

	suKeyName[0] = 0;

	if( lpszKeyName[0] != 0 )
	{
		// character string in unicode string umwandeln
		MultiByteToWideChar(
		  CP_ACP,         // code page
		  MB_PRECOMPOSED,         // character-type options
		  lpszKeyName, // address of string to map
		  strlen(lpszKeyName)+1,      // number of bytes in string
		  suKeyName,  // address of wide-character buffer
		  100        // size of buffer
		);
	}
	return Open(hKeyParent, suKeyName, samDesired);
}
inline LONG CRegKey::EnumKey(DWORD dwIndex, TCHAR* pstrName)
{
	DWORD dwCount = _MAX_PATH;
	LONG lResult = ::RegEnumKeyEx(m_hKey, dwIndex++,
		pstrName, &dwCount, NULL, NULL, NULL, NULL);

	return lResult;
}
inline LONG CRegKey::EnumKey(DWORD dwIndex, char* pstrName)
{
	TCHAR suKey[100];

	LONG lResult = EnumKey( dwIndex, suKey );
	if( lResult != ERROR_SUCCESS )
		return lResult;

	WideCharToMultiByte(
	  CP_ACP,         // code page
	  0,         // performance and mapping flags
	  suKey, // address of wide-character string
	  _tcslen(suKey)+1,       // number of characters in string
	  pstrName,  // address of buffer for new string
	  100,      // size of buffer
	  NULL,// address of default for unmappable 
							 // characters
	  NULL// address of flag set when default 
								 // char. used
	);
 
	return ERROR_SUCCESS;
}

inline LONG CRegKey::EnumValue(DWORD dwIndex, TCHAR* strName, DWORD* dwType, LPBYTE lpData, LPDWORD lpcbData )
{
	DWORD dwCount = _MAX_PATH;
	LONG lRes = ::RegEnumValue(m_hKey, dwIndex++, strName,
		&dwCount, NULL, dwType, lpData, lpcbData );

	ASSERT(dwCount <= _MAX_PATH);

	if( lRes == ERROR_NO_MORE_ITEMS )
		dwIndex = 0;

	return lRes;
}

inline LONG CRegKey::EnumValue(DWORD dwIndex, TCHAR* strName, DWORD* dwType)
{
	DWORD dwCount = _MAX_PATH;
	LONG lRes = ::RegEnumValue(m_hKey, dwIndex++, strName,
		&dwCount, NULL, dwType, NULL, NULL);

	ASSERT(dwCount <= _MAX_PATH);

	if( lRes == ERROR_NO_MORE_ITEMS )
		dwIndex = 0;

	return lRes;
}

inline LONG CRegKey::EnumValue(DWORD dwIndex, char* pstrName, DWORD* dwType)
{
	TCHAR suKey[100];

	LONG lResult = EnumValue( dwIndex, suKey, dwType );
	if( lResult != ERROR_SUCCESS )
		return lResult;

	if( *dwType == REG_SZ )
	{
		WideCharToMultiByte(
		  CP_ACP,         // code page
		  0,         // performance and mapping flags
		  suKey, // address of wide-character string
		  _tcslen(suKey)+1,       // number of characters in string
		  pstrName,  // address of buffer for new string
		  100,      // size of buffer
		  NULL,// address of default for unmappable 
								 // characters
		  NULL// address of flag set when default 
									 // char. used
		);
	}

	return ERROR_SUCCESS;
}

inline LONG CRegKey::EnumValue(DWORD dwIndex, char* pstrName, DWORD* dwType, LPBYTE pData, LPDWORD lpcbData )
{
	TCHAR suKey[100];

	LONG lResult = EnumValue( dwIndex, suKey, dwType, pData, lpcbData );
	if( lResult != ERROR_SUCCESS )
		return lResult;

	WideCharToMultiByte(
	  CP_ACP,         // code page
	  0,         // performance and mapping flags
	  suKey, // address of wide-character string
	  _tcslen(suKey)+1,       // number of characters in string
	  pstrName,  // address of buffer for new string
	  100,      // size of buffer
	  NULL,// address of default for unmappable 
							 // characters
	  NULL// address of flag set when default 
								 // char. used
	);

	return ERROR_SUCCESS;
}

inline LONG CRegKey::QueryValue(LPCTSTR strName, DWORD& dwValue)
{
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	LONG lRes = RegQueryValueEx(m_hKey, strName, NULL, &dwType,
		(LPBYTE)&dwValue, &dwCount);
	ASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_DWORD));
	ASSERT((lRes!=ERROR_SUCCESS) || (dwCount == sizeof(DWORD)));
	return lRes;
}
inline LONG CRegKey::QueryValue(LPCSTR strName, DWORD& dwValue)
{
	TCHAR suName[100];

	// character string in unicode string umwandeln
	MultiByteToWideChar(
	  CP_ACP,         // code page
	  MB_PRECOMPOSED,         // character-type options
	  strName, // address of string to map
	  strlen(strName)+1,      // number of bytes in string
	  suName,  // address of wide-character buffer
	  100        // size of buffer
	);

	return QueryValue( suName, dwValue);
}

inline LONG CRegKey::QueryValue(LPCTSTR strName, TCHAR* strValue)
{
	DWORD dwType = NULL, dwCount = _MAX_PATH;
	LONG lRes = RegQueryValueEx(m_hKey, strName, NULL, &dwType,
		(LPBYTE) strValue, &dwCount);

	ASSERT(dwCount <= _MAX_PATH);
	ASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_SZ) ||
			 (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ));
	return lRes;
}

#if 0
inline LONG CRegKey::QueryValue(CStringW strName, LPBYTE buffer, DWORD& dwCount)
{
	DWORD dwType = NULL, dwOrgCount = dwCount;
	LONG lRes = RegQueryValueEx(m_hKey, strName, NULL,
		&dwType, buffer, &dwCount);

	ASSERT(dwCount <= dwOrgCount);
	ASSERT(lRes != ERROR_SUCCESS || dwType == REG_BINARY);
	return lRes;
}

inline BOOL CRegKey::RenameChild(CStringW strOld, CStringW strNew)
{
	ASSERT(m_hKey != NULL);

	CRegKey keyOld, keyNew;
	VERIFY( keyOld.Open(m_hKey, strOld) == ERROR_SUCCESS );
	if (keyNew.Create(m_hKey, strNew) != ERROR_SUCCESS)
		return FALSE;

	if (keyOld.Duplicate(keyNew))
	{
		keyOld.Close();
		RecurseDeleteKey(strOld);
		return TRUE;
	}
	else
	{
		keyNew.Close();
		RecurseDeleteKey(strNew);
		return FALSE;
	}
}

inline BOOL CRegKey::QueryType(CStringW strValue, DWORD* dwType)
{
	return ::RegQueryValueEx(m_hKey, strValue, NULL,
			dwType, NULL, NULL) == ERROR_SUCCESS;
}

inline BOOL CRegKey::RenameValue(CStringW strOld, CStringW strNew)
{
	if (strOld == strNew)
		return TRUE;

	if (::RegQueryValueEx(m_hKey, strNew, NULL,
			NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		return FALSE;
	}

	DWORD dwValType, dwCount = _MAX_PATH;
	LPBYTE lpData = new BYTE[dwCount];
	if (::RegQueryValueEx(m_hKey, strOld, NULL,
		&dwValType, lpData, &dwCount) != ERROR_SUCCESS)
	{
		delete [] lpData;
		return FALSE;
	}

	if (::RegSetValueEx(m_hKey, strNew, NULL,
		dwValType, lpData, dwCount) != ERROR_SUCCESS)
	{
		delete [] lpData;
		return FALSE;
	}

	DeleteValue(strOld);
	return TRUE;
}

inline BOOL CRegKey::Duplicate(CRegKey& keyDest)
{
	DWORD dwIndex = 0;
	CStringW strName;
	while (EnumKey(dwIndex, strName) == ERROR_SUCCESS)
	{
		CRegKey keySubSrc, keySubDest;
		if (keySubSrc.Open(m_hKey, strName) != ERROR_SUCCESS)
			return FALSE;

		keySubDest.Create(keyDest, strName);
		if (!keySubSrc.Duplicate(keySubDest))
			return FALSE;
	}

	DWORD dwKeyType, dwNameLen = _MAX_PATH;
	LPTSTR lpValueName = new TCHAR[dwNameLen];
	dwIndex = 0;

	while (::RegEnumValue(m_hKey, dwIndex++, lpValueName,
		&dwNameLen, NULL, &dwKeyType, NULL, NULL) == ERROR_SUCCESS)
	{
		DWORD dwValType, dwCount = _MAX_PATH;
		LPBYTE lpData = new BYTE[dwCount];
		if (::RegQueryValueEx(m_hKey, lpValueName, NULL,
			&dwValType, lpData, &dwCount) != ERROR_SUCCESS)
		{
			delete [] lpData;
			return FALSE;
		}

		if (::RegSetValueEx(keyDest, lpValueName, NULL,
			dwValType, lpData, dwCount) != ERROR_SUCCESS)
		{
			delete [] lpData;
			return FALSE;
		}

		dwNameLen = _MAX_PATH;
		delete [] lpData;
	}

	delete [] lpValueName;
	return TRUE;
}

inline HKEY CRegKey::Detach()
{
	HKEY hKey = m_hKey;
	m_hKey = NULL;
	return hKey;
}

inline void CRegKey::Attach(HKEY hKey)
{
	ASSERT(m_hKey == NULL);
	m_hKey = hKey;
}
#endif
inline LONG CRegKey::DeleteSubKey(LPCTSTR lpszSubKey)
{
	ASSERT(m_hKey != NULL);
	return RegDeleteKey(m_hKey, lpszSubKey);
}

inline LONG CRegKey::DeleteValue(LPCTSTR lpszValue)
{
	ASSERT(m_hKey != NULL);
	return RegDeleteValue(m_hKey, (LPTSTR)lpszValue);
}

inline LONG CRegKey::Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
	LPTSTR lpszClass, DWORD dwOptions, REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecAttr, LPDWORD lpdwDisposition)
{
	ASSERT(hKeyParent != NULL);
	DWORD dw;
	HKEY hKey = NULL;
	LONG lRes = RegCreateKeyEx(hKeyParent, lpszKeyName, 0,
		lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, &dw);
	if (lpdwDisposition != NULL)
		*lpdwDisposition = dw;
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		m_hKey = hKey;
	}
	return lRes;
}

inline LONG CRegKey::SetValue(LPCTSTR strName, const DWORD dwValue)
{
	ASSERT(m_hKey != NULL);
	return RegSetValueEx(m_hKey, strName, NULL, REG_DWORD,
		(BYTE * const)&dwValue, sizeof(DWORD));
}

inline LONG CRegKey::SetValue(LPCTSTR strName, LPCTSTR strValue)
{
	ASSERT(m_hKey != NULL);
	LONG lRes = RegSetValueEx(m_hKey, strName, NULL, REG_SZ,
		(LPBYTE) strValue,
		(_tcslen(strValue) + 1) * sizeof(TCHAR));

	return lRes;
}

inline LONG CRegKey::SetValueMulti(LPCTSTR strName, LPCTSTR strValue)
{
	const TCHAR* p = strValue;

	// search for terminating double NULL
	// total string length plus 2* NULL
	int nLen = 2;
	while( *p || *(p+1) )
	{
		p++;
		nLen ++;
	}

	ASSERT(m_hKey != NULL);
	LONG lRes = RegSetValueEx(m_hKey, strName, NULL, REG_MULTI_SZ,
		(LPBYTE) strValue,
		nLen * sizeof(TCHAR));

	return lRes;
}

inline LONG CRegKey::SetValue(LPCTSTR strName, LPBYTE lpBuffer, DWORD dwCount)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpBuffer != NULL);
	return RegSetValueEx(m_hKey, strName, NULL, REG_BINARY,
		lpBuffer, dwCount);
}

inline LONG CRegKey::RecurseDeleteKey(LPCTSTR lpszKey)
{
	CRegKey key;
	LONG lRes = key.Open(m_hKey, lpszKey, KEY_READ | KEY_WRITE);
	if (lRes != ERROR_SUCCESS)
		return lRes;
	FILETIME time;
	DWORD dwSize = 256;
	TCHAR szBuffer[256];
	while (RegEnumKeyEx(key.m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time)==ERROR_SUCCESS)
	{
		lRes = key.RecurseDeleteKey(szBuffer);
		if (lRes != ERROR_SUCCESS)
			return lRes;
		dwSize = 256;
	}
	key.Close();
	return DeleteSubKey(lpszKey);
}
