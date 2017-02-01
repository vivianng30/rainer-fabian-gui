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

	//TEST
	//bCheckMasterFeature=false;
	
	//if(bCheckMasterFeature)
	//{
	//	CStringA szEncryptedMasterKey=getModel()->getDATAHANDLER()->encryptKey(MOD_MASTER);

	//	char szHex[30];
	//	//strcpy(szHex,szEncryptedMasterKey);
	//	strcpy_s(szHex,_countof(szHex),szEncryptedMasterKey);

	//	m_szEncryptedFormattedMasterKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedMasterKey);

	//	//check master key
	//	if(m_szParsedMasterKey==szHex)
	//	{
	//		m_bMasterModuleEnabled=true;
	//	}
	//}
	//else
	{
		if(bCheckHFOFeature)
		{
			CStringA szEncryptedHFOKey=getModel()->getDATAHANDLER()->encryptKey(MOD_HFO);

			char szHex[30];
			//strcpy(szHex,szEncryptedHFOKey);
			strcpy_s(szHex,_countof(szHex),szEncryptedHFOKey);

			m_szEncryptedFormattedHFOKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedHFOKey);

			//check master key
			if(m_szParsedHFOKey==szHex)
			{
				m_bHFOModuleEnabled=true;
			}
		}
		if(bCheckNMODEFeature)
		{
			CStringA szEncryptedNMODEKey=getModel()->getDATAHANDLER()->encryptKey(MOD_NMODE);

			char szHex[30];
			//strcpy(szHex,szEncryptedNMODEKey);
			strcpy_s(szHex,_countof(szHex),szEncryptedNMODEKey);

			m_szEncryptedFormattedNMODEKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedNMODEKey);
			
			//check master key
			if(m_szParsedNMODEKey==szHex)
			{
				m_bNMODEModuleEnabled=true;
			}
		}
		if(bCheckVGUARANTFeature)
		{
			CStringA szEncryptedVGUARANTYKey=getModel()->getDATAHANDLER()->encryptKey(MOD_VGUARANTY);

			char szHex[30];
			//strcpy(szHex,szEncryptedVGUARANTYKey);
			strcpy_s(szHex,_countof(szHex),szEncryptedVGUARANTYKey);

			m_szEncryptedFormattedVGUARANTYKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedVGUARANTYKey);

			//check master key
			if(m_szParsedVGUARANTYKey==szHex)
			{
				m_bVGUARANTModuleEnabled=true;
			}
		}
		if(bCheckVLIMITFeature)
		{
			CStringA szEncryptedVLIMITKey=getModel()->getDATAHANDLER()->encryptKey(MOD_VLIMIT);

			char szHex[30];
			//strcpy(szHex,szEncryptedVLIMITKey);
			strcpy_s(szHex,_countof(szHex),szEncryptedVLIMITKey);

			m_szEncryptedFormattedVLIMITKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedVLIMITKey);

			//check master key
			if(m_szParsedVLIMITKey==szHex)
			{
				m_bVLIMITModuleEnabled=true;
			}
		}
		if(bCheckLUNGRECFeature)
		{
			CStringA szEncryptedLUNGRECKey=getModel()->getDATAHANDLER()->encryptKey(MOD_LUNGREC);

			char szHex[30];
			//strcpy(szHex,szEncryptedVLIMITKey);
			strcpy_s(szHex,_countof(szHex),szEncryptedLUNGRECKey);

			m_szEncryptedFormattedLUNGRECKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedLUNGRECKey);

			//check master key
			if(m_szParsedLUNGRECKey==szHex)
			{
				m_bLUNGRECModuleEnabled=true;
			}
		}
		if(bCheckTRENDSFeature)
		{
			CStringA szEncryptedTRENDSKey=getModel()->getDATAHANDLER()->encryptKey(MOD_TREND);

			char szHex[30];
			strcpy_s(szHex,_countof(szHex),szEncryptedTRENDSKey);

			m_szEncryptedFormattedTRENDSKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedTRENDSKey);

			//check master key
			if(m_szParsedTRENDSKey==szHex)
			{
				m_bTRENDSModuleEnabled=true;
			}
		}
		if(bCheckTHERAPYFeature)
		{
			CStringA szEncryptedTHERAPYKey=getModel()->getDATAHANDLER()->encryptKey(MOD_THERAPY);

			char szHex[30];
			strcpy_s(szHex,_countof(szHex),szEncryptedTHERAPYKey);

			m_szEncryptedFormattedTHERAPYKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedTHERAPYKey);

			//check master key
			if(m_szParsedTHERAPYKey==szHex)
			{
				m_bTHERAPYModuleEnabled=true;
			}
		}
		if(bCheckPRICOFeature)
		{
			CStringA szEncryptedPRICOKey=getModel()->getDATAHANDLER()->encryptKey(MOD_PRICO);

			char szHex[30];
			strcpy_s(szHex,_countof(szHex),szEncryptedPRICOKey);

			m_szEncryptedFormattedPRICOKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedPRICOKey);

			if(m_szParsedPRICOKey==szHex)
			{
				m_bPRICOModuleEnabled=true;
			}
		}
		if(bCheckFOTFeature)
		{
			CStringA szEncryptedFOTKey=getModel()->getDATAHANDLER()->encryptKey(MOD_FOT);

			char szHex[30];
			strcpy_s(szHex,_countof(szHex),szEncryptedFOTKey);

			m_szEncryptedFormattedFOTKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedFOTKey);

			if(m_szParsedFOTKey==szHex)
			{
				m_bFOTModuleEnabled=true;
			}
		}
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

	rc.left = 50;  
	rc.top = 30;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("FOT");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	if(m_bFOTModuleEnabled)
	{
		m_pcActivateFOT->ShowWindow(SW_HIDE);
		rc.left = 230;  
		rc.top = 30;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("enabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 330;  
		rc.top = 30;  
		rc.right  = 760;  
		rc.bottom = 500;
		DrawText(hdcMem,m_szEncryptedFormattedFOTKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelFOT->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateFOT->ShowWindow(SW_SHOW);
		rc.left = 230;  
		rc.top = 30;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelFOT->ShowWindow(SW_HIDE);
	}

	rc.left = 50;  
	rc.top = 80;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("HFO:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bHFOModuleEnabled)
	{
		m_pcActivateHFO->ShowWindow(SW_HIDE);
		rc.left = 230;  
		rc.top = 80;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("enabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 330;  
		rc.top = 80;  
		rc.right  = 760;  
		rc.bottom = 500;
		DrawText(hdcMem,m_szEncryptedFormattedHFOKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelHFO->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateHFO->ShowWindow(SW_SHOW);
		rc.left = 230;  
		rc.top = 80;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelHFO->ShowWindow(SW_HIDE);

	}

	

	rc.left = 50;  
	rc.top = 130;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("NMode:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bNMODEModuleEnabled)
	{
		m_pcActivateNMODE->ShowWindow(SW_HIDE);
		rc.left = 230;  
		rc.top = 130;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("enabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 330;  
		rc.top = 130;  
		rc.right  = 760;  
		rc.bottom = 500;
		DrawText(hdcMem,m_szEncryptedFormattedNMODEKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelNMODE->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateNMODE->ShowWindow(SW_SHOW);
		rc.left = 230;  
		rc.top = 130;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelNMODE->ShowWindow(SW_HIDE);
	}

	rc.left = 50;  
	rc.top = 180;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Volume guaranty:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bVGUARANTModuleEnabled)
	{
		m_pcActivateVguaranty->ShowWindow(SW_HIDE);
		rc.left = 230;  
		rc.top = 180;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("enabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 330;  
		rc.top = 180;  
		rc.right  = 760;  
		rc.bottom = 500;
		DrawText(hdcMem,m_szEncryptedFormattedVGUARANTYKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelVguaranty->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateVguaranty->ShowWindow(SW_SHOW);
		rc.left = 230;  
		rc.top = 180;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelVguaranty->ShowWindow(SW_HIDE);
	}

	rc.left = 50;  
	rc.top = 230;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Volume limit:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bVLIMITModuleEnabled)
	{
		m_pcActivateVlimit->ShowWindow(SW_HIDE);
		rc.left = 230;  
		rc.top = 230;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("enabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 330;  
		rc.top = 230;  
		rc.right  = 760;  
		rc.bottom = 500;
		DrawText(hdcMem,m_szEncryptedFormattedVLIMITKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelVlimit->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateVlimit->ShowWindow(SW_SHOW);
		rc.left = 230;  
		rc.top = 230;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelVlimit->ShowWindow(SW_HIDE);
	}


	rc.left = 50;  
	rc.top = 280;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Lung recruitment:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bLUNGRECModuleEnabled)
	{
		m_pcActivateLungRec->ShowWindow(SW_HIDE);
		rc.left = 230;  
		rc.top = 280;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("enabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 330;  
		rc.top = 280;  
		rc.right  = 760;  
		rc.bottom = 500;
		DrawText(hdcMem,m_szEncryptedFormattedLUNGRECKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelLungRec->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateLungRec->ShowWindow(SW_SHOW);
		rc.left = 230;  
		rc.top = 280;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelLungRec->ShowWindow(SW_HIDE);
	}



	rc.left = 50;  
	rc.top = 330;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Trend:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bTRENDSModuleEnabled)
	{
		m_pcActivateTrends->ShowWindow(SW_HIDE);
		rc.left = 230;  
		rc.top = 330;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("enabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 330;  
		rc.top = 330;  
		rc.right  = 760;  
		rc.bottom = 500;
		DrawText(hdcMem,m_szEncryptedFormattedTRENDSKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelTrends->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateTrends->ShowWindow(SW_SHOW);
		rc.left = 230;  
		rc.top = 330;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelTrends->ShowWindow(SW_HIDE);
	}


	rc.left = 50;  
	rc.top = 380;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("O2 Therapy:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bTHERAPYModuleEnabled)
	{
		m_pcActivateTherapy->ShowWindow(SW_HIDE);
		rc.left = 230;  
		rc.top = 380;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("enabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 330;  
		rc.top = 380;  
		rc.right  = 760;  
		rc.bottom = 500;
		DrawText(hdcMem,m_szEncryptedFormattedTHERAPYKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelTherapy->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivateTherapy->ShowWindow(SW_SHOW);
		rc.left = 230;  
		rc.top = 380;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("disabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelTherapy->ShowWindow(SW_HIDE);
	}


	rc.left = 50;  
	rc.top = 430;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("PRICO:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bPRICOModuleEnabled)
	{
		m_pcActivatePRICO->ShowWindow(SW_HIDE);
		rc.left = 230;  
		rc.top = 430;  
		rc.right  = 760;  
		rc.bottom = 500;
		cs = _T("enabled");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 330;  
		rc.top = 430;  
		rc.right  = 760;  
		rc.bottom = 500;
		DrawText(hdcMem,m_szEncryptedFormattedPRICOKey,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcDelPRICO->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcActivatePRICO->ShowWindow(SW_SHOW);
		rc.left = 230;  
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

//void CWndServiceLicensing::WriteLicenseFile()
//{
//	CTlsOStream_File os(m_szLicenseFile);
//	CTlsXmlWriter xml(&os);
//
//	CStringW szEncryptedMasterKey=_T("");
//	szEncryptedMasterKey+=m_szParsedMasterKey;
//	CString szEncryptedHFOKey=_T("");
//	szEncryptedHFOKey+=m_szParsedHFOKey;
//	/*CString szEncryptedCO2Key=_T("0");
//	szEncryptedCO2Key+=m_szParsedCO2Key;*/
//	CString szEncryptedNMODEKey=_T("");
//	szEncryptedNMODEKey+=m_szParsedNMODEKey;
//	CString szEncryptedVGUARANTYKey=_T("");
//	szEncryptedVGUARANTYKey+=m_szParsedVGUARANTYKey;
//	CString szEncryptedVLIMITKey=_T("");
//	szEncryptedVLIMITKey+=m_szParsedVLIMITKey;
//	CString szEncryptedTRENDSKey=_T("");
//	szEncryptedTRENDSKey+=m_szParsedTRENDSKey;
//	CString szEncryptedLUNGRECKey=_T("");
//	szEncryptedLUNGRECKey+=m_szParsedLUNGRECKey;
//	CString szEncryptedTHERAPYKey=_T("");
//	szEncryptedTHERAPYKey+=m_szParsedTHERAPYKey;
//	CString szEncryptedPRICOKey=_T("");
//	szEncryptedPRICOKey+=m_szParsedPRICOKey;
//
//	xml.OpenTag(_T("FabianHFO"), true);
//
//	int iSizeOfArray = 9;
//	if(0 < iSizeOfArray)
//	{
//		for(int iIndex = 0; iIndex < iSizeOfArray; iIndex++)	
//		{																
//			xml.OpenTag(_T("License"), true);
//
//			switch(iIndex)
//			{
//			case 0:
//				{
//					xml.WriteTextTag(_T("MODULE"), _T("MASTER2"));
//					//xml.WriteTextTag(_T("KEY"), _T("06DFA88151EB4629F80D"));
//					xml.WriteTextTag(_T("KEY"), szEncryptedMasterKey);
//				}
//				break;
//			case 1:
//				{
//					xml.WriteTextTag(_T("MODULE"), _T("HFO"));
//					//xml.WriteTextTag(_T("KEY"), _T("0F8FA61401714C32BECE"));
//					xml.WriteTextTag(_T("KEY"), szEncryptedHFOKey);
//				}
//				break;
//			case 2:
//				{
//					xml.WriteTextTag(_T("MODULE"), _T("NMODE"));
//					//xml.WriteTextTag(_T("KEY"), _T("00D9D41FBCF263CB1737"));
//					xml.WriteTextTag(_T("KEY"), szEncryptedNMODEKey);
//				}
//				break;
//			case 3:
//				{
//					xml.WriteTextTag(_T("MODULE"), _T("VGUARANTY"));
//					//xml.WriteTextTag(_T("KEY"), _T("6607A0E9629CDD0DBD5B"));
//					xml.WriteTextTag(_T("KEY"), szEncryptedVGUARANTYKey);
//				}
//				break;
//			case 4:
//				{
//					xml.WriteTextTag(_T("MODULE"), _T("VLIMIT"));
//					//xml.WriteTextTag(_T("KEY"), _T("4AD1BE6777DDE0D5E697"));
//					xml.WriteTextTag(_T("KEY"), szEncryptedVLIMITKey);
//				}
//				break;
//			case 5:
//				{
//					xml.WriteTextTag(_T("MODULE"), _T("LUNGREC"));
//					//xml.WriteTextTag(_T("KEY"), _T("4AD1BE6777DDE0D5E697"));
//					xml.WriteTextTag(_T("KEY"), szEncryptedLUNGRECKey);
//				}
//				break;
//			case 6:
//				{
//					xml.WriteTextTag(_T("MODULE"), _T("TRENDS"));
//					//xml.WriteTextTag(_T("KEY"), _T("4AD1BE6777DDE0D5E697"));
//					xml.WriteTextTag(_T("KEY"), szEncryptedTRENDSKey);
//				}
//				break;
//			case 7:
//				{
//					xml.WriteTextTag(_T("MODULE"), _T("THERAPY"));
//					//xml.WriteTextTag(_T("KEY"), _T("4AD1BE6777DDE0D5E697"));
//					xml.WriteTextTag(_T("KEY"), szEncryptedTHERAPYKey);
//				}
//				break;
//			case 8:
//				{
//					xml.WriteTextTag(_T("MODULE"), _T("PRICO"));
//					//xml.WriteTextTag(_T("KEY"), _T("4AD1BE6777DDE0D5E697"));
//					xml.WriteTextTag(_T("KEY"), szEncryptedPRICOKey);
//				}
//				break;
//			default:
//				break;
//			}
//
//			xml.CloseTag(_T("License"), true);
//		}
//	}
//
//	xml.CloseTag(_T("FabianHFO"), true);
//
//	os.Close();
//}

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
		if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true)
		{
			m_szParsedLUNGRECKey=_T("0");
			m_bLUNGRECModuleEnabled=false;
			m_szEncryptedFormattedLUNGRECKey=_T("");
			getModel()->getDATAHANDLER()->disableLUNGRECLicense();
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
