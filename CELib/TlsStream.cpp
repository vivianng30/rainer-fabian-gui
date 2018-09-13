
#include "stdafx.h"
#include "TlsStream.h"
#include "TlsFile.h"

#include <ctype.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#include "TlsDbg.h"
#define new DEBUG_NEW
#define malloc(a) DEBUG_MALLOC(a)
#define free(a) DEBUG_FREE(a)
#define realloc DEBUG_REALLOC
#endif

#define RSTD_PLATFORM_WINDOWSCE

//=============================================================================
CTlsIStream_Base::CTlsIStream_Base() 
: m_iPushBack(-1), m_bEof(false)
{
}

//=============================================================================
CTlsIStream_Base::~CTlsIStream_Base() 
{
}

//=============================================================================
//
// \brief read a char from the stream
// \return the char
//
//=============================================================================
int CTlsIStream_Base::ReadChar()
{
  if(m_iPushBack != -1)
  { 
    int iRet = m_iPushBack;
    m_iPushBack = -1;
    return iRet;
  }
  else
    return GetChar();
}

//=============================================================================
//
// \brief push a char back into the stream.
// \param i the char
//
// there is only ONE pushback-char at once
//
//=============================================================================
void CTlsIStream_Base::PushBack(int i)
{
  m_iPushBack = i;
}
  
//=============================================================================
//
// \brief Is End Of File/Stream
// \return bool if EOF
//
//=============================================================================
bool CTlsIStream_Base::IsEof()
{
  if(m_iPushBack != -1)
    return false;
  int i = GetChar();
  PushBack(i);

  return i == -1;
}

//=============================================================================
//
// \brief Is Good
// \return true if Good
//
//=============================================================================
bool CTlsIStream_Base::IsGood()
{
  return true;
  // TODO
}

//=============================================================================
//
//\brief 
//
//=============================================================================
bool CTlsIStream_Base::Read(CStringW& rString, int iLen)
{
  for(int i = 0; i < iLen; i++)
  {
    int c = ReadChar();
    rString += (char)c;
  }
  return true;
}

//=============================================================================
bool CTlsIStream_Base::ReadLine(CStringW& rString)
{
  while(!IsEof())
  {
    char c = (char) ReadChar();
    if(c == '\r')
    {
      int c2 = ReadChar();
      if(c2 != '\n')
        PushBack(c2);
      break;
    }
    if(c == '\n')
    {
      int c2 = ReadChar();
      if(c2 != '\r')
        PushBack(c2);
      break;
    }
    wchar_t wc;
    mbstowcs(&wc, &c, 1);
    rString += wc;
  }
  return true;
}

//=============================================================================
bool CTlsIStream_Base::ReadString(CStringW& rString, CStringW delimchars)
{
  while(!IsEof())
  {
    int c = ReadChar();
    if(c == -1)
      break;

    char sz[2] = " ";
    sz[0] = (char) c;
    if(delimchars.FindOneOf(CStringW(sz)) != -1)
      break;

    rString += (char) c;
  }

  return true;
}

//=============================================================================
bool CTlsIStream_Base::Read(CTlsBlob& rBlob, int iLen)
{
  if(iLen == -1)
  {
    while(!IsEof())
    {
      BYTE szBuffer[4096];
      int iLen = Read(szBuffer, sizeof(szBuffer));
      rBlob.Append(szBuffer, iLen);

      if(iLen != sizeof(szBuffer))
        break;
    }
  }
  else
  {
    int iRead = 0;
    if(!rBlob.Resize(iLen))
      return false;

    int i = Read(rBlob.GetBuffer(), iLen);
    if(i != iLen)
      rBlob.Resize(i);
  }
  return true;
}

//=============================================================================
bool CTlsIStream_Base::ReadBYTE(BYTE& pc)
{
  int i = ReadChar();
  if(i == -1)
    return false;

  pc = (BYTE)i;

  return true;
}

//=============================================================================
int CTlsIStream_Base::Read(BYTE* by, int iCnt)
{
int i = 0;
  for(; i < iCnt; i++)
  {
    if(IsEof())
      break;
    by[i] = (char) ReadChar();
  }
  return i;
}

//=============================================================================
bool CTlsIStream_Base::ReadLeWORD(WORD& pw)
{
  BYTE sz[2];
	Read(sz, 2);
	pw = ((WORD)sz[1] << 8) | sz[0];
	return true;
}

//=============================================================================
bool CTlsIStream_Base::ReadLeDWORD(DWORD& pdw)
{
	BYTE sz[4];
	Read(sz, 4);
	DWORD l;
	l = sz[3];
	l = l << 8;
	l |= sz[2];
	l = l << 8;
	l |= sz[1];
	l = l << 8;
	l |= sz[0];
	pdw = l;
	return true;
}

//=============================================================================
bool CTlsIStream_Base::ReadLeLONG(LONG& pl)
{
	BYTE sz[4];
	Read(sz, 4);
	LONG l;
	l = sz[3];
	l = l << 8;
	l |= sz[2];
	l = l << 8;
	l |= sz[1];
	l = l << 8;
	l |= sz[0];
	pl = l;
	return true;
}

//=============================================================================
bool CTlsIStream_Base::ReadBeWORD(WORD& pw)
{
	BYTE sz[2];
	if(!Read(sz, 2))
    return false;

	pw = ((WORD)sz[0] << 8) | sz[1];
	return true;
}

//=============================================================================
bool CTlsIStream_Base::ReadBeDWORD(DWORD& dw)
{
	BYTE sz[4];
	if(!Read(sz, 4))
    return false;

	DWORD l;
	l = sz[0];
	l = l << 8;
	l |= sz[1];
	l = l << 8;
	l |= sz[2];
	l = l << 8;
	l |= sz[3];
	dw = l;
	return true;
}

//=============================================================================
bool CTlsOStream_Base::WriteBeDWORD(const DWORD dw)
{
	Write((BYTE)((dw & 0xff000000) >> 24));
	Write((BYTE)((dw & 0x00ff0000) >> 16));
	Write((BYTE)((dw & 0x0000ff00) >>  8));
	Write((BYTE)((dw & 0x000000ff) >>  0));
	return true;
}

//=============================================================================
bool CTlsIStream_Base::ReadBeLONG(LONG& pl)
{
	BYTE sz[4];
	if(!Read(sz, 4))
    return false;

	long l;
	l = sz[0];
	l = l << 8;
	l |= sz[1];
	l = l << 8;
	l |= sz[2];
	l = l << 8;
	l |= sz[3];
	pl = l;
	return true;
}

//=============================================================================
int  CTlsIStream_Base::GetPos()
{
  return -1;
}

//=============================================================================
bool CTlsIStream_Base::GotoAbs(int)
{
  return false;
}

//=============================================================================
bool CTlsIStream_Base::GotoAbsR(int)
{
  return false;
}

//=============================================================================
CTlsOStream_Base::CTlsOStream_Base()
: m_iStreamPos(0)
{
}

//=============================================================================
CTlsOStream_Base::~CTlsOStream_Base() 
{
  Finish();
}

//=============================================================================
bool CTlsOStream_Base::IsGood()
{
  return true;
}

//=============================================================================
int CTlsOStream_Base::GetPos()
{
  return m_iStreamPos;
}

//=============================================================================
bool CTlsOStream_Base::Write(const int c)
{
  m_iStreamPos++;
  return PutChar(c);
}

//=============================================================================
bool CTlsOStream_Base::Write(const char* psz)
{
  while(*psz)
  {
    if(!Write(*psz))
      return false;
    psz++;
  }
  return true;
}

//=============================================================================
bool CTlsOStream_Base::Write(const BYTE* psz, int iCnt)
{
  return Put((char*)psz, iCnt);
}

//=============================================================================
bool CTlsOStream_Base::Finish()
{
  return true;
}

//=============================================================================
bool CTlsOStream_Base::Flush()
{
  return Finish();
}

//=============================================================================
bool CTlsOStream_Base::Write(const CStringW sData)
{
  bool bRet = true;
  CStringW s = sData;
  int iLen = sData.GetLength();
  char* psz = (char*)malloc(iLen+1);
  for(int idx = 0; idx < iLen; idx++)
	  psz[idx] = (char) sData.GetAt(idx);
/*
  int iLen = wcstombs(psz, sData, i);
*/
  if(iLen > 0)
    bRet = Write(psz, iLen);
  free(psz);
  return bRet;
}

//=============================================================================
bool CTlsOStream_Base::Write(const CTlsBlob& blData)
{
  return Write((char*)blData.GetBufferC(), blData.GetBufferLen());
}

//=============================================================================
bool CTlsOStream_Base::WriteBYTE(const BYTE by)
{
  return Write(by);
}

//=============================================================================
bool CTlsOStream_Base::WriteLeWORD(const WORD w)
{
	if(!Write(w & 0xff))
    return false;
	return Write((w >> 8) & 0xff);
}

//=============================================================================
bool CTlsOStream_Base::WriteLeDWORD(const DWORD rdw)
{
    DWORD dw = rdw;
    if(!Write(dw & 0xff))
        return false;
    dw = dw >> 8;
    if(!Write(dw & 0xff))
        return false;
    dw = dw >> 8;
    if(!Write(dw & 0xff))
        return false;
    dw = dw >> 8;
    if(!Write(dw & 0xff))
        return false;
    return true;
}

//=============================================================================
bool CTlsOStream_Base::WriteLeLONG(const LONG l)
{
	return WriteLeDWORD(l);
}

//=============================================================================
bool CTlsOStream_Base::WriteBeWORD(const WORD w)
{
	Write((BYTE)((w & 0xff00) >> 8));
	Write((BYTE)(w & 0xff));
  return true;
}

//=============================================================================
bool CTlsOStream_Base::WriteBeLONG(const LONG l)
{
	return WriteBeDWORD((unsigned long)l);
}

//=============================================================================
int CTlsIStream_File::GetChar()
{
  if(!m_hIn)
    return -1;

  return fgetc(m_hIn);
}

//=============================================================================
CTlsIStream_File::CTlsIStream_File(CStringW sFileName)
{
  char szFileName[255];
  wcstombs(szFileName, sFileName.GetBuffer(0), 255);
  m_hIn = fopen(szFileName, "rb");
}

//=============================================================================
CTlsIStream_File::~CTlsIStream_File()
{
  Close();
}

//=============================================================================
void CTlsIStream_File::Close()
{
  if(m_hIn)
    fclose(m_hIn);
}

//=============================================================================
bool CTlsIStream_File::GotoAbs(int i)
{
  if(fseek(m_hIn, i, SEEK_SET) == 0)
    return true;
  return false;
}

//=============================================================================
bool CTlsIStream_File::GotoAbsR(int i)
{
  if(fseek(m_hIn, -i, SEEK_END) == 0)
    return true;
  return false;
}

//=============================================================================
CTlsOStream_File::CTlsOStream_File(CStringW sFileName, bool bAppend)
  : m_hOut(NULL)
{
  char szFileName[255];
  wcstombs(szFileName, sFileName.GetBuffer(0), 255);

  if(CTlsFile::Exists(sFileName))
  {
	  TRY {
		  CFileStatus Status; 
		  if(CFile::GetStatus(sFileName, Status))
		  {
			  Status.m_attribute &= ~CFile::readOnly;
			  CFile::SetStatus(sFileName, Status);
		  }
	  } CATCH (CFileException, e) {
		  //return false;
	  }
	  END_CATCH
  }

   m_hOut = fopen(szFileName, bAppend ? "ab" : "wb");
}

//=============================================================================
void CTlsOStream_File::Close()
{
  if(m_hOut)
    fclose(m_hOut);
}

//=============================================================================
CTlsOStream_File::~CTlsOStream_File()
{
  Close();
}

//=============================================================================
bool CTlsOStream_File::PutChar(int i)
{
  if(!m_hOut)
    return false;

  int r = fputc(i, m_hOut);

  return r != EOF;
}

//=============================================================================
bool CTlsOStream_File::Put(char* pszBytes, int iBytes)
{
  if(!m_hOut)
    return false;

  int iWritten = fwrite(pszBytes, 1, iBytes, m_hOut);
  fflush(m_hOut);

  return iWritten == iBytes;
}

//=============================================================================
bool CTlsOStream_File::Finish()
{
  fflush(m_hOut);
  return true; 
}

//=============================================================================
int CTlsIStream_Mem::GetChar()
{
  if(m_iCurrPos == m_blData.GetBufferLen())
    return -1;

  return m_blData.GetBuffer()[m_iCurrPos++];
}

//=============================================================================
CTlsIStream_Mem::CTlsIStream_Mem()
  : m_iCurrPos(0)
{
}

//=============================================================================
CTlsIStream_Mem::CTlsIStream_Mem(CTlsBlob blData)
  : m_blData(blData), m_iCurrPos(0)
{
}

//=============================================================================
CTlsIStream_Mem::CTlsIStream_Mem(const BYTE* pData, int iLen)
  : m_blData(pData, iLen), m_iCurrPos(0)
{
}

//=============================================================================
CTlsIStream_Mem::~CTlsIStream_Mem()
{
}

//=============================================================================
void CTlsIStream_Mem::SetBuffer(CTlsBlob blData)
{
  m_blData = blData;
}

//=============================================================================
void CTlsIStream_Mem::SetBuffer(const BYTE* pData, int iLen)
{
  m_blData.Set(pData, iLen);
}

//=============================================================================
bool CTlsIStream_Mem::GotoAbs(int i)
{
  if(i > m_blData.GetBufferLen())
    return false;

  m_iCurrPos = i;

  return true;
}

//=============================================================================
bool CTlsIStream_Mem::GotoAbsR(int i)
{
  if(i > m_blData.GetBufferLen())
    return false;

  m_iCurrPos = m_blData.GetBufferLen() - i;

  return true;
}

//=============================================================================
CTlsOStream_Mem::CTlsOStream_Mem(int iInitialLength)
  : m_blData(iInitialLength), m_iCurrPos(0)
{
}
 
//=============================================================================
CTlsOStream_Mem::CTlsOStream_Mem(CTlsBlob& blBuffer)
  : m_blData(blBuffer), m_iCurrPos(0)
{
}
 
//=============================================================================
CTlsOStream_Mem::~CTlsOStream_Mem()
{
  
}

//=============================================================================
bool CTlsOStream_Mem::PutChar(int i)
{
  int iBufLen = m_blData.GetBufferLen();
  if(m_iCurrPos >= iBufLen-1)
    m_blData.Resize(iBufLen == 0 ? 100 : iBufLen * 2);

  m_blData.GetBuffer()[m_iCurrPos] = (char)i;
  m_iCurrPos++;

  return true;
}

//=============================================================================
bool CTlsOStream_Mem::Put(char* pszBytes, int iBytes)
{
  for(int idx = 0; idx < iBytes; idx++)
    PutChar(pszBytes[idx]);

  return true;
}

//=============================================================================
bool CTlsOStream_Mem::Finish()
{
  return m_blData.Resize(m_iCurrPos);
}

//=============================================================================
void CTlsOStream_Mem::SetBuffer(CTlsBlob blBuffer)
{
  m_blData = blBuffer;
}

//=============================================================================
CTlsBlob CTlsOStream_Mem::GetBuffer()
{
  m_blData.Resize(m_iCurrPos);

  return m_blData;
}

//=============================================================================
CTlsIStream_Res::CTlsIStream_Res(HINSTANCE, int)
{
}

//=============================================================================
CTlsIStream_Res::CTlsIStream_Res(HINSTANCE hMod, CStringW sResName, CStringW sResType)
{
	if(!hMod)
		hMod = GetModule();
	
//	HRSRC hres = FindResource(hMod, CStringW(sResName), CStringW(sResType));
	HRSRC hres = FindResource(hMod, sResName, sResType);
	if(!hres)
		return;
	
	Load(hMod, hres);
}

//=============================================================================
CTlsIStream_Res::~CTlsIStream_Res()
{
}

//=============================================================================
HINSTANCE CTlsIStream_Res::GetModule()
{
#ifdef RSTD_PLATFORM_WINDOWSCE
  
  #pragma message("missing platform port")
  return NULL;
  
#else
	char pszCommandLine[255];
	strncpy(pszCommandLine, GetCommandLine(), 254);
	char* psz = strchr(pszCommandLine, '.');
	if(psz)
		*(psz+4) = '\0';
	return GetModuleHandle(pszCommandLine);
#endif
}

//=============================================================================
bool CTlsIStream_Res::Load(HINSTANCE hMod, HRSRC hRes)
{
	HGLOBAL hglob = LoadResource(hMod, hRes);
	if(!hglob)
		return false;
	
	DWORD size = ::SizeofResource(hMod, hRes);
	
	BYTE* pv = (BYTE*)LockResource(hglob);
	if(!pv)
		return false;
	
  SetBuffer(pv, size);

  return true;
}

//=============================================================================
int CTlsIStream_Res::GetChar()
{
  return -1;
}

//=============================================================================
int CTlsIStream_Counter::GetChar()
{
  int i = m_pPre->ReadChar();
  if(i == '\n')
  {
    m_iCurrLine++;
    m_iCurrCol = 0;
  }
  else
  {
    m_iCurrCol++;
  }
  return i;
}

//=============================================================================
CTlsIStream_Counter::CTlsIStream_Counter(CTlsIStream_Base* pPre)
  : m_pPre(pPre), m_iCurrLine(0), m_iCurrCol(0)
{
}

//=============================================================================
int CTlsIStream_Counter::GetCurrLine()
{
  return m_iCurrLine;
}

//=============================================================================
int CTlsIStream_Counter::GetCurrCol()
{
  return m_iCurrCol;
}

/*=============================================================================
    E O F
=============================================================================*/
