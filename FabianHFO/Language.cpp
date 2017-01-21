#include "StdAfx.h"
#include "Language.h"
#include "FabianHFO.h"
#include "MVModel.h"
#include "TlsXml.h"
#include "TlsFile.h"

CLanguage* CLanguage::theLang=0;
extern CLangAdmin* g_pGlobalLanguageStrings;

CLanguage::CLanguage(void)
{
	m_pModel = NULL;
	m_pcsLangFiles= new CStringArray();
	/*m_pcsLangFiles->SetSize(32);*/

	CStringW szLangFile=_T("");

	CString szFile = _T("\\FFSDISK\\Language\\languages.xml");
	if(CTlsFile::Exists(szFile))
	{

		CTlsIStream_File is(szFile);
		CTlsXmlScanner scanner(&is);

		// +++ skip start +++
		if(scanner.NextToken() == CTlsXmlScanner::TOK_TYPESTART)
		{
			while(scanner.NextToken() != CTlsXmlScanner::TOK_TYPEEND)
				; // Skip
		}

		//if(scanner.CurrToken() != CTlsXmlScanner::TOK_TAGSTART)
		//	return SetError(scanner, "Tag erwartet (ImportAdmin)");

		if(CString(_T("FabianHFO")).CompareNoCase(CString(scanner.CurrText())) == 0)
		{
			scanner.NextToken();

			scanner.NextToken();

			// +++ Worker-Parameter +++
			int iLangID=-1;
			CTlsXmlScanner::eTokenType Type;
			CString sKey;
			CString sValue;

			while(true)
			{
				if(!scanner.Parse(scanner, Type, sKey, sValue))
				{
					//return false;
					break;
				}

				if(Type == CTlsXmlScanner::TT_BS)
				{
					if(sKey == "language")
					{
						while(true)
						{
							iLangID=-1;
							szLangFile=_T("");

							if(!scanner.Parse(scanner, Type, sKey, sValue))
							{
								//return false;
								break;
							}

							if(Type == CTlsXmlScanner::TT_ATT)
							{
								if(sKey == "id")
								{
									iLangID=_wtoi(sValue);
									/*m_pMTmessage = new CMessageTypClass(	sValue,
										"0",
										"0",
										"0",
										"0",
										"0");*/
									//MessageBox(sValue, L"id", MB_OK); // ignore pWorker->AddHint(sValue);
								}
								else
								{
									CString sTemp = _T("Falsche Option: ");
									sTemp += sKey;
									sTemp += " ";
									sTemp += sValue;

									//CTlsLogfile::WriteLine(sTemp);

									//SetError(scanner, sTemp);
									//SetError(scanner, "Falsche Worker-Option");
									{
										//return false;
										break;
									}
								}



							}
							else if(Type == CTlsXmlScanner::TT_BS)
							{
								//SetError(scanner, "Fehler Parse");
								{
									//return false;
									break;
								}
							}
							else if(Type == CTlsXmlScanner::TT_ES)
							{
								break;
							}

							if(!scanner.Parse(scanner, Type, sKey, sValue))
							{
								//return false;
								break;
							}

							if(Type == CTlsXmlScanner::TT_ATT)
							{
								if(sKey == "file")
								{
									szLangFile=sValue;
									//m_pMTmessage->m_szText1 = sValue;
									//MessageBox(sValue, L"file", MB_OK);//pWorker->SetName(sValue);
								}
								else
								{
									CString sTemp = _T("Falsche Option: ");
									sTemp += sKey;
									sTemp += " ";
									sTemp += sValue;

									//CTlsLogfile::WriteLine(sTemp);

									//SetError(scanner, sTemp);
									//SetError(scanner, "Falsche Worker-Option");
									{
										//return false;
										break;
									}
								}



							}
							else if(Type == CTlsXmlScanner::TT_BS)
							{
								//SetError(scanner, "Fehler Parse");
								{
									//return false;
									break;
								}
							}
							else if(Type == CTlsXmlScanner::TT_ES)
							{
								break;
							}

							if(iLangID!=-1 && szLangFile!=_T(""))
							{
								m_pcsLangFiles->InsertAt(iLangID,szLangFile);
							}
							

						}

						
					}
					else 
					{
						//return false;
						break;
					}
				}
				else if(Type == CTlsXmlScanner::TT_ES)
					break;
				else
				{
					//return false;
					break;
				}
			}
		}
	}
}

CLanguage::~CLanguage(void)
{
	try
	{
		m_pcsLangFiles->RemoveAll();
	}
	catch (...)
	{
		throw;
	}
	
	delete m_pcsLangFiles;
	//memory leak!!!! delete funktioniert nicht
	try
	{
		m_pcLangAdmin.Close();
	}
	catch (...)
	{
	}
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CLanguage::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

// **************************************************************************
// 
// **************************************************************************
CLanguage* CLanguage::GetInstance()
{
	if(theLang == 0)
	{
		theLang = new CLanguage;

		
	}
	return theLang;
}

// **************************************************************************
// 
// **************************************************************************
void CLanguage::DestroyInstance()
{
	if(theLang != NULL)
	{
		delete theLang;
		theLang = NULL;

		
	}
}

// **************************************************************************
// 
// **************************************************************************
void CLanguage::Init()
{
	//m_pcLangAdmin=new CLangAdmin();
	m_pcLangAdmin.Init();
	m_pcLangAdmin.EnumLang(IDS_LANGPATH);

	//m_pcsLangNames = m_pcLangAdmin->GetLangFiles();


	/*CStringW cs[3];
	cs[0]=_T("engl");
	cs[1]=_T("chin");
	cs[2]=_T("russ");
	m_pcLangAdmin->LoadLang(cs[2]);*/

	DWORD dwResult = m_pcLangAdmin.LoadLang(getModel()->getCONFIG()->getLANGUAGE(),true);

	if(dwResult==100)
	{
		CStringW szLog = _T("#HFO:0214: ");
		szLog+=getModel()->getCONFIG()->getLANGUAGE();
		theApp.getLog()->WriteLine(szLog);
		getModel()->getCONFIG()->SetLanguage(LANGFILE_ENGLISH);

		dwResult = m_pcLangAdmin.LoadLang(LANGFILE_ENGLISH,true);

		if(dwResult==100)
		{
			szLog = _T("#HFO:0215: ");
			szLog+=LANGFILE_ENGLISH;
			theApp.getLog()->WriteLine(szLog);
		}
	}

	//EnterCriticalSection(&csList);

	//g_pGlobalLanguageStrings=m_pcLangAdmin;
	//LeaveCriticalSection(&csList);

}

bool CLanguage::IsXMLLangFile()
{
	if(m_pcsLangFiles->GetSize()>0)
		return true;
	else
		return false;
}

CStringW CLanguage::GetLangFileName(int iPos)
{
	if(m_pcsLangFiles->GetSize()>=iPos+1)
		return m_pcsLangFiles->GetAt(iPos);
	else
		return LANGFILE_ENGLISH;
}

int CLanguage::GetLangFileID(CStringW szName)
{
	CStringW szTemp=_T("");
	for (int iPos=0;iPos < m_pcsLangFiles->GetSize();iPos++)
	{
		szTemp=m_pcsLangFiles->GetAt(iPos);
		if(szTemp==szName)
			return iPos;
	}
	return 0;
}

CStringList* CLanguage::GetLangFiles()
{
	return m_pcLangAdmin.GetLangFiles();
}

DWORD CLanguage::LoadLang(CStringW csLang, bool bExt)
{
	return m_pcLangAdmin.LoadLang(csLang,bExt);
}

//CConfiguration *CLanguage::getCONFIG()
//{
//	if(m_pConfig==NULL)
//		m_pConfig=CConfiguration::GetInstance();
//	return m_pConfig;
//}


// **************************************************************************
//
// Language-Library
//
// **************************************************************************
static CStringW CS_ERROR=_T("- - -");
CStringW CLanguage::GetLanguageString(int nID)
{
	CStringW cs=_T("");
	//if(m_pcLangAdmin)
	{
		//EnterCriticalSection(&csList);
		cs=m_pcLangAdmin.FindString(nID);
		if(cs==_T(""))
		{
			cs.LoadString(nID);
			//LeaveCriticalSection(&csList);
			return cs;
		}
		return cs;
	}
	return CS_ERROR;
}