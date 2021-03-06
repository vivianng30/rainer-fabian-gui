// LangAdmin.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "LangAdmin.h"

/**********************************************************************************************//**
 * A macro that defines Maximum Language files
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

#define MAX_LANG_FILES	32

/**********************************************************************************************//**
 * A macro that defines Language Extent
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

#define LANG_EXT		_T(".lang")

//extern int  OutputMessage (TCHAR *pFormat, ...);

/**********************************************************************************************//**
 * CLangAdmin
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CLangAdmin::CLangAdmin()
{
	InitializeCriticalSection(&csList);

	m_dwLangFolderStringID=0;
	m_pcStringTable=NULL;
	m_pcsFileNames=NULL;
	m_szTemp=_T("");

	m_tLineBuffer[0]=0x0000;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CLangAdmin class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CLangAdmin::~CLangAdmin()
{
	delete m_pcStringTable;
	m_pcStringTable=NULL;
	delete m_pcsFileNames;
	m_pcsFileNames=NULL;


	if (m_tLineBuffer != NULL) {
		delete [] m_tLineBuffer;
	}
	
	DeleteCriticalSection(&csList);
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CLangAdmin::Init()
{
	m_pcStringTable=new CList<STRINGTABLELINE,STRINGTABLELINE&>;
	m_pcsFileNames=new CStringList();
}

/**********************************************************************************************//**
 * Closes this instance
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CLangAdmin::Close()
{
	m_pcStringTable->RemoveAll();

	m_pcsFileNames->RemoveAll();

	
}

/**********************************************************************************************//**
 * CLangAdmin-Memberfunktionen
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	dwLangFolderStringID	Identifier for the language folder string.
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CLangAdmin::EnumLang( DWORD dwLangFolderStringID )
{
	// Enum avaiable languages
	m_dwLangFolderStringID=dwLangFolderStringID;
	CStringW cs;
	cs.LoadString(dwLangFolderStringID);

	WIN32_FIND_DATA fd;
	int nMax=MAX_LANG_FILES;	
	CStringW csSearch=cs+_T("*");
	csSearch+=LANG_EXT;
	HANDLE hFind=FindFirstFile(csSearch,&fd);

	if(hFind!=INVALID_HANDLE_VALUE)
	{
		try
		{
			do
			{
				m_pcsFileNames->AddHead(fd.cFileName);
			}while(FindNextFile(hFind,&fd) && nMax--);
		}
		catch (CMemoryException* e)
		{
			AfxThrowMemoryException( );
			TCHAR   szCause[255];
			CStringW strFormatted;

			e->GetErrorMessage(szCause, 255);

			strFormatted = _T("CMemoryException CLangAdmin::EnumLang: ");
			strFormatted += szCause;

			theApp.WriteLog(strFormatted);
			//e->ReportError();
			e->Delete();
		}
		catch (...)
		{
			theApp.ReportException(_T("CLangAdmin::EnumLang"));
		}
		

		FindClose(hFind);

		if(GetLastError()!=ERROR_NO_MORE_FILES)
		{
			//CloseHandle(hFind);
			return 0;//ERROR
		}
	}
	else
	{
		return 0;
	}
	BubbleSort(m_pcsFileNames);
	return 1;
}


// *** BubbleSort ***

/**********************************************************************************************//**
 * Bubble sort
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	slStrings	If non-null, the sl strings.
 **************************************************************************************************/

void CLangAdmin::BubbleSort(CStringList *slStrings)
// Sorts the string list alphabetically
// Parameter slStrings String list to sort
// Return None
{
	// Loop at most (slStrings->GetCount() - 1) times
	POSITION endpos = slStrings->GetTailPosition();
	for (int i=1; i<slStrings->GetCount(); i++) 
	{
		bool changed = false;
		POSITION pos2;
		POSITION pos1 = slStrings->GetHeadPosition();
		// Loop through list
		while ((pos2=pos1) != endpos) 
		{
			CString str1 = slStrings->GetNext(pos1); // str1 = string at position pos1
			CString str2 = slStrings->GetAt(pos1); // str2 = string at next position
			if (_tcscmp(str1, str2) > 0) 
			{
				// If str2 is greater than str1 exchange strings
				slStrings->SetAt(pos2, str2);
				slStrings->SetAt(pos1, str1);
				changed = true;
			}
		}
		if (!changed) // If nothing changed in this pass
			return; // sorting is finished
		slStrings->GetPrev(endpos); // Decrease endpos for next pass
	}
}

/**********************************************************************************************//**
 * Gets language files
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the language files.
 **************************************************************************************************/

CStringList* CLangAdmin::GetLangFiles(void)
{
	return m_pcsFileNames;
}

/**********************************************************************************************//**
 * Loads a language
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	csLang	The create struct language.
 * \param	bExt  	True to extent.
 *
 * \return	The language.
 **************************************************************************************************/

DWORD CLangAdmin::LoadLang(CStringW csLang, bool bExt)
{
	EnterCriticalSection(&csList);
	CFile cfDataFile;
	CStringW csFile;
	csFile.LoadString(m_dwLangFolderStringID);
	csFile+=csLang;
	if(!bExt)
		csFile+=LANG_EXT;

	m_pcStringTable->RemoveAll();

	// Does the file exists? 		
	if(cfDataFile.Open(csFile,CFile::modeRead,NULL))
	{
		if(!CheckUnicode(cfDataFile))
		{
			LeaveCriticalSection(&csList);
			return 100;
		}

		DWORD dwResult;

		do
		{
			dwResult=GetStringLine( cfDataFile);
			Sleep(0);
		}while(dwResult==LANGADM_LINESCANNED);

		if(dwResult==LANGADM_ERROR)
		{
			LeaveCriticalSection(&csList);
			return 0;
		}

		cfDataFile.Close();
	}
	LeaveCriticalSection(&csList);
	return 1;
}

/**********************************************************************************************//**
 * Check unicode
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	cf	The cf.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CLangAdmin::CheckUnicode( CFile& cf )
{
	TCHAR p;
	TCHAR q=0xFEFF;

	cf.Read(&p,sizeof(TCHAR));

	if(p==q)
		return true;
	return false;
}

/**********************************************************************************************//**
 * Eocs the given p
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	p	If non-null, a TCHAR to process.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool EOC(TCHAR* p)
{
	if(*p=='#')
		if(*(p-1)=='#')
			return true;
	return false;
}

/**********************************************************************************************//**
 * Eols the given p
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	p	If non-null, a TCHAR to process.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool EOL(TCHAR* p)
{
	if(*p==0x000A)
		if(*(p-1)==0x000D)
			return true;
	return false;
}

/**********************************************************************************************//**
 * Gets string line
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	cf	The cf.
 *
 * \return	The string line.
 **************************************************************************************************/

DWORD CLangAdmin::GetStringLine( CFile& cf)
{
	//////////////////////////////////////////////////////////////////////////////////
	// 1. Column		## 2.			## 3.				
	// IDSTRING			## IDValue		## StringNewLang

	// *******************************************************************************
	int nColumn=1;
	TCHAR* p;
	STRINGTABLELINE stl;
	int nRead=2;

	do
	{
		// Scan columns /////////////////////////////////////////////////////
		int nCount=0;
		int nMaxCount=MAX_PATH;
		memset(m_tLineBuffer,0x00,MAX_PATH*sizeof(TCHAR));
		p=m_tLineBuffer;

		while(nCount<2 && nRead && nMaxCount--)
		{
			nRead=cf.Read(p,sizeof(TCHAR));

			if(*p=='#')
				nCount++;

			if(EOL(p)/*No matching entry found*/) 
			{
				// Set default string (eol, string is empty)
				if(stl.cs.IsEmpty())
				{
					stl.cs.LoadString(stl.nID);
					m_pcStringTable->AddTail(stl);
				}
				return LANGADM_LINESCANNED;
			}
			p++;
		}
		m_tLineBuffer[_tcslen(m_tLineBuffer)-2]=0x0000;

		// Evaluate column data /////////////////////////////////////////////
		if(nColumn==2/*ID-Value*/)
		{
			stl.nID=_wtoi(m_tLineBuffer);
		}
		if(nColumn==3/*StringNewLang*/)
		{
			if(_tcslen(m_tLineBuffer)==0)
			{
				stl.cs.LoadString(stl.nID);
			}
			else
			{
				CStringW cs=m_tLineBuffer;
				int l=cs.GetLength();
				int i=cs.Find(_T("\\n"));
				while(i != -1)
				{
					cs.SetAt(i,0x0a);
					if(i+1<=l)
						cs.SetAt(i+1,0x20);
					i=cs.Find(_T("\\n"));
				}
				stl.cs=cs;
			}
			m_pcStringTable->AddTail(stl);
		}
		nColumn++;

		if(nRead==0)
			return LANGADM_FILESCANNED; // End of file		
	}while(true);

	return LANGADM_ERROR;
}

/**********************************************************************************************//**
 * Searches for the first string
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nID	The identifier.
 *
 * \return	The found string.
 **************************************************************************************************/

CStringW& CLangAdmin::FindString( int nID )
{
	EnterCriticalSection(&csList);
	//int nCount = m_pcStringTable->GetCount();

	POSITION p=m_pcStringTable->GetHeadPosition();

	while(p)
	{	
		static STRINGTABLELINE stl;
		stl=m_pcStringTable->GetNext(p);

		if(stl.nID==nID)
		{
			LeaveCriticalSection(&csList);
			return stl.cs;
		}
	}

	LeaveCriticalSection(&csList);

	return m_szTemp;
}
