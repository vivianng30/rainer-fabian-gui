//=============================================================================
/** 
\file TlsBase64.h
\brief 
\author Rainer Kühner
\author $Author: Rainer Kühner $
*/


//=============================================================================

#ifndef __BIStream_H
#define __BIStream_H

#include "TlsBlob.h"

//===========================================================================//
/**
\brief Base64 Encoder/Decoder
*/
//===========================================================================//
class CTlsBase64  
{
  static int DecodeChar(char c);
  
public:
	CTlsBase64();
	virtual ~CTlsBase64();

  static bool Encode(CTlsBlob rIn, CTlsBlob& rOut);
  static bool Decode(CTlsBlob rIn, CTlsBlob& rOut);

};

//===========================================================================//
/**
\brief Hex Encoder/Decoder

*/
//===========================================================================//
class CTlsHex  
{
public:
	static char ToHex(unsigned int h);
	static unsigned int FromHex(wchar_t c);
	static CString CharToHex(char c);
	static CString IntToHex8(int i);

    static int ParseInt(CString s);
};

//===========================================================================//
/**
\brief 
*/
//===========================================================================//
class CTlsCrc32
{
public:
	static unsigned long Crc32(unsigned char* psz, unsigned int iLen);
	static unsigned long Crc32(CTlsBlob blob);
};

#endif
/*=============================================================================
    E O F
=============================================================================*/
