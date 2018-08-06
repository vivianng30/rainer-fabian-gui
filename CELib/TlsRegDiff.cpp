
#include "stdafx.h"
#include "TlsRegDiff.h"
#include "TlsLanguage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#include "TlsDbg.h"
#define new DEBUG_NEW
#define malloc DEBUG_MALLOC
#define free DEBUG_FREE
#define realloc DEBUG_REALLOC
#endif

//#define USE_FLATE

//=============================================================================
CTlsRegDiff::CTlsRegDiff()
{

}

//=============================================================================
CTlsRegDiff::~CTlsRegDiff()
{

}

//=============================================================================
char CTlsRegDiff::TypeToChar(DWORD dwType)
{
  switch(dwType)
  {
  case REG_BINARY:  return 'B';
  case REG_DWORD:   return 'W';
  case REG_DWORD_BIG_ENDIAN:   return 'E';
  case REG_EXPAND_SZ:   return 'X';
  case REG_LINK:   return 'L';
  case REG_MULTI_SZ:   return 'M';
  case REG_NONE:   return 'N';
  case REG_RESOURCE_LIST:   return 'R';
  case REG_SZ:   return 'S';
  }

  return 'U'; // unknown
}

//=============================================================================
void CTlsRegDiff::WriteOut(CTlsOStream_Base& rOut, CStringW sPath, HKEY hkCurrent)
{
  // +++ write entries +++
  {
	  int nSubkeyIndex = 0;

    DWORD dwNameSize = 255;
    CStringW sKey;
    DWORD dwType = -1;
	CTlsBlob blData(4096);
    DWORD dwDataLen = 4096;

    if(RegEnumValue(hkCurrent, nSubkeyIndex, sKey.GetBuffer(dwNameSize), &dwNameSize, 0, &dwType, blData.GetBuffer(), &dwDataLen) == ERROR_SUCCESS)
    {
      sKey.ReleaseBuffer();

      while(true)
      {
        ASSERT(dwDataLen < 4096);
        
        rOut.Write(sPath);
        rOut.Write("\\");
        rOut.Write(sKey);
        rOut.WriteBYTE(0); // delim
        rOut.WriteBeDWORD(dwDataLen);
        rOut.Write(TypeToChar(dwType));
        rOut.Write((char*)blData.GetBuffer(), dwDataLen);

        // +++ next item +++
	      nSubkeyIndex++;

        dwNameSize = 255;
        dwDataLen = 4096;
	      if(RegEnumValue(hkCurrent, nSubkeyIndex, sKey.GetBuffer(dwNameSize), &dwNameSize, 0, &dwType, blData.GetBuffer(), &dwDataLen) != ERROR_SUCCESS)
	      {
		      sKey.ReleaseBuffer();
		      break;
	      }

	      sKey.ReleaseBuffer();
      }
    }
    else
      sKey.ReleaseBuffer();
  }

  // +++ recurse through the sub-keys +++
  {
	  int nSubkeyIndex = 0;

    DWORD dwNameSize = 255;
    CStringW sKey;
	  if(RegEnumKeyEx(hkCurrent, nSubkeyIndex, sKey.GetBuffer(dwNameSize), &dwNameSize, 0, NULL, NULL, NULL) == ERROR_SUCCESS)
    {
      sKey.ReleaseBuffer();

      while(true)
      {
        // +++ get data +++
        DWORD dwType = -1;
        unsigned char szData[4096];
        DWORD dwDataLen = 0;
        LONG lr = RegQueryValueEx(hkCurrent, sKey, NULL, &dwType, szData, &dwDataLen);

        // +++ open key +++
	      DWORD dwDisposition;
        HKEY hkChild;
	      RegCreateKeyEx(hkCurrent, sKey, 0, NULL, 0, 0, NULL, &hkChild, &dwDisposition);

        WriteOut(rOut, sPath + L"\\" + sKey, hkChild);

        // +++ close key +++
	      RegCloseKey(hkChild);

        // +++ next item +++
	      nSubkeyIndex++;

        dwNameSize = 255;
	      if(RegEnumKeyEx(hkCurrent, nSubkeyIndex, sKey.GetBuffer(dwNameSize), &dwNameSize, 0, NULL, NULL, NULL) != ERROR_SUCCESS)
	      {
		      sKey.ReleaseBuffer();
		      break;
	      }

	      sKey.ReleaseBuffer();
      }
    }
    else
      sKey.ReleaseBuffer();
  }
}

//=============================================================================
void CTlsRegDiff::RegDump(CTlsOStream_Base& rOut) 
{
  CWaitCursor wait;


#ifdef USE_FLATE
  CTlsOStream_Mem os;

  WriteOut(os, "L", HKEY_LOCAL_MACHINE);
  WriteOut(os, "U", HKEY_CURRENT_USER);
  WriteOut(os, "C", HKEY_CLASSES_ROOT);

  CTlsBlob blEncoded;
  CTlsFlate::Encode(os.GetBuffer(), blEncoded);

  rOut.Write(blEncoded);
#else
  WriteOut(rOut, L"L", HKEY_LOCAL_MACHINE);
  WriteOut(rOut, L"U", HKEY_CURRENT_USER);
  WriteOut(rOut, L"C", HKEY_CLASSES_ROOT);
#endif


  //TlsMessageBox(CStringW("Dump wurde erstellt"), MB_OK);
}

//=============================================================================
void CTlsRegDiff::WriteHexByte(CTlsOStream_Base& rOut, int iByte)
{
	/*
  int i1 = iByte >> 4;
  int i2 = iByte & 0x0f;

  if(i1 < 10)
  {
    rOut.Write(CStringW(CTlsInt::Format('0'+i1)));
  }
  else
    rOut.Write(CStringW(CTlsInt::Format(i1-10+'a')));

  if(i2 < 10)
    rOut.Write(CStringW(CTlsInt::Format('0'+i2)));
  else
    rOut.Write(CStringW(CTlsInt::Format(i2-10+'a')));
	*/
	CStringW temp;
	temp.Format(L"%02X",iByte);
	rOut.Write(temp);
}

//=============================================================================
void CTlsRegDiff::WriteKey(CTlsOStream_Base& rOut, CStringW sPath, CStringW sKey, DWORD dwType, BYTE* szData, DWORD dwData)
{
  /*
  Sample: 

  [HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows CE
  Services\Filters\.bmp\InstalledFilters]
  "{DA01ED80-97E8-11cf-8011-00A0C90A8F78}"=""

  [HKEY_CLASSES_ROOT\CLSID\{DA01ED80-97E8-11cf-8011-00A0C90A8F78}]
  @="Bitmap Image"
  */

  rOut.Write(CStringW("["));

  switch(sPath.GetAt(0))
  {
  case 'L':
    rOut.Write("HKEY_LOCAL_MACHINE");
    break;
  case 'U':
    rOut.Write("HKEY_CURRENT_USER");
    break;
  case 'C':
    rOut.Write("HKEY_CLASSES_ROOT");
    break;
  }

  rOut.Write(sPath.Right(sPath.GetLength()-1) + L"]\r\n");
  if(sKey.GetLength() == 0)
    rOut.Write("@=");
  else
    rOut.Write(CStringW("\"") + sKey + L"\"=");

  switch(dwType)
  {
  case REG_BINARY:
    {
      //"binary"=hex:\
      //    23,23,34,55,46,78
      rOut.Write("hex:\\\r\n      ");

      for(DWORD idx = 0; idx < dwData; idx++)
      {
        if(idx != 0)
          rOut.Write(",");
        if((idx % 25) == 0 && idx)
          rOut.Write("\\\r\n      ");
        WriteHexByte(rOut, szData[idx]);
      }
    }
    break;
  case REG_DWORD:
    {
      //"dword"=dword:00000014
      rOut.Write("dword:");
      WriteHexByte(rOut, szData[3]);  // Ist die Reihenfolge so richtig ??
      WriteHexByte(rOut, szData[2]);
      WriteHexByte(rOut, szData[1]);
      WriteHexByte(rOut, szData[0]);
      // write 4 hex-bytes
    }
    break;
  case REG_DWORD_BIG_ENDIAN:
    {
      rOut.Write("\"\""); // not supported
    }
    break;
  case REG_EXPAND_SZ:
    {
      rOut.Write("hex:\\\r\n      "); // eigentlich: hex(2)...
      for(DWORD idx = 0; idx < dwData; idx++)
      {
        if(idx != 0)
          rOut.Write(",");
        if((idx % 25) == 0 && idx)
          rOut.Write("\\\r\n      ");
        WriteHexByte(rOut, szData[idx]);
      }
    }
    break;
  case REG_LINK:
    {
      rOut.Write("\"\""); // not supported
    }
    break;
  case REG_MULTI_SZ:
    {
      // "MulitLine"=hex(7):\
      //    74,65,73,74,00,74,65,73,74,00,74,65,73,74,00,74,65,73,74,00,00,00
      rOut.Write("hex:\\\r\n      "); // hex(7) hat nicht funktioniert
      for(DWORD idx = 0; idx < dwData; idx++)
      {
        if(idx != 0)
          rOut.Write(",");
        if((idx % 25) == 0 && idx)
          rOut.Write("\\\r\n      ");
        WriteHexByte(rOut, szData[idx]);
      }
    }
    break;
  case REG_NONE:
    {
      rOut.Write("\"\""); // take empty...
    }
    break;
  case REG_RESOURCE_LIST:
    {
      rOut.Write("\"\""); // not supported
    }
    break;
  case REG_SZ:
    {
      // "noch"="testtest  " \\ ""

      rOut.Write("\"");
      char szBuffer[1024];
      int iLen = wcstombs( szBuffer, (const wchar_t*)szData, dwData); // WM5

      // todo --> replace \ by \\ and " by \"
      for(int idx = 0; idx < iLen; idx++)
      {
        if(szBuffer[idx] == '\\')
          rOut.Write("\\\\");
        else if(szBuffer[idx] == '\"')
          rOut.Write("\\\""); // not documented !!!
        else
          rOut.Write(szBuffer+idx, 1);
      }
      rOut.Write("\"");
    }
    break;
  default:
    {
      rOut.Write("\"\""); // not supported
    }
    break;
  }

  rOut.Write("\r\n\r\n");
}

//=============================================================================
void CTlsRegDiff::Compare(CMapStringToPtr& map, CStringW sPath, HKEY hkCurrent, CTlsOStream_Base& rDiff)
{
//  CWaitCursor wait;

  // +++ write entries +++
  {
	  int nSubkeyIndex = 0;

    DWORD dwNameSize = 255;
    CStringW sKey;
    DWORD dwType = -1;
    CTlsBlob blData(4096);
    DWORD dwDataLen = 4096;

    if(RegEnumValue(hkCurrent, nSubkeyIndex, sKey.GetBuffer(dwNameSize), &dwNameSize, 0, &dwType, blData.GetBuffer(), &dwDataLen) == ERROR_SUCCESS)
    {
      sKey.ReleaseBuffer();

      while(true)
      {
        // +++ compare entries +++
        void* pV = NULL;
        bool bFound = map.Lookup(sPath + L"\\" + sKey, pV) != 0;
        CTlsBlob* pBl = (CTlsBlob*)pV;
        if(!bFound)
        {
          // new key --> create
          WriteKey(rDiff, sPath, sKey, dwType, blData.GetBuffer(), dwDataLen);
        }
        else
        {
          if(((int)dwDataLen != pBl->GetBufferLen()))
          {
            WriteKey(rDiff, sPath, sKey, dwType, blData.GetBuffer(), dwDataLen);
          }
          else if(memcmp(blData.GetBuffer(), pBl->GetBuffer(), dwDataLen) != 0)
          {
            // Value has changed
            WriteKey(rDiff, sPath, sKey, dwType, blData.GetBuffer(), dwDataLen);
          }

          // remove key from map to find missing keys
          map.RemoveKey(sPath + L"\\" + sKey);
        }

        // +++ next item +++
	      nSubkeyIndex++;

        dwNameSize = 255;
        dwDataLen = 4096;
	      if(RegEnumValue(hkCurrent, nSubkeyIndex, sKey.GetBuffer(dwNameSize), &dwNameSize, 0, &dwType, blData.GetBuffer(), &dwDataLen) != ERROR_SUCCESS)
	      {
		      sKey.ReleaseBuffer();
		      break;
	      }

	      sKey.ReleaseBuffer();
      }
    }
    else
      sKey.ReleaseBuffer();
  }

  // +++ recurse through the sub-keys +++
  {
	  int nSubkeyIndex = 0;

    DWORD dwNameSize = 255;
    CStringW sKey;
	  if(RegEnumKeyEx(hkCurrent, nSubkeyIndex, sKey.GetBuffer(dwNameSize), &dwNameSize, 0, NULL, NULL, NULL) == ERROR_SUCCESS)
    {
      sKey.ReleaseBuffer();

      while(true)
      {
        // +++ get data +++
        DWORD dwType = -1;
        unsigned char szData[4096];
        DWORD dwDataLen = 0;
        LONG lr = RegQueryValueEx(hkCurrent, sKey, NULL, &dwType, szData, &dwDataLen);

        // +++ open key +++
	      DWORD dwDisposition;
        HKEY hkChild;
	      RegCreateKeyEx(hkCurrent, sKey, 0, NULL, 0, 0, NULL, &hkChild, &dwDisposition);

        Compare(map, sPath + L"\\" + sKey, hkChild, rDiff);

        // +++ close key +++
	      RegCloseKey(hkChild);

        // +++ next item +++
	      nSubkeyIndex++;

        dwNameSize = 255;
	      if(RegEnumKeyEx(hkCurrent, nSubkeyIndex, sKey.GetBuffer(dwNameSize), &dwNameSize, 0, NULL, NULL, NULL) != ERROR_SUCCESS)
	      {
		      sKey.ReleaseBuffer();
		      break;
	      }

	      sKey.ReleaseBuffer();
      }
    }
    else
      sKey.ReleaseBuffer();
  }
}

//=============================================================================
void CTlsRegDiff::RegDiff(CTlsIStream_Base& is, CTlsOStream_Base& rDiff) 
{
  CWaitCursor wait;

  CMapStringToPtr map(60);

  // +++ read data +++
  CTlsBlob blIn;
  is.Read(blIn);

#ifdef USE_FLATE
  CTlsBlob blDecoded;
  CTlsFlate::Decode(blIn, blDecoded);
#else 
  CTlsBlob blDecoded = blIn;
#endif

  CTlsIStream_Mem rIn(blDecoded);

  while(true)
  {
    char szKey[1024];
    int  iKey = 0;
    while(true)
    {
      BYTE b;
      if(rIn.Read(&b, 1) != 1)
        goto END; // EOF
      if(b == '\0')
        break;
      szKey[iKey++] = (char)b;
    }
    szKey[iKey] = '\0';

    DWORD dwLen;
    rIn.ReadBeDWORD(dwLen);

    unsigned char szType;
    if(rIn.ReadBYTE(szType) != 1)
    {
      return; // error
    }

    unsigned char szData[4096];
    if((DWORD)rIn.Read(szData, dwLen) != dwLen)
    {
      return; // error
    }

    CTlsBlob* pBlob = new CTlsBlob(szData, dwLen);
    map.SetAt(CStringW(szKey), pBlob);
  }
END:  

  // +++ write header +++
  rDiff.Write(";---------------------------------------------------------------------\r\n");
  rDiff.Write(";\r\n");
  rDiff.Write("; Registry-Diff für AIS Scanner\r\n");
  rDiff.Write("; 2002 Björn Schwarzendahl\r\n");
  rDiff.Write(";\r\n");
  rDiff.Write(";---------------------------------------------------------------------\r\n");
  rDiff.Write("\r\n");


  Compare(map, L"L", HKEY_LOCAL_MACHINE, rDiff);
  Compare(map, L"U", HKEY_CURRENT_USER, rDiff);
  Compare(map, L"C", HKEY_CLASSES_ROOT, rDiff);

  // +++ delete remaining keys +++

  rDiff.Write(";gelöschte Schlüssel\r\n");
  rDiff.Write(";diese können leider nicht per .reg-File behandelt werden\r\n");

  POSITION pos = map.GetStartPosition();
  CStringW sKey;
  void*   pItem;
  while (pos != NULL)
  {
    map.GetNextAssoc( pos, sKey, pItem);
    rDiff.Write(CStringW(";Deleted ") + sKey + L"\r\n");
    CTlsBlob* pBl = (CTlsBlob*)pItem;
	CStringW sz = L"<<";
	sz+= (char*)pBl->GetBuffer();
	sz+= L"<<\r\n";
    rDiff.Write(sz);
  }

//  file.Close();

  // +++ finished +++

//  MessageBox(CStringW("Diff wurde erstellt"));

}

/*=============================================================================
    E O F
=============================================================================*/
