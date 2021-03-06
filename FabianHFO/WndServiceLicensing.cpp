// WndServiceLicensing.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceLicensing.h"
#include "TlsFile.h"
#include "TlsXml.h"
#include "Rijndael.h"
#include "PasswordDlg.h"
#include "DlgMessageBox.h"

/**********************************************************************************************//**
 * CWndServiceLicensing
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndServiceLicensing, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndServiceLicensing class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceLicensing::CWndServiceLicensing():
CWndService()
{
	m_pDlg = NULL;

	m_pcActivate_Up=NULL;
	m_pcActivate_Dw=NULL;
	
	m_pcActivateFOT=NULL;
	m_pcActivateHFO=NULL;
	m_pcActivateNMODE=NULL;
	m_pcActivateVguaranty=NULL;
	m_pcActivateVlimit=NULL;
	m_pcActivateTrends=NULL;
	m_pcActivateLungRec=NULL;
	m_pcActivateTherapy=NULL;
	m_pcActivatePRICO=NULL;
	m_pcActivateNIVTRIGGER=NULL;
	m_pcDelNIVTRIGGER=NULL;

	m_pcDelFOT=NULL;
	m_pcDelHFO=NULL;
	m_pcDelNMODE=NULL;
	m_pcDelVguaranty=NULL;
	m_pcDelVlimit=NULL;
	m_pcDelTrends=NULL;
	m_pcDelLungRec=NULL;
	m_pcDelTherapy=NULL;
	m_pcDelPRICO=NULL;

	m_szParsedHFOKey=_T("0");
	m_szParsedNMODEKey=_T("0");
	m_szParsedVGUARANTYKey=_T("0");
	m_szParsedVLIMITKey=_T("0");
	m_szParsedTRENDSKey=_T("0");
	m_szParsedLUNGRECKey=_T("0");
	m_szParsedTHERAPYKey=_T("0");
	m_szParsedPRICOKey=_T("0");
	m_szParsedFOTKey=_T("0");
	m_szParsedNIVTRIGGERKey=_T("0");

	m_bVGUARANTModuleEnabled=false;
	m_bVLIMITModuleEnabled=false;
	m_bHFOModuleEnabled=false;
	m_bNMODEModuleEnabled=false;
	m_bTRENDSModuleEnabled=false;
	m_bLUNGRECModuleEnabled=false;
	m_bTHERAPYModuleEnabled=false;
	m_bPRICOModuleEnabled=false;
	m_bFOTModuleEnabled=false;
	m_bNIVTRIGGERModuleEnabled=false;

	m_szEncryptedFormattedHFOKey=_T("");
	m_szEncryptedFormattedNMODEKey=_T("");
	m_szEncryptedFormattedVGUARANTYKey=_T("");
	m_szEncryptedFormattedVLIMITKey=_T("");
	m_szEncryptedFormattedTRENDSKey=_T("");
	m_szEncryptedFormattedLUNGRECKey=_T("");
	m_szEncryptedFormattedTHERAPYKey=_T("");
	m_szEncryptedFormattedPRICOKey=_T("");
	m_szEncryptedFormattedFOTKey=_T("");
	m_szEncryptedFormattedNIVTRIGGERKey=_T("");
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndServiceLicensing class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceLicensing::~CWndServiceLicensing()
{
	delete m_pcActivateFOT;
	delete m_pcActivateHFO;
	delete m_pcActivateNMODE;
	delete m_pcActivateVguaranty;
	delete m_pcActivateVlimit;
	delete m_pcActivateTrends;
	delete m_pcActivateLungRec;
	delete m_pcActivateTherapy;
	delete m_pcActivatePRICO;
	delete m_pcActivateNIVTRIGGER;

	delete m_pcDelFOT;
	delete m_pcDelHFO;
	delete m_pcDelNMODE;
	delete m_pcDelVguaranty;
	delete m_pcDelVlimit;
	delete m_pcDelTrends;
	delete m_pcDelLungRec;
	delete m_pcDelTherapy;
	delete m_pcDelPRICO;
	delete m_pcDelNIVTRIGGER;

	m_pcActivateHFO=NULL;
	m_pcActivateFOT=NULL;
	m_pcActivateNMODE=NULL;
	m_pcActivateVguaranty=NULL;
	m_pcActivateVlimit=NULL;
	m_pcActivateTrends=NULL;
	m_pcActivateLungRec=NULL;
	m_pcActivateTherapy=NULL;
	m_pcActivatePRICO=NULL;
	m_pcActivateNIVTRIGGER=NULL;

	//m_pcDelMaster=NULL;
	m_pcDelHFO=NULL;
	m_pcDelFOT=NULL;
	m_pcDelNMODE=NULL;
	m_pcDelVguaranty=NULL;
	m_pcDelVlimit=NULL;
	m_pcDelTrends=NULL;
	m_pcDelLungRec=NULL;
	m_pcDelTherapy=NULL;
	m_pcDelPRICO=NULL;
	m_pcDelNIVTRIGGER=NULL;

	delete m_pcActivate_Up;
	delete m_pcActivate_Dw;

	m_pcActivate_Up=NULL;
	m_pcActivate_Dw=NULL;
}

BEGIN_MESSAGE_MAP(CWndServiceLicensing, CWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SERVICE_FOT_KEY, &CWndServiceLicensing::OnBnClickedActivateFOT)
	ON_BN_CLICKED(IDC_BTN_SERVICE_NIVTRIG_KEY, &CWndServiceLicensing::OnBnClickedActivateNIVTRIGGER)
	ON_BN_CLICKED(IDC_BTN_SERVICE_HFO_KEY, &CWndServiceLicensing::OnBnClickedActivateHFO)
	ON_BN_CLICKED(IDC_BTN_SERVICE_NMODE_KEY, &CWndServiceLicensing::OnBnClickedActivateNMODE)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VGUARANTY_KEY, &CWndServiceLicensing::OnBnClickedActivateVGUARANTY)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VLIMIT_KEY, &CWndServiceLicensing::OnBnClickedActivateVLIMIT)
	ON_BN_CLICKED(IDC_BTN_SERVICE_TRENDS_KEY, &CWndServiceLicensing::OnBnClickedActivateTRENDS)
	ON_BN_CLICKED(IDC_BTN_SERVICE_THERAPY_KEY, &CWndServiceLicensing::OnBnClickedActivateTHERAPY)
	ON_BN_CLICKED(IDC_BTN_SERVICE_PRICO_KEY, &CWndServiceLicensing::OnBnClickedActivatePRICO)
	ON_BN_CLICKED(IDC_BTN_SERVICE_LUNGREC_KEY, &CWndServiceLicensing::OnBnClickedActivateLUNGREC)
	ON_BN_CLICKED(IDC_BTN_SERVICE_FOT_DELKEY, &CWndServiceLicensing::OnBnClickedDelFOT)
	ON_BN_CLICKED(IDC_BTN_SERVICE_NIVTRIG_DELKEY, &CWndServiceLicensing::OnBnClickedDelNIVTRIGGER)
	ON_BN_CLICKED(IDC_BTN_SERVICE_HFO_DELKEY, &CWndServiceLicensing::OnBnClickedDelHFO)
	ON_BN_CLICKED(IDC_BTN_SERVICE_NMODE_DELKEY, &CWndServiceLicensing::OnBnClickedDelNMODE)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VGUARANTY_DELKEY, &CWndServiceLicensing::OnBnClickedDelVGUARANTY)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VLIMIT_DELKEY, &CWndServiceLicensing::OnBnClickedDelVLIMIT)
	ON_BN_CLICKED(IDC_BTN_SERVICE_TRENDS_DELKEY, &CWndServiceLicensing::OnBnClickedDelTRENDS)
	ON_BN_CLICKED(IDC_BTN_SERVICE_THERAPY_DELKEY, &CWndServiceLicensing::OnBnClickedDelTHERAPY)
	ON_BN_CLICKED(IDC_BTN_SERVICE_LUNGREC_DELKEY, &CWndServiceLicensing::OnBnClickedDelLUNGREC)
	ON_BN_CLICKED(IDC_BTN_SERVICE_PRICO_DELKEY, &CWndServiceLicensing::OnBnClickedDelPRICO)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::Init()
{
	CClientDC dc(this);

	CStringA szUniqueID16=getModel()->GetUniqueID();
	m_szLicenseFile=_T("\\FFSDISK\\AL");
	m_szLicenseFile+=szUniqueID16;
	m_szLicenseFile+=_T(".alic");

	int iLenUniqueID=szUniqueID16.GetLength();

	if(iLenUniqueID>16)
	{
		theApp.WriteLog(_T("#HFO:0282"));
	}

	for(iLenUniqueID;iLenUniqueID<16;iLenUniqueID++)
	{
		szUniqueID16.AppendChar('0');
	}

	bool bCheckVGUARANTFeature=false;
	bool bCheckVLIMITFeature=false;
	bool bCheckHFOFeature=false;
	bool bCheckNMODEFeature=false;
	bool bCheckLUNGRECFeature=false;
	bool bCheckTRENDSFeature=false;
	bool bCheckTHERAPYFeature=false;
	bool bCheckPRICOFeature=false;
	bool bCheckFOTFeature=false;
	bool bCheckNIVTRIGGERFeature=false;

	if(CTlsFile::Exists(m_szLicenseFile))
	{
		CTlsIStream_File is(m_szLicenseFile);
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
			CTlsXmlScanner::eTokenType Type;
			CString sKey;
			CString sValue;

			CString sModuleName=_T("");
			CString sModuleKey=_T("");

			while(true)
			{
				if(!scanner.Parse(scanner, Type, sKey, sValue))
				{
					//return false;
					break;
				}

				if(Type == CTlsXmlScanner::TT_BS)
				{
					if(sKey == "license")
					{
						while(true)
						{
							sModuleName=_T("");
							sModuleKey=_T("");

							if(!scanner.Parse(scanner, Type, sKey, sValue))
							{
								//return false;
								break;
							}

							if(Type == CTlsXmlScanner::TT_ATT)
							{
								if(sKey == "module")
								{
									sModuleName=sValue;

								}
								else
								{
									CString sTemp = _T("Falsche Option: ");
									sTemp += sKey;
									sTemp += " ";
									sTemp += sValue;

									break;
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
								if(sKey == "key")
								{
									sModuleKey=sValue;
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

							if(sModuleName!=_T("") && sModuleKey!=_T(""))
							{
								if(sModuleName==_T("HFO"))
								{
									m_szParsedHFOKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckHFOFeature=true;
									}
								}
								else if(sModuleName==_T("CO2"))
								{
									/*m_szParsedCO2Key=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckCO2Feature=true;
									}*/
								}
								else if(sModuleName==_T("NMODE"))
								{
									m_szParsedNMODEKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckNMODEFeature=true;
									}
								}
								else if(sModuleName==_T("VGUARANTY"))
								{
									m_szParsedVGUARANTYKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckVGUARANTFeature=true;
									}
								}
								else if(sModuleName==_T("VLIMIT"))
								{
									m_szParsedVLIMITKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckVLIMITFeature=true;
									}
								}
								else if(sModuleName==_T("LUNGREC"))
								{
									m_szParsedLUNGRECKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckLUNGRECFeature=true;
									}
								}
								else if(sModuleName==_T("TRENDS"))
								{
									m_szParsedTRENDSKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckTRENDSFeature=true;
									}
								}
								else if(sModuleName==_T("THERAPY"))
								{
									m_szParsedTHERAPYKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckTHERAPYFeature=true;
									}
								}
								else if(sModuleName==_T("PRICO"))
								{
									m_szParsedPRICOKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckPRICOFeature=true;
									}
								}
								else if(sModuleName==_T("FOT"))
								{
									m_szParsedFOTKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckFOTFeature=true;
									}
								}
								else if(sModuleName==_T("NIVTRIGGER"))
								{
									m_szParsedNIVTRIGGERKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckNIVTRIGGERFeature=true;
									}
								}
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
	else
	{
		m_bVGUARANTModuleEnabled=false;
		m_bVLIMITModuleEnabled=false;
		m_bHFOModuleEnabled=false;
		m_bNMODEModuleEnabled=false;
		m_bLUNGRECModuleEnabled=false;
		m_bTRENDSModuleEnabled=false;
		m_bTHERAPYModuleEnabled=false;
		m_bPRICOModuleEnabled=false;
		m_bFOTModuleEnabled=false;
		m_bNIVTRIGGERModuleEnabled=false;
	}

	if(bCheckHFOFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isHFODemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_HFO);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedHFOKey==szHex)
			{
				m_bHFOModuleEnabled=true;
			}
		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_HFO);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedHFOKey==szHex)
			{
				m_bHFOModuleEnabled=true;
			}
		}
		m_szEncryptedFormattedHFOKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}
	if(bCheckNMODEFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isNMODEDemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_NMODE);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedNMODEKey==szHex)
			{
				m_bNMODEModuleEnabled=true;
			}
		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_NMODE);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedNMODEKey==szHex)
			{
				m_bNMODEModuleEnabled=true;
			}
		}
		m_szEncryptedFormattedNMODEKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}
	if(bCheckVGUARANTFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isVGUARANTDemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_VGUARANTY);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedVGUARANTYKey==szHex)
			{
				m_bVGUARANTModuleEnabled=true;
			}
		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_VGUARANTY);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedVGUARANTYKey==szHex)
			{
				m_bVGUARANTModuleEnabled=true;
			}
		}		
		m_szEncryptedFormattedVGUARANTYKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}
	if(bCheckVLIMITFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isVLIMITDemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_VLIMIT);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedVLIMITKey==szHex)
			{
				m_bVLIMITModuleEnabled=true;
			}

		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_VLIMIT);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedVLIMITKey==szHex)
			{
				m_bVLIMITModuleEnabled=true;
			}
		}
		m_szEncryptedFormattedVLIMITKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}
	if(bCheckLUNGRECFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isLUNGRECDemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_LUNGREC);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedLUNGRECKey==szHex)
			{
				m_bLUNGRECModuleEnabled=true;
			}
		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_LUNGREC);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedLUNGRECKey==szHex)
			{
				m_bLUNGRECModuleEnabled=true;
			}
		}		
		m_szEncryptedFormattedLUNGRECKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}
	if(bCheckTRENDSFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isTRENDDemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_TREND);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedTRENDSKey==szHex)
			{
				m_bTRENDSModuleEnabled=true;
			}
		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_TREND);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedTRENDSKey==szHex)
			{
				m_bTRENDSModuleEnabled=true;
			}
		}	
		m_szEncryptedFormattedTRENDSKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}
	if(bCheckTHERAPYFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isTHERAPYDemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_THERAPY);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedTHERAPYKey==szHex)
			{
				m_bTHERAPYModuleEnabled=true;
			}
		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_THERAPY);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedTHERAPYKey==szHex)
			{
				m_bTHERAPYModuleEnabled=true;
			}
		}		
		m_szEncryptedFormattedTHERAPYKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}
	if(bCheckPRICOFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isPRICODemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_PRICO);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedPRICOKey==szHex)
			{
				m_bPRICOModuleEnabled=true;
			}
		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_PRICO);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedPRICOKey==szHex)
			{
				m_bPRICOModuleEnabled=true;
			}
		}		
		m_szEncryptedFormattedPRICOKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}
	if(bCheckFOTFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isFOTDemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_FOT);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedFOTKey==szHex)
			{
				m_bFOTModuleEnabled=true;
			}
		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_FOT);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedFOTKey==szHex)
			{
				m_bFOTModuleEnabled=true;
			}
		}		
		m_szEncryptedFormattedFOTKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}
	if(bCheckNIVTRIGGERFeature)
	{
		CStringA szEncryptedKey="";
		char szHex[30];

		if(false==getModel()->getDATAHANDLER()->isNIVTRIGGERDemoLicAvailable())
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptKey(MOD_NIVTRIGGER);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedNIVTRIGGERKey==szHex)
			{
				m_bNIVTRIGGERModuleEnabled=true;
			}
		}
		else
		{
			szEncryptedKey=getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_NIVTRIGGER);
			strcpy_s(szHex,_countof(szHex),szEncryptedKey);

			if(m_szParsedNIVTRIGGERKey==szHex)
			{
				m_bNIVTRIGGERModuleEnabled=true;
			}
		}		
		m_szEncryptedFormattedNIVTRIGGERKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedKey);
	}

	m_pcActivate_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELSETTING_UP);
	m_pcActivate_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELSETTING_DW);

	BTN btn;

	btn.wID					= IDC_BTN_SERVICE_FOT_KEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 40;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivateFOT=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivateFOT->Create(this,g_hf15AcuMed,0);
	m_pcActivateFOT->SetText(_T("activate"));
	m_pcActivateFOT->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_SERVICE_FOT_DELKEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 40;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelFOT=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelFOT->Create(this,g_hf15AcuMed,0);
	m_pcDelFOT->SetText(_T("delete"));
	m_pcDelFOT->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_SERVICE_HFO_KEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 100;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivateHFO=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivateHFO->Create(this,g_hf15AcuMed,0);
	m_pcActivateHFO->SetText(_T("activate"));
	m_pcActivateHFO->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SERVICE_HFO_DELKEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 100;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelHFO=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelHFO->Create(this,g_hf15AcuMed,0);
	m_pcDelHFO->SetText(_T("delete"));
	m_pcDelHFO->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_SERVICE_NMODE_KEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 160;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivateNMODE=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivateNMODE->Create(this,g_hf15AcuMed,0);
	m_pcActivateNMODE->SetText(_T("activate"));
	m_pcActivateNMODE->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SERVICE_NMODE_DELKEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 160;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelNMODE=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelNMODE->Create(this,g_hf15AcuMed,0);
	m_pcDelNMODE->SetText(_T("delete"));
	m_pcDelNMODE->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_SERVICE_VGUARANTY_KEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 220;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivateVguaranty=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivateVguaranty->Create(this,g_hf15AcuMed,0);
	m_pcActivateVguaranty->SetText(_T("activate"));
	m_pcActivateVguaranty->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SERVICE_VGUARANTY_DELKEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 220;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelVguaranty=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelVguaranty->Create(this,g_hf15AcuMed,0);
	m_pcDelVguaranty->SetText(_T("delete"));
	m_pcDelVguaranty->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_SERVICE_VLIMIT_KEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 280;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivateVlimit=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivateVlimit->Create(this,g_hf15AcuMed,0);
	m_pcActivateVlimit->SetText(_T("activate"));
	m_pcActivateVlimit->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SERVICE_VLIMIT_DELKEY;	
	btn.poPosition.x		= 30;
	btn.poPosition.y		= 280;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelVlimit=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelVlimit->Create(this,g_hf15AcuMed,0);
	m_pcDelVlimit->SetText(_T("delete"));
	m_pcDelVlimit->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_SERVICE_LUNGREC_KEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 40;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivateLungRec=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivateLungRec->Create(this,g_hf15AcuMed,0);
	m_pcActivateLungRec->SetText(_T("activate"));
	m_pcActivateLungRec->ShowWindow(SW_SHOW);
	

	btn.wID					= IDC_BTN_SERVICE_LUNGREC_DELKEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 40;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelLungRec=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelLungRec->Create(this,g_hf15AcuMed,0);
	m_pcDelLungRec->SetText(_T("delete"));
	m_pcDelLungRec->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_SERVICE_TRENDS_KEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 100;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivateTrends=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivateTrends->Create(this,g_hf15AcuMed,0);
	m_pcActivateTrends->SetText(_T("activate"));
	m_pcActivateTrends->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SERVICE_TRENDS_DELKEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 100;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelTrends=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelTrends->Create(this,g_hf15AcuMed,0);
	m_pcDelTrends->SetText(_T("delete"));
	m_pcDelTrends->ShowWindow(SW_HIDE);



	btn.wID					= IDC_BTN_SERVICE_THERAPY_KEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 160;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivateTherapy=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivateTherapy->Create(this,g_hf15AcuMed,0);
	m_pcActivateTherapy->SetText(_T("activate"));
	m_pcActivateTherapy->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SERVICE_THERAPY_DELKEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 160;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelTherapy=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelTherapy->Create(this,g_hf15AcuMed,0);
	m_pcDelTherapy->SetText(_T("delete"));
	m_pcDelTherapy->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_SERVICE_PRICO_KEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 220;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivatePRICO=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivatePRICO->Create(this,g_hf15AcuMed,0);
	m_pcActivatePRICO->SetText(_T("activate"));
	m_pcActivatePRICO->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SERVICE_PRICO_DELKEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 220;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelPRICO=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelPRICO->Create(this,g_hf15AcuMed,0);
	m_pcDelPRICO->SetText(_T("delete"));
	m_pcDelPRICO->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_SERVICE_NIVTRIG_KEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 280;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcActivateNIVTRIGGER=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcActivateNIVTRIGGER->Create(this,g_hf15AcuMed,0);
	m_pcActivateNIVTRIGGER->SetText(_T("activate"));
	m_pcActivateNIVTRIGGER->ShowWindow(SW_SHOW);

	
	btn.wID					= IDC_BTN_SERVICE_NIVTRIG_DELKEY;	
	btn.poPosition.x		= 350;
	btn.poPosition.y		= 280;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelNIVTRIGGER=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelNIVTRIGGER->Create(this,g_hf15AcuMed,0);
	m_pcDelNIVTRIGGER->SetText(_T("delete"));
	m_pcDelNIVTRIGGER->ShowWindow(SW_HIDE);

	m_pcMenuBack->ShowWindow(SW_SHOW);
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush cbrBack(RGB(181,178,181));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf14AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 10, 10, 770, 470,20,20);

	CStringW szDev = _T("");
	szDev+=getModel()->GetUniqueID();

	rc.left = 30;  
	rc.top = 480;  
	rc.right  = 500;  
	rc.bottom = 520;
	CStringW csLicense = _T("Licensing ");
	CStringW csDemo = _T("");
	csLicense += getModel()->GetLanguageString(IDS_TXT_DEVID);
	csLicense+=_T(": ");
	csLicense += szDev;
	DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	bool bDemoAvailable=false;
	SelectObject(hdcMem,g_hf14AcuMed);

	rc.left = 135;  
	rc.right  = 350;  
	rc.top = 40;  
	rc.bottom = 88;
	csLicense = _T("FOT");
	if(getModel()->getDATAHANDLER()->isFOTDemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetFOTdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	
	if(m_bFOTModuleEnabled)
	{
		m_pcActivateFOT->ShowWindow(SW_HIDE);
		m_pcDelFOT->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateFOT->ShowWindow(SW_SHOW);
		m_pcDelFOT->ShowWindow(SW_HIDE);
	}

	bDemoAvailable=false;
	rc.left = 135;  
	rc.right  = 350; 
	rc.top = 100;  
	rc.bottom = 148;
	csLicense = _T("HFO");
	if(getModel()->getDATAHANDLER()->isHFODemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetHFOdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	if(m_bHFOModuleEnabled)
	{
		m_pcActivateHFO->ShowWindow(SW_HIDE);
		m_pcDelHFO->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateHFO->ShowWindow(SW_SHOW);
		m_pcDelHFO->ShowWindow(SW_HIDE);
	}

	
	bDemoAvailable=false;
	rc.left = 135;  
	rc.right  = 350;  
	rc.top = 160;  
	rc.bottom = 208;
	csLicense = _T("NMode");
	if(getModel()->getDATAHANDLER()->isNMODEDemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetNMODEdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	if(m_bNMODEModuleEnabled)
	{
		m_pcActivateNMODE->ShowWindow(SW_HIDE);
		m_pcDelNMODE->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateNMODE->ShowWindow(SW_SHOW);
		m_pcDelNMODE->ShowWindow(SW_HIDE);
	}

	bDemoAvailable=false;
	rc.left = 135;  
	rc.right  = 350;  
	rc.top = 220;  
	rc.bottom = 268;
	csLicense = _T("Vol. Guarantee");
	if(getModel()->getDATAHANDLER()->isVGUARANTDemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetVGUARANTYdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	if(m_bVGUARANTModuleEnabled)
	{
		m_pcActivateVguaranty->ShowWindow(SW_HIDE);

		if(false==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
		{
			m_pcDelVguaranty->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcDelVguaranty->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_pcActivateVguaranty->ShowWindow(SW_SHOW);
		m_pcDelVguaranty->ShowWindow(SW_HIDE);
	}

	bDemoAvailable=false;
	rc.left = 135;  
	rc.right  = 350;  
	rc.top = 280;  
	rc.bottom = 328;
	csLicense = _T("Vol.Limit");
	if(getModel()->getDATAHANDLER()->isVLIMITDemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetVLIMITdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	if(m_bVLIMITModuleEnabled)
	{
		m_pcActivateVlimit->ShowWindow(SW_HIDE);
		m_pcDelVlimit->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateVlimit->ShowWindow(SW_SHOW);

		if(false==getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn())
		{
			m_pcDelVlimit->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcDelVlimit->ShowWindow(SW_HIDE);
		}
		m_pcDelVlimit->ShowWindow(SW_HIDE);
	}

	bDemoAvailable=false;
	rc.left = 455;  
	rc.right  = 800;  
	rc.top = 40;  
	rc.bottom = 88;
	csLicense = _T("Lung rec.");
	if(getModel()->getDATAHANDLER()->isLUNGRECDemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetLUNGRECdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	if(m_bLUNGRECModuleEnabled)
	{
		m_pcActivateLungRec->ShowWindow(SW_HIDE);
		m_pcDelLungRec->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateLungRec->ShowWindow(SW_SHOW);

		if(getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()==0)
		{
			m_pcDelLungRec->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcDelLungRec->ShowWindow(SW_HIDE);
		}
		m_pcDelLungRec->ShowWindow(SW_HIDE);
	}

	bDemoAvailable=false;
	rc.left = 455;  
	rc.right  = 800;   
	rc.top = 100;  
	rc.bottom = 148;
	csLicense = _T("Trend");
	if(getModel()->getDATAHANDLER()->isTRENDDemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetTRENDdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	if(m_bTRENDSModuleEnabled)
	{
		m_pcActivateTrends->ShowWindow(SW_HIDE);
		m_pcDelTrends->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateTrends->ShowWindow(SW_SHOW);
		m_pcDelTrends->ShowWindow(SW_HIDE);
	}

	bDemoAvailable=false;
	rc.left = 455;  
	rc.right  = 800;  
	rc.top = 160;  
	rc.bottom = 208;
	csLicense = _T("O2 Therapy");
	if(getModel()->getDATAHANDLER()->isTHERAPYDemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetTHERAPYdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	if(m_bTHERAPYModuleEnabled)
	{
		m_pcActivateTherapy->ShowWindow(SW_HIDE);
		m_pcDelTherapy->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateTherapy->ShowWindow(SW_SHOW);
		m_pcDelTherapy->ShowWindow(SW_HIDE);
	}

	bDemoAvailable=false;
	rc.left = 455;  
	rc.right  = 800;   
	rc.top = 220;  
	rc.bottom = 268;
	csLicense = _T("PRICO");
	if(getModel()->getDATAHANDLER()->isPRICODemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetPRICOdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	if(m_bPRICOModuleEnabled)
	{
		m_pcActivatePRICO->ShowWindow(SW_HIDE);
		
		if(false==getModel()->getPRICOThread()->isPRICOalgorithmRunning())
		{
			m_pcDelPRICO->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcDelPRICO->ShowWindow(SW_HIDE);

			rc.left = 25;  
			rc.right  = 400;  
			csLicense = _T("- PRICO active -");
			DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
	}
	else
	{
		m_pcActivatePRICO->ShowWindow(SW_SHOW);
		m_pcDelPRICO->ShowWindow(SW_HIDE);
	}

	bDemoAvailable=false;
	rc.left = 455;  
	rc.right  = 800;  
	rc.top = 280;  
	rc.bottom = 328;
	csLicense = _T("NIV trigger");
	if(getModel()->getDATAHANDLER()->isNIVTRIGGERDemoLicAvailable())
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetNIVTRIGGERdemoTimestamp();

		csDemo=_T("(ERROR)");

		if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
		{
			if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
			{
				COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

				int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
				if(iDaysLeft<0)
					iDaysLeft=0;
				csDemo.Format(_T(" (%d days left)"), iDaysLeft);
			}
		}

		bDemoAvailable=true;
	}
	if(bDemoAvailable)
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawText(hdcMem,csDemo,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,csLicense,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}


	if(m_bNIVTRIGGERModuleEnabled)
	{
		m_pcActivateNIVTRIGGER->ShowWindow(SW_HIDE);
		m_pcDelNIVTRIGGER->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateNIVTRIGGER->ShowWindow(SW_SHOW);
		m_pcDelNIVTRIGGER->ShowWindow(SW_HIDE);
	}

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	penLine.DeleteObject();

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

/**********************************************************************************************//**
 * Executes the button clicked delete nivtrigger action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelNIVTRIGGER()
{
	CDlgMessageBox box(this,_T("Delete NIVTRIGGER license?"),_T("This will delete the license of the NIVTRIGGER module. Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();


	if(iRes==IDYES)
	{
		m_szParsedNIVTRIGGERKey=_T("0");
		m_bNIVTRIGGERModuleEnabled=false;
		m_szEncryptedFormattedNIVTRIGGERKey=_T("");

		getModel()->getDATAHANDLER()->disableNIVTRIGGERLicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_NIVTRIGGER,false);

		getModel()->getDATAHANDLER()->WriteLicenseFile();

		getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked delete fot action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelFOT()
{
	CDlgMessageBox box(this,_T("Delete FOT license?"),_T("This will delete the license of the FOT module.  FOT module will be disabled! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();


	if(iRes==IDYES)
	{
		m_szParsedFOTKey=_T("0");
		m_bFOTModuleEnabled=false;
		m_szEncryptedFormattedFOTKey=_T("");

		getModel()->getDATAHANDLER()->disableFOTLicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_FOT,false);

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked delete hfo action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelHFO()
{
	CDlgMessageBox box(this,_T("Delete HFO license?"),_T("This will delete the license of the HFO module. HFO module will be disabled! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();

	if(iRes==IDYES)
	{
		m_szParsedHFOKey=_T("0");
		m_bHFOModuleEnabled=false;
		m_szEncryptedFormattedHFOKey=_T("");
		
		getModel()->getDATAHANDLER()->disableHFOLicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_HFO,false);

		if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true)
		{
			m_szParsedLUNGRECKey=_T("0");
			m_bLUNGRECModuleEnabled=false;
			m_szEncryptedFormattedLUNGRECKey=_T("");
			getModel()->getDATAHANDLER()->disableLUNGRECLicense();
			getModel()->getDATAHANDLER()->delDemoLicense(MOD_HFO,false);
		}
		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked delete nmode action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelNMODE()
{
	CDlgMessageBox box(this,_T("Delete NMODE license?"),_T("This will delete the license of the NMODE module. NMODE module will be disabled! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();

	if(iRes==IDYES)
	{
		m_szParsedNMODEKey=_T("0");
		m_bNMODEModuleEnabled=false;
		m_szEncryptedFormattedNMODEKey=_T("");

		getModel()->getDATAHANDLER()->disableNMODELicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_NMODE,false);

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked delete vguaranty action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelVGUARANTY()
{
	CDlgMessageBox box(this,_T("Delete Volumen guaranty license?"),_T("This will delete the license of the Volumen guaranty module. Volumen guaranty module will be disabled! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();

	if(iRes==IDYES)
	{
		m_szParsedVGUARANTYKey=_T("0");
		m_bVGUARANTModuleEnabled=false;
		m_szEncryptedFormattedVGUARANTYKey=_T("");

		getModel()->getDATAHANDLER()->disableVGUARANTLicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_VGUARANTY,false);

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked delete vlimit action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelVLIMIT()
{
	CDlgMessageBox box(this,_T("Delete Volumen limit license?"),_T("This will delete the license of the Volumen limit module. Volumen limit module will be disabled! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();

	if(iRes==IDYES)
	{
		m_szParsedVLIMITKey=_T("0");
		m_bVLIMITModuleEnabled=false;
		m_szEncryptedFormattedVLIMITKey=_T("");

		getModel()->getDATAHANDLER()->disableVLIMITLicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_VLIMIT,false);

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked delete trends action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelTRENDS()
{
	CDlgMessageBox box(this,_T("Delete Trend license?"),_T("This will delete the license of the trend module. Trend module will be disabled! All trend data will be deleted too! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();

	if(iRes==IDYES)
	{
		m_szParsedTRENDSKey=_T("0");
		m_bTRENDSModuleEnabled=false;
		m_szEncryptedFormattedTRENDSKey=_T("");

		getModel()->getDATAHANDLER()->disableTRENDLicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_TREND,false);

		getModel()->getDATAHANDLER()->WriteLicenseFile();

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_DELETE_TRENDS);
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked delete therapy action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelTHERAPY()
{
	CDlgMessageBox box(this,_T("Delete O2 Therapy license?"),_T("This will delete the license of the O2 Therapy module. O2 Therapy module will be disabled! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();

	if(iRes==IDYES)
	{
		m_szParsedTHERAPYKey=_T("0");
		m_bTHERAPYModuleEnabled=false;
		m_szEncryptedFormattedTHERAPYKey=_T("");

		getModel()->getDATAHANDLER()->disableTHERAPYLicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_THERAPY,false);

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked delete prico action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelPRICO()
{
	CDlgMessageBox box(this,_T("Delete PRICO license?"),_T("This will delete the license of the PRICO module. PRICO module will be disabled! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();

	if(iRes==IDYES)
	{
		m_szParsedPRICOKey=_T("0");
		m_bPRICOModuleEnabled=false;
		m_szEncryptedFormattedPRICOKey=_T("");

		getModel()->getDATAHANDLER()->disablePRICOLicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_PRICO,false);

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked delete lungrec action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedDelLUNGREC()
{
	CDlgMessageBox box(this,_T("Delete lung recruitment license?"),_T("This will delete the license of the lung recruitment module. Lung recruitment module will be disabled! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();

	if(iRes==IDYES)
	{
		m_szParsedLUNGRECKey=_T("0");
		m_bLUNGRECModuleEnabled=false;
		m_szEncryptedFormattedLUNGRECKey=_T("");
		
		getModel()->getDATAHANDLER()->SetFreqRecParadata(0);
		getModel()->getDATAHANDLER()->disableLUNGRECLicense();
		getModel()->getDATAHANDLER()->delDemoLicense(MOD_LUNGREC,false);

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate nivtrigger action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivateNIVTRIGGER()
{
	CPasswordDlg pwDlg(this,MOD_NIVTRIGGER);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;


	if(iRes==IDOK)
	{
		m_szParsedNIVTRIGGERKey=getModel()->getDATAHANDLER()->encryptKey(MOD_NIVTRIGGER);
		m_szEncryptedFormattedNIVTRIGGERKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedNIVTRIGGERKey);

		getModel()->getDATAHANDLER()->enableNIVTRIGGERLicense();
		m_bNIVTRIGGERModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}

	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate fot action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivateFOT()
{
	CPasswordDlg pwDlg(this,MOD_FOT);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;
	

	if(iRes==IDOK)
	{
		m_szParsedFOTKey=getModel()->getDATAHANDLER()->encryptKey(MOD_FOT);
		m_szEncryptedFormattedFOTKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedFOTKey);

		getModel()->getDATAHANDLER()->enableFOTLicense();
		m_bFOTModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}
	
	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate hfo action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivateHFO()
{
	CPasswordDlg pwDlg(this,MOD_HFO);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;


	if(iRes==IDOK)
	{
		m_szParsedHFOKey=getModel()->getDATAHANDLER()->encryptKey(MOD_HFO);
		m_szEncryptedFormattedHFOKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedHFOKey);

		getModel()->getDATAHANDLER()->enableHFOLicense();
		m_bHFOModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();

	}
	
	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate nmode action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivateNMODE()
{
	CPasswordDlg pwDlg(this,MOD_NMODE);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;


	if(iRes==IDOK)
	{
		m_szParsedNMODEKey=getModel()->getDATAHANDLER()->encryptKey(MOD_NMODE);
		m_szEncryptedFormattedNMODEKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedNMODEKey);

		getModel()->getDATAHANDLER()->enableNMODELicense();
		m_bNMODEModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();

	}
	
	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate vguaranty action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivateVGUARANTY()
{
	CPasswordDlg pwDlg(this,MOD_VGUARANTY);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;

	if(iRes==IDOK)
	{
		m_szParsedVGUARANTYKey=getModel()->getDATAHANDLER()->encryptKey(MOD_VGUARANTY);
		m_szEncryptedFormattedVGUARANTYKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedVGUARANTYKey);

		getModel()->getDATAHANDLER()->enableVGUARANTLicense();
		m_bVGUARANTModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();

	}
	
	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate vlimit action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivateVLIMIT()
{
	CPasswordDlg pwDlg(this,MOD_VLIMIT);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;

	if(iRes==IDOK)
	{
		m_szParsedVLIMITKey=getModel()->getDATAHANDLER()->encryptKey(MOD_VLIMIT);
		m_szEncryptedFormattedVLIMITKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedVLIMITKey);

		getModel()->getDATAHANDLER()->enableVLIMITLicense();
		m_bVLIMITModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();

	}
	
	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate trends action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivateTRENDS()
{
	CPasswordDlg pwDlg(this,MOD_TREND);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;

	if(iRes==IDOK)
	{
		m_szParsedTRENDSKey=getModel()->getDATAHANDLER()->encryptKey(MOD_TREND);
		m_szEncryptedFormattedTRENDSKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedTRENDSKey);

		getModel()->getDATAHANDLER()->enableTRENDLicense();
		m_bTRENDSModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();

	}
	
	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate therapy action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivateTHERAPY()
{
	CPasswordDlg pwDlg(this,MOD_THERAPY);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;

	if(iRes==IDOK)
	{
		m_szParsedTHERAPYKey=getModel()->getDATAHANDLER()->encryptKey(MOD_THERAPY);
		m_szEncryptedFormattedTHERAPYKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedTHERAPYKey);

		getModel()->getDATAHANDLER()->enableTHERAPYLicense();
		m_bTHERAPYModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}
	
	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate prico action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivatePRICO()
{
	CPasswordDlg pwDlg(this,MOD_PRICO);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;

	if(iRes==IDOK)
	{
		m_szParsedPRICOKey=getModel()->getDATAHANDLER()->encryptKey(MOD_PRICO);
		m_szEncryptedFormattedPRICOKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedPRICOKey);

		getModel()->getDATAHANDLER()->enablePRICOLicense();
		m_bPRICOModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();

	}
	
	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the button clicked activate lungrec action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnBnClickedActivateLUNGREC()
{
	if(getModel()->getDATAHANDLER()->isHFOLicenseAvailable()==false)
	{
		CDlgMessageBox box(this,_T("Not possible!"),_T("You need to enable HFO first!"),MB_OK,IDB_MSG_STOP);
		m_pDlg = &box;
		int iRes=box.DoModal();
		m_pDlg = NULL;
		Invalidate();
		UpdateWindow();
		Draw();

		return;
	}
	CPasswordDlg pwDlg(this,MOD_LUNGREC);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;

	if(iRes==IDOK)
	{
		m_szParsedLUNGRECKey=getModel()->getDATAHANDLER()->encryptKey(MOD_LUNGREC);
		m_szEncryptedFormattedLUNGRECKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(m_szParsedLUNGRECKey);

		getModel()->getDATAHANDLER()->enableLUNGRECLicense();
		m_bLUNGRECModuleEnabled=true;

		getModel()->getDATAHANDLER()->WriteLicenseFile();
	}
	
	Invalidate();
	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceLicensing::OnDestroy()
{
	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	CWndService::OnDestroy();
}
