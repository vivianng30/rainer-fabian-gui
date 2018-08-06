//=============================================================================
/** 
* \file Language.h
* \class CLanguage
* \brief Class for loading language files.
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
* The language classloads depending on the selected language the associated
* strings from the language files.
*
*       Copyright (c) 2013 Ing.Buero Rainer Kuehner
*                      Illerkirchberg
*
**/
//=============================================================================
#pragma once
//#include "globDefs.h"
#include "LangAdmin.h"
//#include "Configuration.h"

class CMVModel;

class CLanguage
{
public:
	

	static CLanguage* GetInstance();
	static void DestroyInstance();

	void Init();

	CMVModel *getModel();

	CStringW GetLanguageString(int nID);
	CStringList* GetLangFiles(void);
	DWORD LoadLang(CStringW csLang, bool bExt);

	CStringW GetLangFileName(int iPos);
	int GetLangFileID(CStringW szName);
	bool IsXMLLangFile();

private:
//singleton
	static CLanguage* theLang;

protected:

	CLanguage(void);
	virtual ~CLanguage(void);

	
	

	//CConfiguration *getCONFIG();

protected:
	CMVModel* m_pModel;
	//CConfiguration *m_pConfig;
	CLangAdmin m_pcLangAdmin;

	//CStringList* m_pcsLangNames;
	CStringArray* m_pcsLangFiles;

	
};
