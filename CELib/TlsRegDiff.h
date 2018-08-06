
#if !defined(__TLSREGDIFF_H__)
#define __TLSREGDIFF_H__

#include "TlsStream.h"

class CTlsRegDiff  
{
  char TypeToChar(DWORD dwType);

  void WriteOut(CTlsOStream_Base& rOut, CStringW sPath, HKEY hkCurrent);
  void WriteHexByte(CTlsOStream_Base& rOut, int iByte);
  

  void Compare(CMapStringToPtr& map, CStringW sPath, HKEY hkCurrent, CTlsOStream_Base& rDiff);
    
public:
	CTlsRegDiff();
	virtual ~CTlsRegDiff();

	void WriteKey(CTlsOStream_Base& rOut, CStringW sPath, CStringW sKey, DWORD dwType, BYTE* szData, DWORD dwData);

  void RegDump(CTlsOStream_Base& rOut);
  void RegDiff(CTlsIStream_Base& rIn, CTlsOStream_Base& rDiff);

};

#endif
/*********************************************************************
  E O F
*********************************************************************/

