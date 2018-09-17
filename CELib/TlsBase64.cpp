//=============================================================================
/** 
\file TlsBase64.cpp
\brief 
\author Rainer Kühner
\author $Author: Rainer Kühner $
\
*/
//=============================================================================

#include "stdafx.h"
#include "TlsBase64.h"
#include "TlsStream.h"
//#include "ZLib/zlib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#include "TlsDbg.h"
#define new DEBUG_NEW
#define malloc DEBUG_MALLOC
#define free DEBUG_FREE
#define realloc DEBUG_REALLOC
#endif

//=============================================================================
/**
\brief 
*/
//=============================================================================
CTlsBase64::CTlsBase64()
{

}

//=============================================================================
/**
\brief 
*/
//=============================================================================
CTlsBase64::~CTlsBase64()
{

}

//=============================================================================
/**
\brief 
*/
//=============================================================================
int CTlsBase64::DecodeChar(char c)
{
  int iCode = -1;
  if(c >= '0' && c <= '9')
    iCode = c - '0' + 52;
  else if(c >= 'a' && c <= 'z')
    iCode = c - 'a' + 26;
  else if(c >= 'A' && c <= 'Z')
    iCode = c - 'A';
  else if(c == '+')
    iCode = 62;
  else if(c == '/')
    iCode = 63;
  
  return iCode;
}

//=============================================================================
/**
\brief 
*/
//=============================================================================
bool CTlsBase64::Decode(CTlsBlob rIn, CTlsBlob& rOut)
{
  int szBuffer[6];
  int iBuffer = 0;

  CTlsIStream_Mem is(rIn);
  CTlsOStream_Mem os(rIn.GetBufferLen());

  while(!is.IsEof())
  {
    char c = (char) is.ReadChar();

    if(c == -1)
      break;
    
    // always ignore '\r' and '\n'
    if(c == '\r' || c == '\n')
      continue;

    szBuffer[iBuffer++] = DecodeChar(c);

    if(iBuffer == 4)
    {
      // 01234567012345670123456701234567
      // 012345  670123  456701  234567

      
      if(szBuffer[0] == -1 || szBuffer[1] == -1)
        ; // nix
      else if(szBuffer[2] == -1)
      {
        os.Write((szBuffer[0] << 2) | ((szBuffer[1] >> 4) & 0x3));
      }
      else if(szBuffer[3] == -1)
      {
        os.Write((szBuffer[0] << 2) | ((szBuffer[1] >> 4) & 0x3));
        os.Write((szBuffer[1] << 4) | ((szBuffer[2] >> 2) & 0xf));
      }
      else
      {
        os.Write((szBuffer[0] << 2) | ((szBuffer[1] >> 4) & 0x3));
        os.Write((szBuffer[1] << 4) | ((szBuffer[2] >> 2) & 0xf));
        os.Write((szBuffer[2] << 6) | (szBuffer[3] & 0x3f));
      }
      
      iBuffer = 0;
    }
  }

  switch(iBuffer)
  {
  case 0:
    break;
  case 1:
    os.Write((szBuffer[0] << 2) | ((szBuffer[1] >> 4) & 0x3));
    break;
  case 2:
    os.Write((szBuffer[0] << 2) | ((szBuffer[1] >> 4) & 0x3));
    os.Write((szBuffer[1] << 4) | ((szBuffer[2] >> 2) & 0xf));
    break;
  default:
    os.Write((szBuffer[0] << 2) | ((szBuffer[1] >> 4) & 0x3));
    os.Write((szBuffer[1] << 4) | ((szBuffer[2] >> 2) & 0xf));
    os.Write((szBuffer[2] << 6) | (szBuffer[3] & 0x3f));
  }

  iBuffer = 0;

  rOut = os.GetBuffer();

  return true;
}



 //*********************************************************************
 //* Base64 - a simple base64 encoder and decoder.
 //*
 //*     Copyright (c) 1999, Bob Withers - bwit@pobox.com
 //*
 //* This code may be freely used for any purpose, either personal
 //* or commercial, provided the authors copyright notice remains
 //* intact.
 //*********************************************************************
 //
 // converted to wxWindows by Frank Buß
 //
 

 const unsigned char fillchar[] = "=";
 
                         // 00000000001111111111222222
                         // 01234567890123456789012345
 static unsigned char     cvt[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
 
                         // 22223333333333444444444455
                         // 67890123456789012345678901
                           "abcdefghijklmnopqrstuvwxyz"
 
                         // 555555556666
                         // 234567890123
                           "0123456789+/";
 
//=============================================================================
/**
\brief 
*/
//=============================================================================
bool CTlsBase64::Encode(CTlsBlob rIn, CTlsBlob& rOut)
// wxString wxBase64::Encode(const wxUint8* pData, size_t len)
{
	unsigned char* pData = rIn.GetBuffer();
	unsigned int len = rIn.GetBufferLen();
     
	size_t c;
    //rOut.Resize(len * 4 / 3 + len * 2 / 50);
    size_t resultLen = 0;
    unsigned char cr[]("\x00d\x00a");
 
     for (size_t i = 0; i < len; ++i)
     {
         c = (pData[i] >> 2) & 0x3f;
         rOut.Append(&cvt[c], 1);
         if (++resultLen == 72) 
		 { 
			 rOut.Append(cr, 2); resultLen = 0; 
		 }

         c = (pData[i] << 4) & 0x3f;
         if (++i < len)
             c |= (pData[i] >> 4) & 0x0f;
 
         rOut.Append(&cvt[c], 1);
         if (++resultLen == 72) 
		 { 
			 rOut.Append(cr, 2); resultLen = 0; 
		 }

         if (i < len)
         {
             c = (pData[i] << 2) & 0x3f;
             if (++i < len)
                 c |= (pData[i] >> 6) & 0x03;
 
             rOut.Append(&cvt[c], 1);
         }
         else
         {
             ++i;
             rOut.Append(fillchar, 1);
         }
         
		 if (++resultLen == 72) 
		 { 
			 rOut.Append(cr, 2); resultLen = 0; 
		 }
 
         if (i < len)
         {
             c = pData[i] & 0x3f;
             rOut.Append(&cvt[c], 1);
         }
         else
         {
             rOut.Append(fillchar, 1);
         }
         if (++resultLen == 72) { rOut.Append(cr, 2); resultLen = 0; }
     }
 
     return true;
 }

//=============================================================================
/**
\brief 
*/
//=============================================================================
char CTlsHex::ToHex(char h)
{
	if(h >= 0 && h <= 9)
		return '0' + h;
	else if( h <= 15)
		return 'A' + h-10;
	else
		return '-';
}

//=============================================================================
/**
\brief 
*/
//=============================================================================
unsigned int CTlsHex::FromHex(wchar_t c)
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
/**
\brief 
*/
//=============================================================================
CString CTlsHex::CharToHex(char c)
{
	char sz[3];
	sz[0] = ToHex(c >> 4);
	sz[1] = ToHex(c & 0x0F);
	sz[2] = '\0';
	CString szTest(sz);
	return szTest;
}

//=============================================================================
/**
\brief 
*/
//=============================================================================
CString CTlsHex::IntToHex8(int i)
{
  return
      CharToHex((i >> 24) & 0xFF) +
      CharToHex((i >> 16) & 0xFF) +
      CharToHex((i >> 8) & 0xFF) +
      CharToHex(i & 0xFF);
}

//=============================================================================
/**
\brief 
*/
//=============================================================================
int CTlsHex::ParseInt(CString s)
{
    int result = 0;
    for(int idx = 0; idx < s.GetLength(); idx++)
    {
        int i = FromHex(s.GetAt(idx));
        result = result * 16 + i;
    }
    
    return result;
}

//=============================================================================
/**
\brief 
*/
//=============================================================================
unsigned long CTlsCrc32::Crc32(unsigned char* psz, unsigned int iLen)
{
	return 0;
	//return crc32(0, psz, iLen);
}

//=============================================================================
/**
\brief 
*/
//=============================================================================
unsigned long CTlsCrc32::Crc32(CTlsBlob blob)
{
	return 0;
   //return crc32(0, blob.GetBuffer(), blob.GetBufferLen());
}

/*=============================================================================
    E O F
=============================================================================*/
