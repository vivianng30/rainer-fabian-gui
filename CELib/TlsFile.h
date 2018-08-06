
#if !defined(AFX_TLSFILE_H__CD0F19D4_FCB8_4801_8BB9_F5B3C8842B19__INCLUDED_)
#define AFX_TLSFILE_H__CD0F19D4_FCB8_4801_8BB9_F5B3C8842B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================//
//
//\brief File-Funktionen
//
//===========================================================================//
class CTlsFile  
{
public:
	CTlsFile();
	virtual ~CTlsFile();

  static bool MkDir(CStringW sDirectory);
  static bool Exists(CStringW sFilename);
  static bool Rename(CStringW sFrom, CStringW sTo, bool bOverwrite);
  static bool Copy(CStringW sFrom, CStringW sTo, bool bOverwrite);
  static bool Delete(CStringW sFilename);
  static bool DeleteDir(CStringW sFilename);
  static int  GetSize(CStringW sPath);
  static CStringW GetUniqueFileName(CStringW sPath, CStringW sPrefix, CStringW sPostfix);
  static CStringW GetFileVersion(CStringW sPath);


};

#endif
/*=============================================================================
    E O F
=============================================================================*/
