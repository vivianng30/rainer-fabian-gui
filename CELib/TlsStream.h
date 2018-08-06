
#ifndef __STREAM_H__
#define __STREAM_H__

#include "TlsBlob.h"
#include <stdio.h>
// #include <windows.h>

//===========================================================================//
//
//\brief 
//
//  Wichtig: Ein Stream ist IMMER Binary
//
//===========================================================================//
class CTlsIStream_Base
{
  bool m_bEof;
  int  m_iPushBack;

protected:
  virtual int GetChar()=0;

public:
  CTlsIStream_Base();
  virtual ~CTlsIStream_Base();

  int  ReadChar();
  void PushBack(int i);
  bool IsEof();
  virtual bool IsGood();
  bool Read(CStringW& CStringW, int iLen);
  bool ReadLine(CStringW& rString);
  bool ReadString(CStringW& rString, CStringW delimchars);
  bool Read(CTlsBlob& CTlsBlob, int iLen = -1);

  bool ReadBYTE(BYTE& pc);
  int  Read(BYTE* by, int iCnt);
  bool ReadLeWORD(WORD& pw);
  bool ReadLeDWORD(DWORD& pdw);
  bool ReadLeLONG(LONG& pl);
  bool ReadBeWORD(WORD& pc);
  bool ReadBeDWORD(DWORD& pc);
  bool ReadBeLONG(LONG& pc);

  int  GetPos();
  virtual bool GotoAbs(int i);
  virtual bool GotoAbsR(int i);
};

class CTlsOStream_Base
{
protected:
  unsigned int m_iStreamPos;

  virtual bool PutChar(int i)=0;
  virtual bool Put(char* pszBytes, int iBytes)=0;
  virtual bool Finish()=0;

public:
  CTlsOStream_Base();
  virtual ~CTlsOStream_Base();

  virtual bool IsGood();

  bool Write(const int i);
  bool Write(const char* psz);
  bool Write(const BYTE* psz, int iCnt);
  bool Write(const char* psz, int iCnt) { return Write((BYTE*)psz, iCnt); }

  bool Write(const CStringW sData);
  bool Write(const CTlsBlob&   blDate);
  bool WriteBYTE(const BYTE pc);
  bool WriteLeWORD(const WORD pc);
  bool WriteLeDWORD(const DWORD pc);
  bool WriteLeLONG(const LONG pc);
  bool WriteBeWORD(const WORD pc);
  bool WriteBeDWORD(const DWORD pc);
  bool WriteBeLONG(const LONG pc);

  virtual int  GetPos();
  bool Flush();
};

class CTlsIStream_File : public CTlsIStream_Base
{
  FILE* m_hIn;

  virtual int GetChar();

public:
  CTlsIStream_File(CStringW sFileName);
  virtual ~CTlsIStream_File();

  virtual bool GotoAbs(int i);
  virtual bool GotoAbsR(int i);

  void Close();
};

class CTlsOStream_File : public CTlsOStream_Base
{
  FILE* m_hOut;

  virtual bool PutChar(int i);
  virtual bool Put(char* pszBytes, int iBytes);
  virtual bool Finish();

public:
  CTlsOStream_File(CStringW sFileName, bool bAppend = false);
  virtual ~CTlsOStream_File();
  void Close();
};

class RStreamFilter
{
public:
  //RStreamFilter();

  virtual void Reset()=0;
  virtual bool Convert(CTlsBlob rIn, CTlsBlob& rOut, bool bFinish)=0;
};

class CTlsIStream_Filter : public CTlsIStream_Base
{
  CTlsIStream_Base* m_pIn;
  RStreamFilter* m_pFilter;
  bool           m_bAutoDelete;
  int            m_iBlockSize;
  CTlsBlob          m_blBuffer;
  int            m_iCurrPos;

  virtual int GetChar();

public:
  CTlsIStream_Filter(CTlsIStream_Base* pIn, RStreamFilter* pFilter, bool bAutoDelete, int iBlockSize);
  virtual ~CTlsIStream_Filter();

  virtual bool GotoAbs(int i);
  virtual bool GotoAbsR(int i);
};

class CTlsOStream_Filter : public CTlsOStream_Base
{
  CTlsOStream_Base* m_pOut;
  RStreamFilter* m_pFilter;
  bool           m_bAutoDelete;
  int            m_iBlockSize;
  CTlsBlob          m_blBuffer;
  int            m_iCurrPos;

  virtual bool PutChar(int i);
  virtual bool Put(char* pszBytes, int iBytes);
  virtual bool Finish();

public:
  CTlsOStream_Filter(CTlsOStream_Base* pOut, RStreamFilter* pFilter, bool bAutoDelete, int iBlockSize);
  virtual ~CTlsOStream_Filter();
};

class CTlsIStream_Mem : public CTlsIStream_Base
{
  CTlsBlob m_blData;
  int   m_iCurrPos;

  virtual int GetChar();
  
public:
  CTlsIStream_Mem();
  CTlsIStream_Mem(CTlsBlob blData);
  CTlsIStream_Mem(const BYTE* pData, int iLen);
  virtual ~CTlsIStream_Mem();

  void SetBuffer(CTlsBlob blData);
  void SetBuffer(const BYTE* pData, int iLen);

  virtual bool GotoAbs(int i);
  virtual bool GotoAbsR(int i);
};

class CTlsOStream_Mem : public CTlsOStream_Base
{
  CTlsBlob m_blData;
  int   m_iCurrPos;

  virtual bool PutChar(int i);
  virtual bool Put(char* pszBytes, int iBytes);
  virtual bool Finish();
  
public:
  CTlsOStream_Mem(int iInitialLength=0);
  CTlsOStream_Mem(CTlsBlob& rData);
  virtual ~CTlsOStream_Mem();

  CTlsBlob GetBuffer();
  void SetBuffer(CTlsBlob blBuffer);
};

class CTlsIStream_Res : public CTlsIStream_Mem
{
  virtual int GetChar();

protected:
	HINSTANCE GetModule();
	bool Load(HINSTANCE hMod, HRSRC hres);
  
public:
  CTlsIStream_Res(HINSTANCE hm, int iResID);
  CTlsIStream_Res(HINSTANCE hMod, CStringW sResName, CStringW sResType);
  virtual ~CTlsIStream_Res();
};

class CTlsIStream_Counter : public CTlsIStream_Base
{
  CTlsIStream_Base* m_pPre;

  int m_iCurrLine;
  int m_iCurrCol;

  virtual int GetChar();
  
public:
  CTlsIStream_Counter(CTlsIStream_Base* pPre);

  int GetCurrLine();
  int GetCurrCol();
};

#endif
/*=============================================================================
    E O F
=============================================================================*/
