
#ifndef __TLS_COMPORT_H__
#define __TLS_COMPORT_H__

//#include "SYSTEM.H"


//extern const char cSYN;
//const char cACK = 0x06;

#include "tlsblob.h"

class CTlsComPort
{
private:                
    

public:                 
  CTlsComPort();
  ~CTlsComPort();

  enum eType {TP_BDR, TP_GARMIN, TP_VIRTUAL};
  DWORD		 Open(DWORD dwIndex,DWORD dwBaudrate);
  void		 Close ();

  DWORD		 Baud;

  DWORD		 Read(void *pData ,DWORD iData);
  DWORD      ReadLine(char* pData,DWORD iData);
  DWORD       Read(CTlsBlob &data);
  DWORD		 ReadCommand(char *pData , DWORD iData);
  DWORD		ReadCO2Command(BYTE *pData , DWORD iData);

  DWORD		 Write (const void *pData,DWORD len);
  DWORD		 Write (CStringW data);
  bool		 Write(CTlsBlob &data); 
  
  BOOL		FlushBuffer();

  bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);
  //static ULONGLONG getTickCount64();

  HANDLE	 m_hCommPort;

};

#endif
/*=============================================================================
    E O F
=============================================================================*/
