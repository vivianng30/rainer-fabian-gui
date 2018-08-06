
#include "TlsBlob.h"	// Added by ClassView

#if !defined(__TLSREGISTRY_H__)
#define __TLSREGISTRY_H__

class CTlsRegistry  
{
	int m_nSubkeyIndex;
	CStringW m_strSubkey;
	HKEY m_hkey;
	HKEY m_hkeyParent;

  void Init(HKEY hkeyParent, CStringW sSubkey, bool bWrite);

public:
	CTlsBlob ReadBlob(CStringW sKey, CTlsBlob bin);
	CTlsRegistry(CStringW sPath, bool bWrite = false);
	CTlsRegistry(HKEY hkeyParent, CStringW sSubkey, bool bWrite = false);
	CTlsRegistry(const CTlsRegistry& rSrc);	// Default Copy Constructor
	virtual ~CTlsRegistry();

	static void FlushHKLM();
	static void FlushHKCU();

	static bool IsValid(CStringW sPath);
	static bool IsValid(HKEY hkeyParent, CStringW pcszSubkey);

	bool WriteDWORD(CStringW sKey, DWORD dwValue);
	DWORD ReadDWORD(CStringW sKey, DWORD dwDefault = 0xDEF0);
	bool WriteString(CStringW sKey, CStringW sValue);
	CStringW ReadString(CStringW sKey, CStringW sDefault = L"");
	bool WriteMultiString(CStringW sKey, CTlsBlob bl);
	CStringW ReadMultiString(CStringW pcszKey, CStringW pcszDefault);

    bool WriteBlob(CStringW sKey, CTlsBlob bl);

	bool GetFirstSubkey(CStringW& strSubkey);
	bool GetNextSubkey(CStringW& strSubkey);
    int  GetNumValues();
    bool DeleteValue(CStringW sKey);

	bool GetFirstValue(CStringW& strSubkey);
	bool GetNextValue(CStringW& strSubkey);

	operator HKEY()	{ return m_hkey; }
	operator HKEY*() { return &m_hkey; }

	CStringW GetSubkey();
	HKEY GetParentKey();
	
	bool DeleteKey(CStringW sKey);
	BOOL RegDelnodeRecurse (LPTSTR lpSubKey);
	BOOL RegDelnode (LPTSTR lpSubKey);

  // Import Reg-Files
  static unsigned int HexDigit(wchar_t c);
  static int Import(CStringW sFileName);
};

#endif
/*********************************************************************
  E O F
*********************************************************************/
