/**********************************************************************************************//**
 * \file	LangAdmin.h.
 *
 * Declares the language admin class
 **************************************************************************************************/

#pragma once


#define LANGADM_LINESCANNED		0x00000001
#define LANGADM_FILESCANNED		0x00000002
#define LANGADM_ERROR			0x0000ffff

/***** HEADER ***************************************************************/
#include <Afxtempl.h> // CList
//static CStringW LANGADM_DEFAULT=_T("");

//////////////////////////////////////////////////////////////////////////////
typedef struct tagSTRINGTABLELINE
{
	int nID;
	CStringW cs;
}STRINGTABLELINE, *LPG_STRINGTABLELINE;

// CLangAdmin-Befehlsziel

class CLangAdmin : public CObject
{
public:
	CLangAdmin();
	virtual ~CLangAdmin();

	DWORD EnumLang( DWORD dwLangFolderStringID );
	DWORD LoadLang(CStringW csLang, bool bExt = false);
	void BubbleSort(CStringList *slStrings);

	CStringList* GetLangFiles(void);
	CStringW& FindString( int nID );

	void Init();
	void Close();
private:
	bool CheckUnicode( CFile& cf );
	DWORD GetStringLine( CFile& cf);
	// Attributes
private:
	CRITICAL_SECTION	csList;
	TCHAR	m_tLineBuffer[MAX_PATH];
	CList<STRINGTABLELINE,STRINGTABLELINE&>* m_pcStringTable;
	CStringList* m_pcsFileNames;
	DWORD m_dwLangFolderStringID;
	CStringW m_szTemp;

};


