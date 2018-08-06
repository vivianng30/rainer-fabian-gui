
#include "stdafx.h"
#include "TlsLanguage.h"

#ifdef _DEBUG
#include "TlsDbg.h"
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================
CTlsLanguage::FileParser::FileParser(CStringW sFileName)
: m_iLookup(-1), m_hIn(NULL)
{
  m_hIn = _wfopen(sFileName, _T("rt"));
}

//=============================================================================
CTlsLanguage::FileParser::~FileParser()
{
  if(m_hIn)
    fclose(m_hIn);
}

//=============================================================================
int CTlsLanguage::FileParser::GetChar()
{
  int iRet = m_iLookup;
  if(iRet == -1)
    iRet = fgetc(m_hIn);
  else
    m_iLookup = -1;
  return iRet;
}

//=============================================================================
void CTlsLanguage::FileParser::Pushback(int i)
{
  m_iLookup = i;
}

//=============================================================================
bool CTlsLanguage::FileParser::IsEof()
{
  if(!m_hIn)
  {
	  //TlsMessageBox("eof", MB_OK);
    return true;
  }
  return feof(m_hIn)!=0;
}

//=============================================================================
void FormatMessage()
{
  LPVOID lpMsgBuf;
  FormatMessage( 
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM | 
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      GetLastError(),
      0, // Default language
      (LPTSTR) &lpMsgBuf,
      0,
      NULL 
  );
  // Process any inserts in lpMsgBuf.
  // ...
  // Display the string.
  CStringW sMessage=_T("");
  sMessage.Format((LPTSTR)lpMsgBuf);
  //TlsMessageBox( sMessage, MB_OK | MB_ICONINFORMATION );
  // Free the buffer.
  LocalFree( lpMsgBuf );
}
 
//=============================================================================
CTlsLanguage::ResParser::ResParser(HINSTANCE hm, int iResID)
  : m_iLookup(-1), m_pData(NULL), m_iData(-1), m_iSize(0)
{
	HRSRC hres = FindResource(hm, MAKEINTRESOURCE(iResID), RT_RCDATA);

	if(!hres)
  {
    FormatMessage();
		return;
  }
	
	HGLOBAL hglob = LoadResource(hm, hres);
	if(!hglob)
		return;
	
	m_iSize = ::SizeofResource(hm, hres);
	m_pData = (char*)LockResource(hglob);
  m_iData = 0;
}

//=============================================================================
CTlsLanguage::ResParser::~ResParser()
{
  // no free needed
}

//=============================================================================
int CTlsLanguage::ResParser::GetChar()
{
  int iRet = m_iLookup;
  if(iRet == -1)
    iRet = m_pData[m_iData++];
  else
    m_iLookup = -1;
  return iRet;
}

//=============================================================================
void CTlsLanguage::ResParser::Pushback(int i)
{
  m_iLookup = i;
}

//=============================================================================
bool CTlsLanguage::ResParser::IsEof()
{
  return (m_pData == NULL) || (m_iData > m_iSize);
}

//=============================================================================
CTlsLanguage::CTlsLanguage()
  : m_eLnCurrLang(LN_NONE)
{
}

//=============================================================================
CTlsLanguage::~CTlsLanguage()
{
}

//=============================================================================
void CTlsLanguage::AddListener(CTlsLanguageListener* pListener)
{
	//TlsMessageBox("test", "lang", MB_OK);
  m_Listeners.AddTail(pListener);

  // init the listener
  pListener->LanguageChanged(this);
}

//=============================================================================
void CTlsLanguage::RemoveListener(CTlsLanguageListener* pListener)
{
  POSITION pos = m_Listeners.Find(pListener);
  if(pos)
    m_Listeners.RemoveAt(pos);
}

//=============================================================================
void CTlsLanguage::ChangeLanguage(eLanguages eLnCurrLang)
{
  if(m_eLnCurrLang == eLnCurrLang)
    return; 

  m_eLnCurrLang = eLnCurrLang;

  POSITION pos = m_Listeners.GetHeadPosition();
  for (int i=0;i < m_Listeners.GetCount();i++)
  {
    CTlsLanguageListener* pCurr = m_Listeners.GetNext(pos);
    pCurr->LanguageChanged(this);
  }
}

//=============================================================================
CTlsLanguage::eLanguages CTlsLanguage::GetCurrLanguage()
{
  return m_eLnCurrLang;
}

//=============================================================================
CTlsLanguage::Language::~Language()
{
/*
  POSITION p = m_Groups.GetStartPosition();
  while(p)
  {
    CStringW sKey;
    void* pVoid;
    m_Groups.GetNextAssoc(p, sKey, pVoid);
    LanguageGroup* pGroup = (LanguageGroup*)pVoid;
    delete pGroup;
  }
*/
}

//=============================================================================
void CTlsLanguage::Language::AddKey(CStringW sGroup, CStringW sKey, CStringW sValue)
{
  void* pVoid = NULL;
  LanguageGroup* pGroup = NULL;
  if(!m_Groups.Lookup(sGroup, pVoid))
  {
	  //TlsMessageBox("new gruppe", sGroup, MB_OK);
    pGroup = new LanguageGroup();
#ifdef AIS_MEMDEBUG
#ifdef _DEBUG
	CTlsDebugHeap::GetInstance().RemoveTrace((unsigned long)pGroup);
#endif
#endif
    m_Groups.SetAt(sGroup, pGroup);
  }
  else
  {
    pGroup = (LanguageGroup*)pVoid;
	
  }

  pGroup->m_Elements.SetAt(sKey, sValue);
}

//=============================================================================
CStringW CTlsLanguage::Language::GetKey(CStringW sGroup, CStringW sKey)
{
  void* pVoid = NULL;
  LanguageGroup* pGroup = NULL;
  if(!m_Groups.Lookup(sGroup, pVoid))
  {
	  //TlsMessageBox("falsche gruppe", sGroup, MB_OK);
    return L"Invalid Group !!";
  }

  pGroup = (LanguageGroup*)pVoid;

  CStringW sRet;
  pGroup->m_Elements.Lookup(sKey, sRet);

  return sRet;
}


//=============================================================================
CMapStringToString& CTlsLanguage::Language::GetGroup(CStringW sGroup)
{
  void* pVoid = NULL;
  LanguageGroup* pGroup = NULL;
  if(!m_Groups.Lookup(sGroup, pVoid))
  {
    static CMapStringToString dummy;
    return dummy;
  }

  pGroup = (LanguageGroup*)pVoid;

  return pGroup->m_Elements;
}

//=============================================================================
bool CTlsLanguage::parseField(LangParser* pParser, char* pszBuffer)
{
  //bool bEscape = false;
  bool bFirst  = true;

  while(!pParser->IsEof())
  {
    int i1 = pParser->GetChar();
    if(i1 == -1)
      break;
    if(bFirst && i1 == '\"')
    {
      while(!pParser->IsEof())
      {
        i1 = pParser->GetChar();
        if(i1 == '\"')
        {
          i1 = pParser->GetChar();
          if(i1 != '\"')
            break;
        }
        *pszBuffer++ = (char) i1;
      }
    }

    if(i1 == '\r')
    {
      int i2 = pParser->GetChar();
      if(i2 == '\n')
      {
        pParser->Pushback('\n');
        break;
      }
      pParser->Pushback(i2);
      break;
    }
    if(i1 == '\n')
    {
      pParser->Pushback('\n');
      break;
    }
    if(i1 == ';')
      break;

    bFirst = false;

    *pszBuffer++ = (char) i1;
  }
  *pszBuffer = '\0';
  return true;
}

//=============================================================================
void CTlsLanguage::skipLine(LangParser* pParser)
{
  while(!pParser->IsEof())
  {
    int i1 = pParser->GetChar();

    if(i1 == '\r')
    {
      int i2 = pParser->GetChar();
      if(i2 != '\n')
        pParser->Pushback(i2);
      return;
    }

    if(i1 == '\n')
      return;
  }
}

//=============================================================================
bool CTlsLanguage::InitLanguages(CStringW sFileName)
{
	bool bSuccess = false;

  FileParser Parser(sFileName);

  InitLanguages(&Parser);

  return bSuccess;
}

//=============================================================================
bool CTlsLanguage::InitLanguages(HINSTANCE hm, int iResID)
{
	bool bSuccess = false;

  ResParser Parser(hm, iResID);

  InitLanguages(&Parser);

  return bSuccess;
}

#define MAX_FIELD_LEN 4096

//=============================================================================
bool CTlsLanguage::InitLanguages(LangParser* pParser)
{
  int iLine = 0;
  while (!pParser->IsEof()) 
  {
    iLine ++;
    
    skipLine(pParser);
    char szBuffer[MAX_FIELD_LEN];
    
    if(!parseField(pParser, szBuffer))
      continue;
    CStringW sGroup(szBuffer);
	//TlsMessageBox("sGroup", sGroup, MB_OK);
    if(!parseField(pParser, szBuffer))
      continue;
    CStringW sKey(szBuffer);
	//TlsMessageBox("sKey", sKey, MB_OK);
    if(!parseField(pParser, szBuffer))
      continue;
    CStringW sFlags(szBuffer);
    if(!parseField(pParser, szBuffer))
      continue;
    CStringW sGerman(szBuffer);
    if(!parseField(pParser, szBuffer))
      continue;
    CStringW sItalian(szBuffer);
    if(!parseField(pParser, szBuffer))
      continue;
    CStringW sFrench(szBuffer);
	if(!parseField(pParser, szBuffer))
      continue;
    CStringW sEnglish(szBuffer);
    //TlsMessageBox("sEnglish", sEnglish, MB_OK);
    m_Languages[LN_GERMAN ].AddKey(sGroup, sKey, sGerman);
    m_Languages[LN_ITALIAN].AddKey(sGroup, sKey, sItalian);
    m_Languages[LN_FRENCH ].AddKey(sGroup, sKey, sFrench);
	m_Languages[LN_ENGLISH].AddKey(sGroup, sKey, sEnglish);
  }
  
//  m_eLnCurrLang = LN_GERMAN;
  
  return true;
}

//=============================================================================
bool CTlsLanguage::IsInited()
{
  return m_eLnCurrLang != LN_NONE;
}

//=============================================================================
//
//\brief Get a language String for 'key' for the current language
//
//=============================================================================
CStringW CTlsLanguage::GetLangString(CStringW sGroup, CStringW sKey)
{
  if(m_eLnCurrLang == LN_NONE)
    return L"No Current Language";

  return m_Languages[m_eLnCurrLang].GetKey(sGroup, sKey);
}

//=============================================================================
//
//\brief Get a language group for the current language
//
//=============================================================================
CMapStringToString& CTlsLanguage::GetLangGroup(CStringW sGroup)
{
  return m_Languages[m_eLnCurrLang].GetGroup(sGroup);
}

//=============================================================================
//
//\brief singleton
//
//=============================================================================
CTlsLanguage& CTlsLanguage::GetInstance()
{
  static CTlsLanguage TheOneAndOnly;
  return TheOneAndOnly;
}




/*=============================================================================
    E O F
=============================================================================*/
