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


// CWndServiceLicensing

IMPLEMENT_DYNAMIC(CWndServiceLicensing, CWnd)

CWndServiceLicensing::CWndServiceLicensing():
CWndService()
{
	/*m_sliderEnable=NULL;
	m_bNurscallActive=false;
	m_pcNurse=NULL;*/

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

	//m_bMasterModuleEnabled=false;
	//m_bCO2ModuleEnabled=false;
	m_bVGUARANTModuleEnabled=false;
	m_bVLIMITModuleEnabled=false;
	m_bHFOModuleEnabled=false;
	m_bNMODEModuleEnabled=false;
	m_bTRENDSModuleEnabled=false;
	m_bLUNGRECModuleEnabled=false;
	m_bTHERAPYModuleEnabled=false;
	m_bPRICOModuleEnabled=false;
	m_bFOTModuleEnabled=false;

	//m_szEncryptedFormattedMasterKey=_T("");
	m_szEncryptedFormattedHFOKey=_T("");
	//m_szEncryptedFormattedCO2Key=_T("");
	m_szEncryptedFormattedNMODEKey=_T("");
	m_szEncryptedFormattedVGUARANTYKey=_T("");
	m_szEncryptedFormattedVLIMITKey=_T("");
	m_szEncryptedFormattedTRENDSKey=_T("");
	m_szEncryptedFormattedLUNGRECKey=_T("");
	m_szEncryptedFormattedTHERAPYKey=_T("");
	m_szEncryptedFormattedPRICOKey=_T("");
	m_szEncryptedFormattedFOTKey=_T("");
}

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

	delete m_pcDelFOT;
	delete m_pcDelHFO;
	//delete m_pcDelCO2;
	delete m_pcDelNMODE;
	delete m_pcDelVguaranty;
	delete m_pcDelVlimit;
	delete m_pcDelTrends;
	delete m_pcDelLungRec;
	delete m_pcDelTherapy;
	delete m_pcDelPRICO;

	//m_pcActivateMaster=NULL;
	m_pcActivateHFO=NULL;
	//m_pcActivateCO2=NULL;
	m_pcActivateNMODE=NULL;
	m_pcActivateVguaranty=NULL;
	m_pcActivateVlimit=NULL;
	m_pcActivateTrends=NULL;
	m_pcActivateLungRec=NULL;
	m_pcActivateTherapy=NULL;
	m_pcActivatePRICO=NULL;

	//m_pcDelMaster=NULL;
	m_pcDelHFO=NULL;
	//m_pcDelCO2=NULL;
	m_pcDelNMODE=NULL;
	m_pcDelVguaranty=NULL;
	m_pcDelVlimit=NULL;
	m_pcDelTrends=NULL;
	m_pcDelLungRec=NULL;
	m_pcDelTherapy=NULL;
	m_pcDelPRICO=NULL;

	delete m_pcActivate_Up;
	delete m_pcActivate_Dw;

	m_pcActivate_Up=NULL;
	m_pcActivate_Dw=NULL;
}

BEGIN_MESSAGE_MAP(CWndServiceLicensing, CWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SERVICE_FOT_KEY, &CWndServiceLicensing::OnBnClickedActivateFOT)
	ON_BN_CLICKED(IDC_BTN_SERVICE_HFO_KEY, &CWndServiceLicensing::OnBnClickedActivateHFO)
	ON_BN_CLICKED(IDC_BTN_SERVICE_NMODE_KEY, &CWndServiceLicensing::OnBnClickedActivateNMODE)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VGUARANTY_KEY, &CWndServiceLicensing::OnBnClickedActivateVGUARANTY)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VLIMIT_KEY, &CWndServiceLicensing::OnBnClickedActivateVLIMIT)
	ON_BN_CLICKED(IDC_BTN_SERVICE_TRENDS_KEY, &CWndServiceLicensing::OnBnClickedActivateTRENDS)
	ON_BN_CLICKED(IDC_BTN_SERVICE_THERAPY_KEY, &CWndServiceLicensing::OnBnClickedActivateTHERAPY)
	ON_BN_CLICKED(IDC_BTN_SERVICE_PRICO_KEY, &CWndServiceLicensing::OnBnClickedActivatePRICO)
	ON_BN_CLICKED(IDC_BTN_SERVICE_LUNGREC_KEY, &CWndServiceLicensing::OnBnClickedActivateLUNGREC)
	ON_BN_CLICKED(IDC_BTN_SERVICE_FOT_DELKEY, &CWndServiceLicensing::OnBnClickedDelFOT)
	ON_BN_CLICKED(IDC_BTN_SERVICE_HFO_DELKEY, &CWndServiceLicensing::OnBnClickedDelHFO)
	ON_BN_CLICKED(IDC_BTN_SERVICE_NMODE_DELKEY, &CWndServiceLicensing::OnBnClickedDelNMODE)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VGUARANTY_DELKEY, &CWndServiceLicensing::OnBnClickedDelVGUARANTY)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VLIMIT_DELKEY, &CWndServiceLicensing::OnBnClickedDelVLIMIT)
	ON_BN_CLICKED(IDC_BTN_SERVICE_TRENDS_DELKEY, &CWndServiceLicensing::OnBnClickedDelTRENDS)
	ON_BN_CLICKED(IDC_BTN_SERVICE_THERAPY_DELKEY, &CWndServiceLicensing::OnBnClickedDelTHERAPY)
	ON_BN_CLICKED(IDC_BTN_SERVICE_LUNGREC_DELKEY, &CWndServiceLicensing::OnBnClickedDelLUNGREC)
	ON_BN_CLICKED(IDC_BTN_SERVICE_PRICO_DELKEY, &CWndServiceLicensing::OnBnClickedDelPRICO)
	/*ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SERVICE_TESTNURSECALL, &CWndServiceSettings::OnBnClickedTestNursecall)*/
END_MESSAGE_MAP()



// CWndServiceNetwork message handlers

// CWndServiceNursecall message handlers
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
		theApp.getLog()->WriteLine(_T("#HFO:0282"));
	}

	for(iLenUniqueID;iLenUniqueID<16;iLenUniqueID++)
	{
		szUniqueID16.AppendChar('0');
	}

	//CStringW szFile=_T("\\FFSDISK\\")+m_szLicenseFile;

	//bool bCheckMasterFeature=false;
	//bool bCheckCO2Feature=false;
	bool bCheckVGUARANTFeature=false;
	bool bCheckVLIMITFeature=false;
	bool bCheckHFOFeature=false;
	bool bCheckNMODEFeature=false;
	bool bCheckLUNGRECFeature=false;
	bool bCheckTRENDSFeature=false;
	bool bCheckTHERAPYFeature=false;
	bool bCheckPRICOFeature=false;
	bool bCheckFOTFeature=false;

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
		//m_bCO2ModuleEnabled=false;
		m_bVGUARANTModuleEnabled=false;
		m_bVLIMITModuleEnabled=false;
		m_bHFOModuleEnabled=false;
		m_bNMODEModuleEnabled=false;
		m_bLUNGRECModuleEnabled=false;
		m_bTRENDSModuleEnabled=false;
		m_bTHERAPYModuleEnabled=false;
		m_bPRICOModuleEnabled=false;
		m_bFOTModuleEnabled=false;
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

	m_pcActivate_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_UP);
	m_pcActivate_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DW);
	//m_pcActivate_Dis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DIS);


	BTN btn;

	btn.wID					= IDC_BTN_SERVICE_FOT_KEY;	
	btn.poPosition.x		= 380;//;586
	btn.poPosition.y		= 15;
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
	btn.poPosition.x		= 600;//;586
	btn.poPosition.y		= 15;
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
	btn.poPosition.x		= 380;//;586
	btn.poPosition.y		= 65;
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
	btn.poPosition.x		= 600;//;586
	btn.poPosition.y		= 65;
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
	btn.poPosition.x		= 380;//;586
	btn.poPosition.y		= 115;
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
	btn.poPosition.x		= 600;//;586
	btn.poPosition.y		= 115;
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
	btn.poPosition.x		= 380;//;586
	btn.poPosition.y		= 165;
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
	btn.poPosition.x		= 600;//;586
	btn.poPosition.y		= 165;
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
	btn.poPosition.x		= 380;//;586
	btn.poPosition.y		= 215;
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
	btn.poPosition.x		= 600;//;586
	btn.poPosition.y		= 215;
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
	btn.poPosition.x		= 380;//;586
	btn.poPosition.y		= 265;
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
	btn.poPosition.x		= 600;//;586
	btn.poPosition.y		= 265;
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
	btn.poPosition.x		= 380;//;586
	btn.poPosition.y		= 315;
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
	btn.poPosition.x		= 600;//;586
	btn.poPosition.y		= 315;
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
	btn.poPosition.x		= 380;//;586
	btn.poPosition.y		= 365;
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
	btn.poPosition.x		= 600;//;586
	btn.poPosition.y		= 365;
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
	btn.poPosition.x		= 380;//;586
	btn.poPosition.y		= 415;
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
	btn.poPosition.x		= 600;//;586
	btn.poPosition.y		= 415;
	btn.pcBmpUp				= m_pcActivate_Up;
	btn.pcBmpDown			= m_pcActivate_Dw;
	btn.pcBmpFocus			= m_pcActivate_Up;
	btn.pcBmpDisabled		= m_pcActivate_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDelPRICO=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDelPRICO->Create(this,g_hf15AcuMed,0);
	m_pcDelPRICO->SetText(_T("delete"));
	m_pcDelPRICO->ShowWindow(SW_HIDE);

	m_pcMenuBack->ShowWindow(SW_SHOW);
}

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

	//CDC* pDCMem=CDC::FromHandle(hdcMem);
	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 10, 10, 770, 470,20,20);

	/*if(m_bNurscallActive)
		m_pcNurse->Draw(hdcMem,360,55);*/

	CStringW szDev = _T("");
	szDev+=getModel()->GetUniqueID();

	rc.left = 30;  
	rc.top = 480;  
	rc.right  = 500;  
	rc.bottom = 520;
	CStringW cs = _T("Licensing ");
	cs += getModel()->GetLanguageString(IDS_TXT_DEVID);
	cs+=_T(": ");
	cs += szDev;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	SelectObject(hdcMem,g_hf14AcuMed);

	rc.left = 30;  
	rc.top = 30;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("FOT:");
	if(getModel()->getDATAHANDLER()->isFOTDemoLicAvailable())
	{
		cs += _T("(demo)");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	if(m_bFOTModuleEnabled)
	{
		m_pcActivateFOT->ShowWindow(SW_HIDE);
		rc.left = 260;  
		rc.top = 30;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("ok");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->isFOTDemoLicAvailable())
		{
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			COleDateTime dtCurrentTime(stTime);
			COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetFOTdemoTimestamp();

			CStringW szDiff=_T("ERROR time stamp");;

			if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
			{
				if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
				{
					COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

					int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
					szDiff.Format(_T("%d days left"), iDaysLeft);
				}
			}

			rc.left = 330;  
			rc.top = 30;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,szDiff,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			rc.left = 330;  
			rc.top = 30;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,m_szEncryptedFormattedFOTKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		

		if(getModel()->getFOTThread()->getFOTstate()==FOT_OFF)
		{
			m_pcDelFOT->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcDelFOT->ShowWindow(SW_HIDE);

			rc.left = 600;  
			rc.right  = 760;  
			rc.top = 15;  
			rc.bottom = 63;
			cs = _T("- FOT active -");
			DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
	}
	else
	{
		m_pcActivateFOT->ShowWindow(SW_SHOW);
		rc.left = 260;  
		rc.top = 30;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelFOT->ShowWindow(SW_HIDE);
	}

	rc.left = 30;  
	rc.top = 80;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("HFO:");
	if(getModel()->getDATAHANDLER()->isHFODemoLicAvailable())
	{
		cs += _T("(demo)");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bHFOModuleEnabled)
	{
		m_pcActivateHFO->ShowWindow(SW_HIDE);
		rc.left = 260;  
		rc.top = 80;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("ok");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->isHFODemoLicAvailable())
		{
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			COleDateTime dtCurrentTime(stTime);
			COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetHFOdemoTimestamp();

			CStringW szDiff=_T("ERROR time stamp");;

			if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
			{
				if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
				{
					COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

					int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
					szDiff.Format(_T("%d days left"), iDaysLeft);
				}
			}

			rc.left = 330;  
			rc.top = 80;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,szDiff,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			rc.left = 330;  
			rc.top = 80;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,m_szEncryptedFormattedHFOKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		
		//if(getModel()->getCONFIG()->GetPrevMode()!=VM_HFO)
		{
			m_pcDelHFO->ShowWindow(SW_SHOW);
		}
		/*else
		{
			m_pcDelHFO->ShowWindow(SW_HIDE);

			rc.left = 600;  
			rc.right  = 760;  
			rc.top = 65;  
			rc.bottom = 113;
			cs = _T("- HFO active -");
			DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}*/
	}
	else
	{
		m_pcActivateHFO->ShowWindow(SW_SHOW);
		rc.left = 260;  
		rc.top = 80;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelHFO->ShowWindow(SW_HIDE);

	}

	

	rc.left = 30;  
	rc.top = 130;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("NMode:");
	if(getModel()->getDATAHANDLER()->isNMODEDemoLicAvailable())
	{
		cs += _T("(demo)");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bNMODEModuleEnabled)
	{
		m_pcActivateNMODE->ShowWindow(SW_HIDE);
		rc.left = 260;  
		rc.top = 130;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("ok");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->isNMODEDemoLicAvailable())
		{
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			COleDateTime dtCurrentTime(stTime);
			COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetNMODEdemoTimestamp();

			CStringW szDiff=_T("ERROR time stamp");;

			if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
			{
				if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
				{
					COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

					int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
					szDiff.Format(_T("%d days left"), iDaysLeft);
				}
			}

			rc.left = 330;  
			rc.top = 130;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,szDiff,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			rc.left = 330;  
			rc.top = 130;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,m_szEncryptedFormattedNMODEKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		

		//if(getModel()->getCONFIG()->GetPrevMode()!=VM_NCPAP && getModel()->getCONFIG()->GetPrevMode()!=VM_DUOPAP)
		{
			m_pcDelNMODE->ShowWindow(SW_SHOW);
		}
		/*else
		{
			m_pcDelNMODE->ShowWindow(SW_HIDE);

			rc.left = 600;  
			rc.right  = 760;  
			rc.top = 115;  
			rc.bottom = 163;
			cs = _T("- NMODE active -");
			DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}*/
	}
	else
	{
		m_pcActivateNMODE->ShowWindow(SW_SHOW);
		rc.left = 260;  
		rc.top = 130;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelNMODE->ShowWindow(SW_HIDE);
	}

	rc.left = 30;  
	rc.top = 180;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Volume guaranty:");
	if(getModel()->getDATAHANDLER()->isVGUARANTDemoLicAvailable())
	{
		cs += _T("(demo)");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bVGUARANTModuleEnabled)
	{
		m_pcActivateVguaranty->ShowWindow(SW_HIDE);
		rc.left = 260;  
		rc.top = 180;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("ok");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->isVGUARANTDemoLicAvailable())
		{
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			COleDateTime dtCurrentTime(stTime);
			COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetVGUARANTYdemoTimestamp();

			CStringW szDiff=_T("ERROR time stamp");;

			if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
			{
				if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
				{
					COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

					int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
					szDiff.Format(_T("%d days left"), iDaysLeft);
				}
			}

			rc.left = 330;  
			rc.top = 180;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,szDiff,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			rc.left = 330;  
			rc.top = 180;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,m_szEncryptedFormattedVGUARANTYKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		

		if(false==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
		{
			m_pcDelVguaranty->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcDelVguaranty->ShowWindow(SW_HIDE);

			rc.left = 600;  
			rc.right  = 760;  
			rc.top = 165;  
			rc.bottom = 213;
			cs = _T("- VG active -");
			DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}

	}
	else
	{
		m_pcActivateVguaranty->ShowWindow(SW_SHOW);
		rc.left = 260;  
		rc.top = 180;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelVguaranty->ShowWindow(SW_HIDE);
	}

	rc.left = 30;  
	rc.top = 230;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Volume limit:");
	if(getModel()->getDATAHANDLER()->isVLIMITDemoLicAvailable())
	{
		cs += _T("(demo)");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bVLIMITModuleEnabled)
	{
		m_pcActivateVlimit->ShowWindow(SW_HIDE);
		rc.left = 260;  
		rc.top = 230;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("ok");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->isVLIMITDemoLicAvailable())
		{
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			COleDateTime dtCurrentTime(stTime);
			COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetVLIMITdemoTimestamp();

			CStringW szDiff=_T("ERROR time stamp");;

			if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
			{
				if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
				{
					COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

					int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
					szDiff.Format(_T("%d days left"), iDaysLeft);
				}
			}

			rc.left = 330;  
			rc.top = 230;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,szDiff,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			rc.left = 330;  
			rc.top = 230;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,m_szEncryptedFormattedVLIMITKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}

		m_pcDelVlimit->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateVlimit->ShowWindow(SW_SHOW);
		rc.left = 260;  
		rc.top = 230;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(false==getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn())
		{
			m_pcDelVlimit->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcDelVlimit->ShowWindow(SW_HIDE);

			rc.left = 600;  
			rc.right  = 760;  
			rc.top = 215;  
			rc.bottom = 263;
			cs = _T("- VL active -");
			DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
	}


	rc.left = 30;  
	rc.top = 280;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Lung recruitment:");
	if(getModel()->getDATAHANDLER()->isLUNGRECDemoLicAvailable())
	{
		cs += _T("(demo)");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bLUNGRECModuleEnabled)
	{
		m_pcActivateLungRec->ShowWindow(SW_HIDE);
		rc.left = 260;  
		rc.top = 280;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("ok");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->isLUNGRECDemoLicAvailable())
		{
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			COleDateTime dtCurrentTime(stTime);
			COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetLUNGRECdemoTimestamp();

			CStringW szDiff=_T("ERROR time stamp");;

			if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
			{
				if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
				{
					COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

					int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
					szDiff.Format(_T("%d days left"), iDaysLeft);
				}
			}

			rc.left = 330;  
			rc.top = 280;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,szDiff,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			rc.left = 330;  
			rc.top = 280;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,m_szEncryptedFormattedLUNGRECKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		

		m_pcDelLungRec->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateLungRec->ShowWindow(SW_SHOW);
		rc.left = 260;  
		rc.top = 280;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()==0)
		{
			m_pcDelLungRec->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcDelLungRec->ShowWindow(SW_HIDE);

			rc.left = 600;  
			rc.right  = 760;  
			rc.top = 265;  
			rc.bottom = 313;
			cs = _T("- LungRec active -");
			DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
	}



	rc.left = 30;  
	rc.top = 330;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Trend:");
	if(getModel()->getDATAHANDLER()->isTRENDDemoLicAvailable())
	{
		cs += _T("(demo)");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bTRENDSModuleEnabled)
	{
		m_pcActivateTrends->ShowWindow(SW_HIDE);
		rc.left = 260;  
		rc.top = 330;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("ok");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->isTRENDDemoLicAvailable())
		{
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			COleDateTime dtCurrentTime(stTime);
			COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetTRENDdemoTimestamp();

			CStringW szDiff=_T("ERROR time stamp");;

			if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
			{
				if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
				{
					COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

					int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
					szDiff.Format(_T("%d days left"), iDaysLeft);
				}
			}

			rc.left = 330;  
			rc.top = 330;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,szDiff,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			rc.left = 330;  
			rc.top = 330;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,m_szEncryptedFormattedTRENDSKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		

		m_pcDelTrends->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateTrends->ShowWindow(SW_SHOW);
		rc.left = 260;  
		rc.top = 330;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelTrends->ShowWindow(SW_HIDE);
	}


	rc.left = 30;  
	rc.top = 380;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("O2 Therapy:");
	if(getModel()->getDATAHANDLER()->isTHERAPYDemoLicAvailable())
	{
		cs += _T("(demo)");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bTHERAPYModuleEnabled)
	{
		m_pcActivateTherapy->ShowWindow(SW_HIDE);
		rc.left = 260;  
		rc.top = 380;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("ok");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->isTHERAPYDemoLicAvailable())
		{
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			COleDateTime dtCurrentTime(stTime);
			COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetTHERAPYdemoTimestamp();

			CStringW szDiff=_T("ERROR time stamp");;

			if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
			{
				if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
				{
					COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

					int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
					szDiff.Format(_T("%d days left"), iDaysLeft);
				}
			}

			rc.left = 330;  
			rc.top = 380;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,szDiff,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			rc.left = 330;  
			rc.top = 380;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,m_szEncryptedFormattedTHERAPYKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		

		//if(getModel()->getCONFIG()->GetPrevMode()!=VM_THERAPIE)
		{
			m_pcDelTherapy->ShowWindow(SW_SHOW);
		}
		/*else
		{
			m_pcDelTherapy->ShowWindow(SW_HIDE);

			rc.left = 600;  
			rc.right  = 760;  
			rc.top = 365;  
			rc.bottom = 413;
			cs = _T("- O2Therapy active -");
			DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}*/
	}
	else
	{
		m_pcActivateTherapy->ShowWindow(SW_SHOW);
		rc.left = 260;  
		rc.top = 380;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelTherapy->ShowWindow(SW_HIDE);
	}


	rc.left = 30;  
	rc.top = 430;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("PRICO:");
	if(getModel()->getDATAHANDLER()->isPRICODemoLicAvailable())
	{
		cs += _T("(demo)");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bPRICOModuleEnabled)
	{
		m_pcActivatePRICO->ShowWindow(SW_HIDE);
		rc.left = 260;  
		rc.top = 430;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("ok");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		if(getModel()->getDATAHANDLER()->isPRICODemoLicAvailable())
		{
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			COleDateTime dtCurrentTime(stTime);
			COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetPRICOdemoTimestamp();

			CStringW szDiff=_T("ERROR time stamp");;

			if(dtLicenseTimestamp.GetStatus()==COleDateTime::valid)
			{
				if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
				{
					COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

					int iDaysLeft=DEMOLICENSE-dtElapsePeriod.GetTotalDays();
					szDiff.Format(_T("%d days left"), iDaysLeft);
				}
			}

			rc.left = 330;  
			rc.top = 430;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,szDiff,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			rc.left = 330;  
			rc.top = 430;  
			rc.right  = 760;  
			rc.bottom = 500;
			DrawText(hdcMem,m_szEncryptedFormattedPRICOKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
		

		if(false==getModel()->getPRICOThread()->isPRICOalgorithmRunning())
		{
			m_pcDelPRICO->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcDelPRICO->ShowWindow(SW_HIDE);

			rc.left = 600;  
			rc.right  = 760;  
			rc.top = 415;  
			rc.bottom = 463;
			cs = _T("- PRICO active -");
			DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
	}
	else
	{
		m_pcActivatePRICO->ShowWindow(SW_SHOW);
		rc.left = 260;  
		rc.top = 430;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelPRICO->ShowWindow(SW_HIDE);
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

//************************************
// Method:    OnDestroy
// FullName:  CWndServiceLicensing::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndServiceLicensing::OnDestroy()
{
	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	CWndService::OnDestroy();
}
