
#if !defined(__TLSLANGUAGE_H__)
#define __TLSLANGUAGE_H__

#include <afxext.h>         // MFC extensions

class CTlsLanguage;

class CTlsLanguageListener
{
public:
  virtual void LanguageChanged(CTlsLanguage* pLanguage)=0;
};

class CTlsLanguage  
{
public:
  enum eLanguages {LN_NONE=-1, LN_GERMAN=0, LN_ITALIAN=1, LN_FRENCH=2, LN_ENGLISH=3};

private:
  eLanguages m_eLnCurrLang;
  CList <CTlsLanguageListener*, CTlsLanguageListener*> m_Listeners;

  class LanguageGroup
  {
  public:
    CMapStringToString m_Elements;
  };
  
  class Language
  {
  public:
    CMapStringToPtr m_Groups;
    ~Language();
    
    void AddKey(CStringW sGroup, CStringW sKey, CStringW sValue);
    CStringW GetKey(CStringW sGroup, CStringW sKey);
    CMapStringToString&  GetGroup(CStringW sGroup);
  };
  
  Language m_Languages[4];

  class LangParser
  {
  public:
    virtual ~LangParser() {}
    virtual int GetChar()=0;
    virtual void Pushback(int i)=0;
    virtual bool IsEof()=0;
  };

  class FileParser : public LangParser
  {
    FILE* m_hIn;
    int m_iLookup;
  public:
    FileParser(CStringW sFile);
    virtual ~FileParser();
    int GetChar();
    void Pushback(int i);
    bool IsEof();
  };

  class ResParser : public LangParser
  {
    int m_iLookup;
    char* m_pData;
    int m_iData;
    int m_iSize;
    
  public:
    ResParser(HINSTANCE hm, int iResID);
    virtual ~ResParser();
    int GetChar();
    void Pushback(int i);
    bool IsEof();
  };

  bool parseField(LangParser* pParser, char* pszBuffer);
  void skipLine(LangParser* pParser);
  bool InitLanguages(LangParser* pParser);

public:
	CTlsLanguage();
	virtual ~CTlsLanguage();

  void AddListener(CTlsLanguageListener* pListener);
  void RemoveListener(CTlsLanguageListener* pListener);

  void ChangeLanguage(eLanguages eLnCurrLang);
  eLanguages GetCurrLanguage();

  bool IsInited();

  bool InitLanguages(CStringW sFileName);
  bool InitLanguages(HINSTANCE hm, int iResID);
  
  CStringW GetLangString(CStringW sGroup, CStringW sKey);
  
  CMapStringToString& GetLangGroup(CStringW sGroup);

  static CTlsLanguage& GetInstance();
};

//UINT TlsMessageBox(CStringW sCaption, CStringW sText, UINT iFlags);
//UINT TlsMessageBox(CStringW sText, UINT iFlags);

#endif
/*=============================================================================
    E O F
=============================================================================*/
